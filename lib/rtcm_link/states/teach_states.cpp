#include "teach_states.h"
#include "base_states.h"

void TeachRXWait::enter(RTCM_Link* radio) {
  if (!radio->_trainConfig){
    if(radio->_trainSettings != nullptr)
        radio->_trainSettings = new RTCMSettings();
    radio->configureTraining();
    radio->_trainTime = 0;
    radio->_trainEnd = false;
    radio->_trainACKWait = false;
  }
}

void TeachRXWait::update(RTCM_Link* radio) {
  //Wait for ping. Once received, transmit training data
  if (radio->_transactionComplete == true) {//If dio0ISR has fired, a packet has arrived
    radio->_transactionComplete = false; //Reset ISR flag
    radio->changeState(TeachRXPacket::getInstance());
  } 
  else if (radio->_trainACKWait && radio->_ackDelay > radio->_trainSettings->airTime(2)) {
    radio->_trainACKWait = false;
  }
  else if (radio->_trainTime > 120000) {
    radio->configure();
    radio->changeState(RXStandby::getInstance());
  }
}

RadioStateBase& TeachRXWait::getInstance() {
	static TeachRXWait singleton;
	return singleton;
}

void TeachRXPacket::update(RTCM_Link* radio) {
  PacketType packetType = radio->identifyPacketType(); //Look at the packet we just received

  if (packetType == PacketType::TRAINING_PING) {
    radio->changeState(TeachTX::getInstance()); //Someone is pinging us, send training data back
  }
  else if (packetType == PacketType::TRAINING_ACK && radio->_trainACKWait) {
    radio->_trainEnd = true;
    radio->changeState(RXStandby::getInstance());
  }
  //During training, only training packets are valid
  else {//PACKET_BAD, PACLET_ACK, PACKET_NETID_MISMATCH, PACKET_PING, PACKET_DATA
     radio->changeState(TeachRXWait::getInstance());
  }
}

void TeachRXPacket::exit(RTCM_Link* radio) {
  if( radio->_trainEnd) {
    radio->configure(); //Setup radio with settings
    radio->_trainConfig = false;
    radio->_trainEnd = false;
  }
}

RadioStateBase& TeachRXPacket::getInstance() {
	static TeachRXPacket singleton;
	return singleton;
}

void TeachTX::update(RTCM_Link* radio) {
  if (radio->_transactionComplete == true) {//If dio0ISR has fired, we are done transmitting 
    radio->_transactionComplete = false; //Reset ISR flag
    radio->_trainACKWait = true;
    radio->changeState(TeachRXWait::getInstance());
  }
}

RadioStateBase& TeachTX::getInstance() {
  static TeachTX singleton;
  return singleton;
}
