#ifndef _TEACH_SCREEN_H_
#define _TEACH_SCREEN_H_

#include <Arduino.h>
#include <lvgl.h>
#include "screen.h"

class ScreenManager;

class TeachScreen : public Screen {
 public:
  void enter(ScreenManager* screenManager);
  void update(ScreenManager* screenManager);
  void exit(ScreenManager* screenManager);
  static Screen& getInstance();

 private:
  TeachScreen() {}
  TeachScreen(const TeachScreen& other);
  TeachScreen& operator=(const TeachScreen& other);

  lv_obj_t* spinner;
  lv_obj_t* teachStatusText;  
};

#endif