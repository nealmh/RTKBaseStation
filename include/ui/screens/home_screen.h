#ifndef _HOME_SCREEN_H_
#define _HOME_SCREEN_H_

#include <Arduino.h>
#include <lvgl.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include "screen.h"

extern SFE_UBLOX_GNSS zedf9p;

class ScreenManager;

class HomeScreen : public Screen {
public:
  void enter(ScreenManager* screenManager);
  void update(ScreenManager* screenManager);
  void exit(ScreenManager* screenManager);
  static Screen& getInstance();
 
private:
  HomeScreen() {}
  HomeScreen(const HomeScreen& other);
  HomeScreen& operator=(const HomeScreen& other);

  lv_obj_t* _latVal;
  lv_obj_t* _lonVal;
  lv_obj_t* _accVal;
  
  elapsedMillis _lastUpdate = 0;
};

#endif