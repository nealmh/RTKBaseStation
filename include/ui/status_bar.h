#ifndef _STATUS_BAR_H_
#define _STATUS_BAR_H_

#include <Arduino.h>
#include <lvgl.h>
#include "widgets/indicator_widgets.h"

class ScreenManager;

class StatusBar {
public:
  void init(ScreenManager* screenManager);

  
  void setGNSSStatus(byte fixType);
  void setRXLed(bool status);
  void setTXLed(bool status);
  void setPageLabel(const char *text);

protected:
  ScreenManager* screenManager_;
  lv_obj_t* bar;
  Indicator txLED;
  Indicator rxLED;
  Indicator gpsInd;
  lv_obj_t* pageLabel;
  lv_obj_t* menuButton;
  lv_obj_t* menuLabel;

private:
  
};

#endif