//Define the enums and structs for the radio control system
#ifndef _RTCM_LINK_TYPES_H_
#define _RTCM_LINK_TYPES_H_

#include <Arduino.h>

//Possible types of packets received
enum class PacketType {
  BAD = 0,
  NETID_MISMATCH,
  DATA,
  PING,
  ACK,

  TRAINING_PING,
  TRAINING_DATA,
  TRAINING_ACK
};

//Bit field to indicate what type of packet we are dealing with
struct ControlTrailer {
  uint8_t ping : 1;
  uint8_t ack : 1;
  uint8_t train : 1;
  uint8_t filler : 5;
};


#endif