#include "learn_states.h"
#include "base_states.h"

//Reset the timer for sending the next ping
void LearnTX::enter(RTCM_Link* radio) {
  if (!radio->_trainConfig) {
    if(radio->_trainSettings != nullptr)
        radio->_trainSettings = new RTCMSettings();

    radio->configureTraining();
    //Reset the training timers
    radio->_trainTime = 0;
    radio->_trainEnd = false;
  }
  
  if (!radio->_trainEnd) {
    //Send a training ping packet
    radio->_TXTrailer.ping  = 1; //This is an empty ping packet
    radio->_TXTrailer.ack   = 0; //This is not an ACK packet
    radio->_TXTrailer.train = 1; //This is a training packet
    radio->_packetSize = 2;
  } else {
    //Send an ACK packet
    radio->_TXTrailer.ping  = 0; //This is an empty ping packet
    radio->_TXTrailer.ack   = 1; //This is not an ACK packet
    radio->_TXTrailer.train = 1; //This is a training packet
    radio->_packetSize = 2;
  }

  radio->_sendPacket();
}

void LearnTX::update(RTCM_Link* radio) {
  if (radio->_transactionComplete == true) {//If dio0ISR has fired, we are done transmitting
    radio->_transactionComplete = false; //Reset ISR flag
    if (!radio->_trainEnd){
      radio->changeState(LearnDataWait::getInstance());
    } else {
      radio->configure(); //Setup radio with settings
      radio->changeState(RXStandby::getInstance());
    }
  }
}

//Reset the timer for sending the next ping
void LearnTX::exit(RTCM_Link* radio) {
  radio->_learnCycle = 0;
}

RadioStateBase& LearnTX::getInstance() {
  static LearnTX singleton;
  return singleton;
}

void LearnDataWait::enter(RTCM_Link* radio) {
  radio->_returnToRX();
}

void LearnDataWait::update(RTCM_Link* radio) {
  //Wait for Data. Once received, transmit training data
  if (radio->_transactionComplete == true) {//If dio0ISR has fired, a packet has arrived
    radio->_transactionComplete = false; //Reset ISR flag
    radio->changeState(LearnRXPacket::getInstance());
  }
  else if (radio->_learnCycle > 1000){
    radio->changeState(LearnTX::getInstance());
  }
  else if (radio->_trainTime > 120000) {
    radio->configure();
    radio->changeState(RXStandby::getInstance());
  }
}

RadioStateBase& LearnDataWait::getInstance() {
  static LearnDataWait singleton;
  return singleton;
}

void LearnRXPacket::update(RTCM_Link* radio) {
PacketType packetType = radio->identifyPacketType(); //Look at the packet we just received

  if (packetType == PacketType::TRAINING_DATA) {
    //The waiting node has responded with a data packet
    //Absorb training data and then return to normal operation

    //Apply new AES Key, netID, airSpeed, radioBandwidth, radioSpreadingFactor, radioCodingRate, and radioPreambleLength if available
      if (radio->_lastPacketSize == radio->_settings->trainDataPacketSize()) {//Error check, should be AES key + NetID
        //Move training data into settings
        uint16_t offset = 0;
        AESKey tempKey;
        for (uint8_t x = 0; x < radio->_settings->encryptionKey().size(); x++)
          tempKey[x] = radio->_lastPacket[x];
        radio->_settings->setEncryptionKey(tempKey);
        offset = sizeof(radio->_settings->encryptionKey());

        radio->_settings->setNetID(radio->_lastPacket[sizeof(radio->_settings->encryptionKey())]);
        offset += sizeof(radio->_settings->netID());

        uint32_t tempAirSpeed = 0;
        *((uint8_t*)(&tempAirSpeed) + 0) = radio->_lastPacket[offset + 0];
        *((uint8_t*)(&tempAirSpeed) + 1) = radio->_lastPacket[offset + 1];
        *((uint8_t*)(&tempAirSpeed) + 2) = radio->_lastPacket[offset + 2];
        *((uint8_t*)(&tempAirSpeed) + 3) = radio->_lastPacket[offset + 3];
        radio->_settings->setAirSpeed(tempAirSpeed);
        offset += sizeof(radio->_settings->airSpeed());

        float tempBandwidth = 0;
        *((uint8_t*)(&tempBandwidth) + 0) = radio->_lastPacket[offset + 0];
        *((uint8_t*)(&tempBandwidth) + 1) = radio->_lastPacket[offset + 1];
        *((uint8_t*)(&tempBandwidth) + 2) = radio->_lastPacket[offset + 2];
        *((uint8_t*)(&tempBandwidth) + 3) = radio->_lastPacket[offset + 3];
        offset += sizeof(radio->_settings->bandwidth());
        radio->_settings->setBandwidth(tempBandwidth);

        radio->_settings->setSpreadFactor(radio->_lastPacket[offset]);
        offset += sizeof(radio->_settings->spreadFactor());
    
        radio->_settings->setCodingRate(radio->_lastPacket[offset]);
        offset += sizeof(radio->_settings->codingRate());
    
        radio->_settings->setPreambleLength(radio->_lastPacket[offset]);
      }
    radio->_trainEnd = true;
    radio->changeState(LearnTX::getInstance());
  }

  //During training, only training packets are valid
  else {//PACKET_BAD, PACKET_NETID_MISMATCH, PACKET_ACK, PACKET_PING, PACKET_DATA
    radio->changeState(LearnDataWait::getInstance());
  }
}

RadioStateBase& LearnRXPacket::getInstance() {
  static LearnRXPacket singleton;
  return singleton;
}
