#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <Arduino.h>

struct systemCalibration {
  float touchCal[6] = { 1.0, 0.0, 0.0, 0.0, 1.0, 0.0 };   // Touchscreen calibration
};

void beginWDT();
void delayWDT(uint16_t delayAmount);
void petWDT();
void beginFileSystem();

void systemReset();

void beginRTCMLink();
#endif