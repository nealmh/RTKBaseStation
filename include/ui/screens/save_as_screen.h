#ifndef _SAVE_AS_SCREEN_H_
#define _SAVE_AS_SCREEN_H_

#include <Arduino.h>
#include <lvgl.h>
#include "screen.h"
#include "settings.h"

extern gnssBaseSettings systemSettings;

class ScreenManager;

class SaveAsScreen : public Screen {
public:
  void enter(ScreenManager* screenManager);
  void update(ScreenManager* screenManager);
  void exit(ScreenManager* screenManager);
  static Screen& getInstance();
 
private:
  SaveAsScreen() {}
  SaveAsScreen(const SaveAsScreen& other);
  SaveAsScreen& operator=(const SaveAsScreen& other);

};

#endif