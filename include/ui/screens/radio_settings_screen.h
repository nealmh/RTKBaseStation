#ifndef _RADIO_SETTINGS_SCREEN_H_
#define _RADIO_SETTINGS_SCREEN_H_

#include <Arduino.h>
#include <lvgl.h>

#include "settings.h"
#include "ui/screens/screen.h"
#include "ui/widgets/spinbox_widgets.h"
#include "ui/widgets/dropdown_widgets.h"

extern gnssBaseSettings systemSettings;

class ScreenManager;

class SettingsScreen : public Screen {
 public:
  void enter(ScreenManager* screenManager);
  void update(ScreenManager* screenManager);
  void exit(ScreenManager* screenManager);
  static Screen& getInstance();
 
  void updateControls();
  void updateSettings();

 private:
  SettingsScreen() {}
  SettingsScreen(const SettingsScreen& other);
  SettingsScreen& operator=(const SettingsScreen& other);

  SpinboxUInt8 netID;
  Dropdown airSpeed;
  Dropdown bandwidth;
  SpinboxUInt8 spreadFactor;
  SpinboxUInt8 codingRate;
  SpinboxUInt16 preambleLength;
};

#endif