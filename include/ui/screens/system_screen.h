#ifndef _SYSTEM_SCREEN_H_
#define _SYSTEM_SCREEN_H_

#include <Arduino.h>
#include <lvgl.h>
#include "screen.h"

class ScreenManager;


class SystemScreen : public Screen {
 public:
  void enter(ScreenManager* screenManager);
  void update(ScreenManager* screenManager);
  void exit(ScreenManager* screenManager);
  static Screen& getInstance();
 
 private:
  SystemScreen() {}
  SystemScreen(const SystemScreen& other);
  SystemScreen& operator=(const SystemScreen& other);
};

#endif