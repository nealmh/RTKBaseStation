#include "rtcm_link.h"

//Returns true if the radio indicates we have an ongoing reception
//Bit 0: Signal Detected indicates that a valid LoRa preamble has been detected
//Bit 1: Signal Synchronized indicates that the end of Preamble has been detected, the modem is in lock
//Bit 3: Header Info Valid toggles high when a valid Header (with correct CRC) is detected
bool RTCM_Link::receiveInProcess() {
  uint8_t radioStatus = _radio.getModemStatus();
  if ((radioStatus & 0b1) == 1) return (true); //If bit 0 is set, forget the other bits, there is a receive in progress
  return (false); //No receive in process

  //If bit 0 is cleared, there is likely no receive in progress, but we need to confirm it
  //The radio will clear this bit before the radio triggers the receiveComplete ISR so we often have a race condition.

  //If we are using FHSS then check channel freq. This is reset to 0 upon receive completion.
  //If radio has jumped back to channel 0 then we can confirm a transaction is complete.
  if (_settings->frequencyHop() == true)   {
    if (_radio.getFHSSChannel() == 0)
      return (false); //Receive not in process

    if (_transactionComplete == false)
      return (true); //Receive still in process
  }
  else {
    //If we're not using FHSS, use small delay.
    delay(5); //Small wait before checking RX complete ISR again
    if (_transactionComplete == false)
      return (true); //Receive still in process
  }

  return (false); //No receive in process
}

//Determine the type of packet received and print data as necessary
//The LoRa radio handles CRC and FHSS for us so we presume the packet was meant for us
//We check the packet netID for validity
//Just return the packet type if the packet is a short/ping packet, ack
//Move any data to incomingPacket buffer
PacketType RTCM_Link::identifyPacketType() {
  uint8_t incomingBuffer[MAX_PACKET_SIZE];
  _radio.readData(incomingBuffer, MAX_PACKET_SIZE);
  uint8_t receivedBytes = _radio.getPacketLength();

  if (receivedBytes < 2) {
    return (PacketType::BAD);
  }

  //Pull out control header
  uint8_t receivedNetID = incomingBuffer[receivedBytes - 2];
  memcpy(&_RXTrailer, &incomingBuffer[receivedBytes - 1], 1);

  //SF6 requires an implicit header which means there is no dataLength in the header
  //Instead, we manually store it 3 bytes from the end (before NetID)
  if (_settings->spreadFactor() == 6) {
    //We've either received a control packet (2 bytes) or a data packet
    if (receivedBytes > 2) {
      receivedBytes = incomingBuffer[receivedBytes - 3]; //Obtain actual packet data length
      receivedBytes -= 1; //Remove the manual packetSize byte from consideration
    }
  }

  receivedBytes -= 2; //Remove control bytes

  
  if (receivedNetID != _settings->netID()) {
    return (PacketType::NETID_MISMATCH);
  }

  //We have empty data packet, this is a control packet used for pinging/scanning
  if (_RXTrailer.ping == 1) {
    //If this packet is marked as training data, someone is sending training ping
    if (_RXTrailer.train == 1) {
      return (PacketType::TRAINING_PING);
    }
    else {
      return (PacketType::PING);
    }
  }

  //Update lastPacket details with current packet
  memcpy(_lastPacket, incomingBuffer, receivedBytes);
  _lastPacketSize = receivedBytes;

  //If this packet is marked as training data,
  //payload contains new AES key and netID which will be processed externally
  if (_RXTrailer.train == 1) {
    return (PacketType::TRAINING_DATA);
  }

  return (PacketType::DATA);
}