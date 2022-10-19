#ifndef _MENU_SCREEN_H_
#define _MENU_SCREEN_H_

#include <Arduino.h>
#include <lvgl.h>
#include "screen.h"

class ScreenManager;

class MenuScreen : public Screen {
public:
  void enter(ScreenManager* screenManager);
  void update(ScreenManager* screenManager);
  void exit(ScreenManager* screenManager);
  static Screen& getInstance();
 
private:
  MenuScreen() {}
  MenuScreen(const MenuScreen& other);
  MenuScreen& operator=(const MenuScreen& other);
};

#endif