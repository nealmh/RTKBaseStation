#ifndef _GNSS_SETTINGS_SCREEN_H_
#define _GNSS_SETTINGS_SCREEN_H_

#include <Arduino.h>
#include <lvgl.h>

#include "settings.h"
#include "ui/screens/screen.h"
#include "ui/widgets/spinbox_widgets.h"

extern gnssBaseSettings systemSettings;

class ScreenManager;

class GNSSSettingsScreen : public Screen {
public:
  void enter(ScreenManager* screenManager);
  void update(ScreenManager* screenManager);
  void exit(ScreenManager* screenManager);
  static Screen& getInstance();
 
  void updateControls();
  void updateSettings();
 
private:
  GNSSSettingsScreen() {}
  GNSSSettingsScreen(const GNSSSettingsScreen& other);
  GNSSSettingsScreen& operator=(const GNSSSettingsScreen& other);

  lv_obj_t* gps_cb;
  lv_obj_t* glonass_cb;
  lv_obj_t* galileo_cb;
  lv_obj_t* beidou_cb;
  SpinboxInt32 lat;
  SpinboxInt8 latHP;
  SpinboxInt32 lon;
  SpinboxInt8 lonHP;
  SpinboxInt32 alt;
  SpinboxInt8 altHP;
};

#endif