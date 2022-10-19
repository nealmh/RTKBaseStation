#include "rtcm_link_settings.h"

RTCMSettings::RTCMSettings(){
  defaultSettings();
}

void RTCMSettings::defaultSettings() {
  //Settings version and size information for validation
  _sizeOfSettings = 0;
  _strIdentifier = FIRMWARE_VERSION;
  
  //Network link configuration
  _netID = 192; //Both radios must share a network ID
  _broadcastPower_dbm = 30; //Transmit power in dBm. Max is 30dBm (1W), min is 14dBm (25mW).
    
  //Data rate settings for the radio
  _airSpeed = 4800; //Default to ~523 bytes per second to support RTCM. Overrides spread, bandwidth, and coding
  _bandwidth = 500.0; //kHz 125/250/500 generally. We need 500kHz for higher data.
  _spreadFactor = 9; //6 to 12. Use higher factor for longer range.
  _codingRate = 6; //5 to 8. 6 was chosen to allow higher spread factor. Higher coding rates ensure less packets dropped.
  _preambleLength = 8; //Number of symbols. Different lengths does *not* guarantee a remote radio privacy. 8 to 11 works. 8 to 15 drops some. 8 to 20 is silent.

 //These settings can technically be changed, but the end user typically should not alter them
  _timeoutBeforeSendingFrame_ms = 50; //Send partial buffer if time expires
  _responseDelayDivisor = 4; //Add on to max response time after packet has been sent. Factor of 2. 8 is ok. 4 is good. A smaller number increases the delay.
  _frequencyHop = true; //Hop between frequencies to avoid dwelling on any one channel for too long
  _syncWord = RADIO_SYNC_WORD; //18 = 0x12 is default for custom/private networks. Different sync words does *not* guarantee a remote radio will not get packet.
  _frameSize = MAX_PACKET_SIZE - 2; //Send batches of bytes through LoRa link, max (255 - control trailer) = 253.
  _frequencyMin = FREQUENCY_MIN; //MHz
  _frequencyMax = FREQUENCY_MAX; //MHz
  _numberOfChannels = NUMBER_OF_CHANNELS; //Divide the min/max freq band into this number of channels and hop between.
  _maxDwellTime = MAX_DWELL_TIME; //Max number of ms before hopping (if enabled). Useful for configuring radio to be within regulator limits (FCC = 400ms max)
}

void RTCMSettings::trainSettings() {
  defaultSettings();
  _frequencyHop = false;
  _netID = 0;
  _broadcastPower_dbm = 14;
  float channelSpacing = (_frequencyMax - _frequencyMin) / (float)(_numberOfChannels + 2);
  float trainFrequency = _frequencyMin + (channelSpacing * (FIRMWARE_VERSION_MAJOR % _numberOfChannels));

  _channels[0] = trainFrequency; //Inject this frequency into the channel table
}

void RTCMSettings::netID(uint8_t netID) {
  if (netID >= 0 && netID <= 255) {
    if (_netID != netID) {
      _dirty = true;
    }
    _netID = netID;
  }
}

void RTCMSettings::broadcastPower_dbm(uint16_t power){
  if (power >= 14 && power <= 30) {
    _broadcastPower_dbm = power;
  }
}

void RTCMSettings::airSpeed(uint32_t airSpeed) {
  if (airSpeed == 0
    || airSpeed == 40
    || airSpeed == 150
    || airSpeed == 400
    || airSpeed == 1200
    || airSpeed == 2400
    || airSpeed == 4800
    || airSpeed == 9600
    || airSpeed == 19200
    || airSpeed == 28800
    || airSpeed == 38400
    ) {
      if (_airSpeed != airSpeed) {
        _dirty = true;
      }
      _airSpeed = airSpeed;

      //Determine if we are using AirSpeed or custom settings
      switch (_airSpeed) {
        case (0):
          //Custom settings - use settings without modification
          break;
        case (40):
          _spreadFactor = 11;
          _bandwidth = 62.5;
          _codingRate = 8;
          break;
        case (150):
          _spreadFactor = 10;
          _bandwidth = 62.5;
          _codingRate = 8;
          break;
        case (400):
          _spreadFactor = 10;
          _bandwidth = 125;
          _codingRate = 8;
          break;
        case (1200):
          _spreadFactor = 9;
          _bandwidth = 125;
          _codingRate = 8;
          break;
        case (2400):
          _spreadFactor = 10;
          _bandwidth = 500;
          _codingRate = 8;
          break;
        case (4800):
          _spreadFactor = 9;
          _bandwidth = 500;
          _codingRate = 8;
          break;
        case (9600):
          _spreadFactor = 8;
          _bandwidth = 500;
          _codingRate = 7;
          break;
        case (19200):
          _spreadFactor = 7;
          _bandwidth = 500;
          _codingRate = 7;
          break;
        case (28800):
          _spreadFactor = 6;
          _bandwidth = 500;
          _codingRate = 6;
          break;
        case (38400):
          _spreadFactor = 6;
          _bandwidth = 500;
          _codingRate = 5;
          break;
        default:
          break;
    }
  }
}
void RTCMSettings::bandwidth(float bandwidth){
  if ( (bandwidth * 100 == 1040)
    || bandwidth == 15.6
    || (bandwidth * 100) == 2080
    || bandwidth == 31.25
    || bandwidth == 41.7
    || bandwidth == 62.5
    || bandwidth == 125.0
    || bandwidth == 250.0
    || bandwidth == 500.0
  ) {
    if (_bandwidth != bandwidth && _airSpeed != 0) {
      _airSpeed = 0;
    }
    if (_bandwidth != bandwidth) {
      _dirty = true;
    }
    _bandwidth = bandwidth;
  }
}
void RTCMSettings::spreadFactor(uint8_t spreadFactor) {
  if (spreadFactor >= 6 && spreadFactor <= 12) {
    if (_spreadFactor != spreadFactor && _airSpeed != 0) {
      _airSpeed = 0;
    }
    if (_spreadFactor != spreadFactor) {
      _dirty = true;
    }
    _spreadFactor = spreadFactor;
  }
}

void RTCMSettings::codingRate(uint8_t codingRate){
  if (codingRate >= 5 && codingRate <= 8) {
    if (_codingRate != codingRate && _airSpeed != 0) {
      _airSpeed = 0;
    }
    if (_codingRate != codingRate) {
      _dirty = true;
    }
    _codingRate = codingRate;
  }
}

void RTCMSettings::preambleLength(uint8_t preambleLength){
  if ( preambleLength >= 6 && preambleLength <= 65535) {
    if (_preambleLength != preambleLength) {
      _dirty = true;
    }
    _preambleLength = preambleLength;
  }
}

uint8_t RTCMSettings::trainDataPacketSize(){
  return sizeof(_netID) + sizeof(_airSpeed) + sizeof(_bandwidth) + sizeof(_spreadFactor) + sizeof(_codingRate) + sizeof(_preambleLength);
}

float RTCMSettings::symbolTime() {
  return pow(2, _spreadFactor) / _bandwidth;
}

//Given spread factor, bandwidth, coding rate and number of bytes, return total Air Time in ms for packet
uint16_t RTCMSettings::airTime(uint8_t bytesToSend) {
  float tSym = symbolTime();
  float tPreamble = (_preambleLength + 4.25) * tSym;
  float p1 = (8 * bytesToSend - 4 * _spreadFactor + 28 + 16 * 1 - 20 * 0) / (4.0 * (_spreadFactor - 2 * 0));
  p1 = ceil(p1) * _codingRate;
  if (p1 < 0) p1 = 0;
  uint16_t payloadBytes = 8 + p1;
  float tPayload = payloadBytes * tSym;
  float tPacket = tPreamble + tPayload;

  return ((uint16_t)ceil(tPacket));
}

//Given spread factor, bandwidth, coding rate and frame size, return most bytes we can push per second
uint16_t RTCMSettings::maxThroughput() {
  uint8_t mostFramesPerSecond = 1000 / airTime(_frameSize);
  uint16_t mostBytesPerSecond = _frameSize * mostFramesPerSecond;

  return (mostBytesPerSecond);
}

// HoppingPeriod = Tsym * FreqHoppingPeriod
// Given defaults of spreadfactor = 9, bandwidth = 125, it follows Tsym = 4.10ms
// HoppingPeriod = 4.10 * x = Yms. Can be as high as 400ms to be within regulatory limits
uint8_t RTCMSettings::hoppingPeriod() {
  uint16_t hoppingPeriod = _maxDwellTime / symbolTime(); //Limit FHSS dwell time to 400ms max. / automatically floors number
  if (hoppingPeriod > 255) 
    hoppingPeriod = 255; //Limit to 8 bits.
  if (_frequencyHop == false)
    hoppingPeriod = 0;
  return hoppingPeriod;
}

//Generate unique hop table based on radio settings
void RTCMSettings::generateHopTable() {
  if (_channels != NULL)
    free(_channels);
  _channels = (float *)malloc(_numberOfChannels * sizeof(float));

  float channelSpacing = (_frequencyMax - _frequencyMin) / (float)(_numberOfChannels + 2);

  //Keep away from edge of available spectrum
  float operatingMinFreq = _frequencyMin + (channelSpacing / 2);

  //Pre populate channel list
  for (int x = 0 ; x < _numberOfChannels; x++)
    _channels[x] = operatingMinFreq + (x * channelSpacing);

  //Feed random number generator with our specific platform settings
  //Use settings that must be identical to have a functioning link.
  //For example, we do not use coding rate because two radios can communicate with different coding rate values
  _seed = _airSpeed + _netID + (uint16_t)_frequencyMin + (uint16_t)_frequencyMax
               + _numberOfChannels + _frequencyHop + _maxDwellTime
               + (uint16_t)_bandwidth + _spreadFactor;
 
  //'Randomly' shuffle list based on our specific seed
  _shuffleChannels();

}


//Array shuffle from http://benpfaff.org/writings/clc/shuffle.html
void RTCMSettings::_shuffleChannels() {
  for (uint8_t i = 0; i < _numberOfChannels - 1; i++) {
    uint8_t j = _lfsrRand() % _numberOfChannels;
    float t = _channels[j];
    _channels[j] = _channels[i];
    _channels[i] = t;
  }
}

//Simple lfsr randomizer. Needed because we cannot guarantee how random()
//will respond on different Arduino platforms. ie Uno acts diffrently from ESP32.
//We don't need 'truly random', we need repeatable across platforms.
uint16_t RTCMSettings::_lfsrRand() {
static bool myRandBit;
  myRandBit = ((_seed >> 0) ^ (_seed >> 2) ^ (_seed >> 3) ^ (_seed >> 5) ) & 1;
  return _seed = (_seed >> 1) | (myRandBit << 15);
}