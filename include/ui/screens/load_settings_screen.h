#ifndef _LOAD_SETTINGS_SCREEN_H_
#define _LOAD_SETTINGS_SCREEN_H_

#include <Arduino.h>
#include <lvgl.h>
#include <LittleFS.h>
#include "screen.h"
#include "settings.h"

extern gnssBaseSettings systemSettings;
extern LittleFS_QSPIFlash systemFS;

class ScreenManager;

class LoadSettingsScreen : public Screen {
public:
  void enter(ScreenManager* screenManager);
  void update(ScreenManager* screenManager);
  void exit(ScreenManager* screenManager);
  static Screen& getInstance();
 
private:
  LoadSettingsScreen() {}
  LoadSettingsScreen(const LoadSettingsScreen& other);
  LoadSettingsScreen& operator=(const LoadSettingsScreen& other);

};

#endif