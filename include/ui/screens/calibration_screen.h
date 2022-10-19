#ifndef _CALIBRATION_SCREEN_H_
#define _CALIBRATION_SCREEN_H_

#include <Arduino.h>
#include <lvgl.h>
#include <ILI9341_T4.h>
#include "screen.h"
#include "settings.h"

extern ILI9341_T4::ILI9341Driver tft;
extern systemCal calibration;

class ScreenManager;

class CalibrationScreen : public Screen {
 public:
  void enter(ScreenManager* screenManager);
  void update(ScreenManager* screenManager);
  void exit(ScreenManager* screenManager);
  static Screen& getInstance();
  void tpcalCreate();

 private:
  CalibrationScreen() {}
  CalibrationScreen(const CalibrationScreen& other);
  CalibrationScreen& operator=(const CalibrationScreen& other); 
};

void addCircle(int centerX, int centerY);
void nextStep(int x, int y, lv_obj_t* page);
void inputPolling(lv_timer_t * timer);

#endif