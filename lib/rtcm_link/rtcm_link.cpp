#include <RadioLib.h>

#include "rtcm_link.h"
#include "states/base_states.h"


RTCM_Link::RTCM_Link() {
}

void RTCM_Link::attachPetWDT(void(*pet_func)()){
  _petWDT = pet_func;
}

void RTCM_Link::attachDelayWDT(void(*delayWDT_func)(uint16_t delayAmount)){
  _delayWDT = delayWDT_func;
}


void RTCM_Link::begin(RTCMSettings *settings, uint8_t pin_txen, uint8_t pin_rxen, uint8_t pin_cs, uint8_t pin_dio0, uint8_t pin_dio1, uint8_t pin_rst){
  _settings = settings;
  Serial.println("Settings attached");
  _pin_txen = pin_txen;
  _pin_rxen = pin_rxen;
  _pin_cs = pin_cs;
  _pin_dio0 = pin_dio0;
  _pin_dio1 = pin_dio1;
  _pin_rst = pin_rst;
  Serial.println("Pins attached");

  _radio = new Module(_pin_cs, _pin_dio0, _pin_rst, _pin_dio1);
  Serial.println("Radio Module attached");

  _frequencyCorrection = 0;
  _trainConfig = false;
  _transactionComplete = false;
  _timeToHop = false;

  float centerFreq = (_settings->frequencyMax() - _settings->frequencyMin()) / 2;
  centerFreq += _settings->frequencyMin();

  int state = _radio.begin(centerFreq); //Doesn't matter what freq we start at
  if (state != RADIOLIB_ERR_NONE) {
    Serial.println("Error Initalizing Radio Module");
    while (1) {
      _petWDT();
      delay(100);
    }
  } else {
    Serial.println("Radio module initialized");
  }
   
  _settings->generateHopTable(); //Generate frequency table based on randomByte
  configure(); //Generate freq table, setup radio, go to receiving, change state to standby
  changeState(RXStandby::getInstance());
}

//Apply settings to radio
//Called after begin() and once user exits from command interface
void RTCM_Link::configure() {
  _radio.setFrequency(_settings->getChannel(0));

  //The SX1276 and RadioLib accepts a value of 2 to 17, with 20 enabling the power amplifier
  //Measuring actual power output the radio will output 14dBm (25mW) to 27.9dBm (617mW) in constant transmission
  //So we use a lookup to convert between the user's chosen power and the radio setting
  int radioPowerSetting = covertdBmToSetting(_settings->broadcastPower_dbm());
  _radio.setOutputPower(radioPowerSetting);
  _radio.setBandwidth(_settings->bandwidth());
  _radio.setSpreadingFactor(_settings->spreadFactor());
  _radio.setCodingRate(_settings->codingRate());
  _radio.setSyncWord(_settings->syncWord());
  _radio.setPreambleLength(_settings->preambleLength());

  //SF6 requires an implicit header. We will transmit 255 bytes for most packets and 2 bytes for ACK packets.
  if (_settings->spreadFactor() == 6) {
    _radio.implicitHeader(255);
  }
  else {
    _radio.explicitHeader();
  }

  _radio.setRfSwitchPins(_pin_rxen, _pin_txen);

  _radio.setFHSSHoppingPeriod(_settings->hoppingPeriod());
  _trainConfig = false;
}


void RTCM_Link::changeState(RadioStateBase& newState){
	_state->exit(this);
	_state = &newState;
	_state->enter(this);
}

void RTCM_Link::_returnToRX() {
  _radio.setFrequency(_settings->getChannel(_radio.getFHSSChannel()));

  _timeToHop = false;

  if (_settings->spreadFactor() > 6) {
    _radio.startReceive();
  }
  else {
    _radio.implicitHeader(255);
    _radio.startReceive(255); //Expect a full data packet
  }
}

//Change to known training frequency based on available freq and current major firmware version
//This will allow different minor versions to continue to train to each other
void RTCM_Link::configureTraining()
{
  //During training use default radio settings. This ensures both radios are at known good settings.
  if (_trainSettings != nullptr) {
    delete _trainSettings;
  }
  _trainSettings = new RTCMSettings();
  _trainSettings->generateHopTable(); //Generate frequency table based on current settings

  configure(); //Setup radio with settings
  _trainConfig = true;
}

//Periodic update of the radio subsystem
void RTCM_Link::update() {
  _state->update(this);
}

//Move to the next channel
//This is called when the FHSS interrupt is received
//at the beginning and during of a transmission or reception
void RTCM_Link::_hopChannel() {
  _radio.clearFHSSInt();
  _timeToHop = false;
  _radio.setFrequency(_settings->getChannel(_radio.getFHSSChannel()) - _frequencyCorrection);
}


//Convert the user's requested dBm to what the radio should be set to, to hit that power level
//3 is lowest allowed setting using SX1276+RadioLib
uint8_t RTCM_Link::covertdBmToSetting(uint8_t power) {
  switch (power) {
    case 14: return (2); break;
    case 15: return (3); break;
    case 16: return (4); break;
    case 17: return (5); break;
    case 18: return (6); break;
    case 19: return (7); break;
    case 20: return (7); break;
    case 21: return (8); break;
    case 22: return (9); break;
    case 23: return (10); break;
    case 24: return (11); break;
    case 25: return (12); break;
    case 26: return (13); break;
    case 27: return (20); break;
    case 28: return (20); break;
    case 29: return (20); break;
    case 30: return (20); break;
    default: return (3); break;
  }
}



//Convert the user's requested dBm to what the radio should be set to, to hit that power level
//3 is lowest allowed setting using SX1276+RadioLib
uint8_t RTCM_Link::convertSettingTodBm(uint8_t setting) {
  switch (setting) {
    case 2: return (14); break;
    case 3: return (15); break;
    case 4: return (16); break;
    case 5: return (17); break;
    case 6: return (18); break;
    case 7: return (19); break;
    case 8: return (21); break;
    case 9: return (22); break;
    case 10: return (23); break;
    case 11: return (24); break;
    case 12: return (25); break;
    case 13: return (26); break;
    case 20: return (30); break;
    default: return (3); break;
  }
}

void RTCM_Link::setDIO0ISR(void (*func)()){
  _radio.setDio0Action(func);
}

void RTCM_Link::setDIO1ISR(void (*func)()){
  _radio.setDio1Action(func);
}

//ISR when DIO0 goes low
//Called when transmission is complete or when RX is received
void RTCM_Link::RxTxDoneISR(void) {
  _transactionComplete = true;
}

//ISR when DIO1 goes low
//Called when FhssChangeChannel interrupt occurs (at regular HoppingPeriods)
void RTCM_Link::HopISR(void) {
  _timeToHop = true;
}