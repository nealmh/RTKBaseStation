#include "rtcm_link.h"


//Return number of bytes sitting in the data transmit buffer
uint16_t RTCM_Link::availableTXBytes() {
  if (_txHead >= _txTail) {
    return (_txHead - _txTail);
  }
  return (sizeof(_txBuffer) - _txTail + _txHead);
}

void RTCM_Link::bufferTXByte(uint8_t data) {
    _txBuffer[_txHead++] = data;
}

//Return true if there is a full frame of data in the buffer, or the timeout has been exceded
bool RTCM_Link::checkTX() {
  if (availableTXBytes()) {
    //Push any available data out
    if (availableTXBytes() >= _settings->frameSize()) {
      return (true);
    }

    //Check if we should send out a partial frame
    else if (availableTXBytes() > 0 && _lastByteReceived_ms >= _settings->timeoutBeforeSendingFrame_ms()) {
      return (true);
    }
  }
  return (false);
}

//Push the outgoing packet to the air
void RTCM_Link::_sendPacket() {
  //Attach netID and control byte to end of packet
  _outgoingPacket[_packetSize - 2] = _settings->netID();
  memcpy(&_outgoingPacket[_packetSize - 1], & _TXTrailer, 1);

  //If we are trainsmitting at high data rates the receiver is often not ready for new data. Pause for a few ms (measured with logic analyzer).
  if (_settings->airSpeed() == 28800 || _settings->airSpeed() == 38400)
    delay(2);

  _radio.setFrequency(_settings->getChannel(_radio.getFHSSChannel())); //Return home before every transmission
  
  int state = _radio.startTransmit(_outgoingPacket, _packetSize);
  if (state == RADIOLIB_ERR_NONE) {
    if (_timeToHop) 
      _hopChannel();
  }
}