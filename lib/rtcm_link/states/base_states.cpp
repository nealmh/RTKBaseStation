#include "base_states.h"

void RXStandby::enter(RTCM_Link* radio) {
  radio->_returnToRX();
}

void RXStandby::update(RTCM_Link* radio) {
  if (radio->_transactionComplete == true) {//If dio0ISR has fired, a packet has arrived
    radio->_transactionComplete = false; //Reset ISR flag
    radio->changeState(RXPacket::getInstance());
  }
  else if (radio->_timeToHop == true) {//If the dio1ISR has fired, move to next frequency
    radio->_hopChannel();
  }
  else {//Transmit data if the radio is available and transmit data is ready
    if (radio->receiveInProcess() == false && radio->checkTX() == true) {
      radio->changeState(TX::getInstance());
    }
  } //End Process Waiting Serial
}

RadioStateBase& RXStandby::getInstance() {
	static RXStandby singleton;
	return singleton;
}

void RXPacket::update(RTCM_Link* radio) {
  PacketType packetType = radio->identifyPacketType(); //Look at the packet we just received

  if (packetType == PacketType::TRAINING_PING || packetType == PacketType::TRAINING_DATA) {
    //We should not be receiving ack or training packets packets, but if we do, just ignore
    radio->_frequencyCorrection += radio->_radio.getFrequencyError() / 1000000.0;
    radio->changeState(RXStandby::getInstance());
  }
  else if (packetType == PacketType::DATA) {
    //Move this packet into the receive buffer
    for (int x = 0 ; x < radio->_lastPacketSize ; x++) {
      radio->_rxBuffer[radio->_rxHead++] = radio->_lastPacket[x];
      radio->_rxHead %= sizeof(radio->_rxBuffer);
    }

    radio->_frequencyCorrection += radio->_radio.getFrequencyError() / 1000000.0;
    radio->changeState(RXStandby::getInstance());
  }
  else {//PACKET_BAD, PACKET_NETID_MISMATCH
    //This packet type is not supported in this state
    radio->changeState(RXStandby::getInstance());
  }
}

RadioStateBase& RXPacket::getInstance() {
	static RXPacket singleton;
	return singleton;
}


void TX::enter(RTCM_Link* radio) {
  uint16_t bytesToSend = radio->availableTXBytes();
  if (bytesToSend > radio->_settings->frameSize())
    bytesToSend = radio->_settings->frameSize();

  //SF6 requires an implicit header which means there is no dataLength in the header
  if (radio->_settings->spreadFactor() == 6) {
    if (bytesToSend > 255 - 3) 
      bytesToSend = 255 - 3; //We are going to transmit 255 bytes no matter what
  }

  radio->_packetSize = bytesToSend;

  //Move this portion of the circular buffer into the outgoingPacket
  for (uint8_t x = 0 ; x < radio->_packetSize ; x++) {
    radio->_outgoingPacket[x] = radio->_txBuffer[radio->_txTail++];
    radio->_txTail %= sizeof(radio->_txBuffer);
  }

  radio->_TXTrailer.ping  = 0; //This is not an empty ping packet
  radio->_TXTrailer.ack   = 0; //This is not an ACK packet
  radio->_TXTrailer.train = 0; //This is not a training packet

  radio->_packetSize += 2; //Make room for control bytes

  //SF6 requires an implicit header which means there is no dataLength in the header
  if (radio->_settings->spreadFactor() == 6) {
    //Manually store actual data length 3 bytes from the end (before NetID)
    //Manual packet size is whatever has been processed + 1 for the manual packetSize byte
    radio->_outgoingPacket[255 - 3] = radio->_packetSize + 1;
    radio->_packetSize = 255; //We're now going to transmit 255 bytes
  }
  radio->_sendPacket();
}

void TX::update(RTCM_Link* radio) {
  if (radio->_transactionComplete == true) {//If dio0ISR has fired, we are done transmitting
    radio->_transactionComplete = false; //Reset ISR flag
    radio->changeState(RXStandby::getInstance()); //No ack response when in broadcasting mode
  }
  else if (radio->_timeToHop == true) //If the dio1ISR has fired, move to next frequency
    radio->_hopChannel();
}

RadioStateBase& TX::getInstance()
{
	static TX singleton;
	return singleton;
}