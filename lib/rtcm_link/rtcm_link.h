//The RTCM_Link class is the main interface to the radio for transmitting and receiving the GNSS correction
//data stream. It manages the radio hardware, as well as providing buffers for tx/rx data. It does not 
//directly manage the settings, but includes a reference to the current settings (RTCMSettings) object so
//the radio can be correctly configured.

#ifndef _RTCM_LINK_H_
#define _RTCM_LINK_H_

#include <Arduino.h>
#include <RadioLib.h>
#include <SPI.h>

#include "rtcm_link_settings.h"
#include "states/state_base.h"
#include "rtcm_link_types.h"

class RadioStateBase;

class RTCM_Link {
public:
  RTCM_Link();
  void begin(RTCMSettings *settings, uint8_t pin_txen, uint8_t pin_rxen, uint8_t pin_cs, uint8_t pin_dio0, uint8_t pin_dio1, uint8_t pin_rst);
  void attachPetWDT(void(*pet_func)());
  void attachDelayWDT(void(*delayWDT_func)(uint16_t));
  void configure();
  void configureTraining();

  void changeState(RadioStateBase& newState);
  bool receiveInProcess();
  bool checkTX();
  uint16_t availableTXBytes();
  PacketType identifyPacketType();
  
  void bufferTXByte(uint8_t data);

  void update();

  //Prototype ISRs and methods to connect them to the RTCM_Link
  void RxTxDoneISR(void);
  void HopISR(void);
  void setDIO0ISR(void (*func)());
  void setDIO1ISR(void (*func)());



  float   getRSSI();
  uint8_t randomByte();
  uint8_t getFHSSChannel();
  uint8_t covertdBmToSetting(uint8_t power);
  uint8_t convertSettingTodBm(uint8_t setting);
  
private:
  //Radio hardware and settings references
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  SX1276 _radio = NULL;
  RTCMSettings* _settings = nullptr;
  RTCMSettings* _trainSettings = nullptr;
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  
  //Watchdog timer interface functions
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  void (*_petWDT)();
  void (*_delayWDT)(uint16_t delayAmount);
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

  //Interface connections to the radio module
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  uint8_t _pin_txen;
  uint8_t _pin_rxen;
  uint8_t _pin_cs;
  uint8_t _pin_dio0;
  uint8_t _pin_rst;
  uint8_t _pin_dio1;
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

  bool _transactionComplete;
  bool _timeToHop;
  float _frequencyCorrection;
  void _returnToRX();
  void _hopChannel();
  void _sendPacket();
  
  //Training status
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  bool _trainConfig;
  bool _trainEnd;
  bool _trainACKWait;
  elapsedMillis _trainTime = 0; //Stores when a training mode was entered for timeout purposes
  elapsedMillis _learnCycle = 0; //Timer for sending the next training ping
  elapsedMillis _ackDelay = 0; //Timer to wait for an ACK packet
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  
  //Radio Data buffers
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  ControlTrailer _RXTrailer;
  ControlTrailer _TXTrailer;

  //Hold the last packet received
  uint8_t _lastPacket[MAX_PACKET_SIZE];
  uint8_t _lastPacketSize;

  //Hold the next packet to send
  uint8_t _outgoingPacket[MAX_PACKET_SIZE]; //Contains the current data in route to receiver
  uint8_t _packetSize; //Tracks how much data + control trailer

  uint8_t _txBuffer[1024 * 4]; //Bytes waiting to be transmitted. Buffer up to 1s of bytes at 4k
  uint8_t _rxBuffer[1024 * 4]; //Bytes received from radio. Buffer up to 1s of bytes at 4k
  uint16_t _txHead;
  uint16_t _txTail;
  uint16_t _rxHead;
  uint16_t _rxTail;
  elapsedMillis _lastByteReceived_ms; //Track when last transmission was. Send partial buffer once time has expired.
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

  //Statemachine states
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  RadioStateBase* _state;

  friend class RXStandby;
  friend class RXPacket;
  friend class TX;

  friend class TeachRXWait;
  friend class TeachRXPacket;
  friend class TeachTX;

  friend class LearnDataWait;
  friend class LearnRXPacket;
  friend class LearnTX;

  	
  void (*_tx_entry_cb)();
  void (*_tx_exit_cb)();
  void (*_rx_entry_cb)();
  void (*_rx_exit_cb)();

  void (*_teach_tx_entry_cb)();
  void (*_teach_tx_exit_cb)();
  void (*_teach_rx_entry_cb)();
  void (*_teach_rx_exit_cb)();

  void (*_learn_tx_entry_cb)();
  void (*_learn_tx_exit_cb)();
  void (*_learn_rx_entry_cb)();
  void (*_learn_rx_exit_cb)();
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
};

#endif