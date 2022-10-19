#ifndef _RTCM_LINK_SETTINGS_H_
#define _RTCM_LINK_SETTINGS_H_

#include <Arduino.h>
#include <Array.h>

const float FREQUENCY_MIN = 902.0;
const float FREQUENCY_MAX = 928.0;
const uint8_t NUMBER_OF_CHANNELS = 50;
const uint16_t MAX_DWELL_TIME = 400; 
const uint8_t RADIO_SYNC_WORD = 0x12; 
#define MAX_PACKET_SIZE 255 //Limited by SX127x
const int FIRMWARE_VERSION_MAJOR = 1;
const int FIRMWARE_VERSION_MINOR = 0;
#define FIRMWARE_VERSION (FIRMWARE_VERSION_MAJOR * 0x10 + FIRMWARE_VERSION_MINOR)

class RTCMSettings {
public:
  RTCMSettings();
  void defaultSettings();
  void trainSettings();
  void generateHopTable();

  bool     dirty()  {return _dirty; }
  void     dirty(bool status)  {_dirty = status; }

  uint16_t sizeOfSettings() { return _sizeOfSettings; }
  uint16_t strIdentifier() { return _strIdentifier; }
  
  uint8_t  netID() { return _netID; }
  void     netID(uint8_t netID);

  uint16_t broadcastPower_dbm() { return _broadcastPower_dbm; }
  void     broadcastPower_dbm(uint16_t power);
  
  uint32_t airSpeed() { return _airSpeed; }
  void     airSpeed(uint32_t airSpeed);

  float    bandwidth() { return _bandwidth; }
  void     bandwidth(float bandwidth);

  uint8_t  spreadFactor() { return _spreadFactor; }
  void     spreadFactor(uint8_t spreadFactor);

  uint8_t  codingRate() { return _codingRate; }
  void     codingRate(uint8_t codingRate);

  uint8_t  preambleLength() { return _preambleLength; }
  void     preambleLength(uint8_t preambleLength);

  uint16_t timeoutBeforeSendingFrame_ms() { return _timeoutBeforeSendingFrame_ms; }
  uint8_t  responseDelayDivisor() { return _responseDelayDivisor; }
  bool     frequencyHop() { return _frequencyHop; }
  uint8_t  syncWord() { return _syncWord; }
  uint8_t  frameSize() { return _frameSize; }
  float    frequencyMin() { return _frequencyMin; }
  float    frequencyMax() { return _frequencyMax; }
  uint8_t  numberOfChannels() { return _numberOfChannels; }
  uint16_t maxDwellTime() { return _maxDwellTime; }


  //values computed from the settings
  uint8_t  trainDataPacketSize();
  float    symbolTime();
  uint16_t airTime(uint8_t bytesToSend);
  uint16_t maxThroughput();
  uint8_t  hoppingPeriod();
  float    getChannel(uint8_t number) { return _channels[number]; }

private:
  bool     _dirty; //indicates if the settings are changed

  //These are used to confirm that the settings are valid for the firmware revision
  uint16_t _sizeOfSettings; //sizeOfSettings **must** be the first entry and must be int
  uint16_t _strIdentifier; // strIdentifier **must** be the second entry
  
  //Network link configuration
  uint8_t  _netID; //Both radios must share a network ID
  uint16_t _broadcastPower_dbm; //Transmit power in dBm. Max is 30dBm (1W), min is 14dBm (25mW).
    
  //Data rate settings for the radio
  uint32_t _airSpeed; //Default to ~523 bytes per second to support RTCM. Overrides spread, bandwidth, and coding
  float    _bandwidth; //kHz 125/250/500 generally. We need 500kHz for higher data.
  uint8_t  _spreadFactor; //6 to 12. Use higher factor for longer range.
  uint8_t  _codingRate; //5 to 8. 6 was chosen to allow higher spread factor. Higher coding rates ensure less packets dropped.
  uint8_t  _preambleLength; //Number of symbols. Different lengths does *not* guarantee a remote radio privacy. 8 to 11 works. 8 to 15 drops some. 8 to 20 is silent.

 //These settings can technically be changed, but the end user typically should not alter them
  uint16_t _timeoutBeforeSendingFrame_ms; //Send partial buffer if time expires
  uint8_t  _responseDelayDivisor; //Add on to max response time after packet has been sent. Factor of 2. 8 is ok. 4 is good. A smaller number increases the delay.
  bool     _frequencyHop; //Hop between frequencies to avoid dwelling on any one channel for too long
  uint8_t  _syncWord; //18 = 0x12 is default for custom/private networks. Different sync words does *not* guarantee a remote radio will not get packet.
  uint8_t  _frameSize; //Send batches of bytes through LoRa link, max (255 - control trailer) = 253.
  float    _frequencyMin; //MHz
  float    _frequencyMax; //MHz
  uint8_t  _numberOfChannels; //Divide the min/max freq band into this number of channels and hop between.
  uint16_t _maxDwellTime; //Max number of ms before hopping (if enabled). Useful for configuring radio to be within regulator limits (FCC = 400ms max)

  float *_channels; // CHannel frequencies for hopping

  //Encryption
  uint16_t _seed;

  //Helper functions
  void _shuffleChannels();
  uint16_t _lfsrRand();
};

#endif