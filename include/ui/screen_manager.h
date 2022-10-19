#ifndef _SCREEN_MANAGER_H_
#define _SCREEN_MANAGER_H_

#include <lvgl.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include "screens/screen.h"
#include "status_bar.h"

extern SFE_UBLOX_GNSS zedf9p;

class Screen;

class ScreenManager {
public:
  void setup();
  void update();

  void changeScreen(Screen& newScreen);
  Screen* currentScreen() {return _screen; }

  void setPageLabel(const char* label);
  void setRXLed(bool status);
  void setTXLed(bool status);

protected:
  

private:
  Screen* _screen;
  StatusBar _statusBar;
};

#endif