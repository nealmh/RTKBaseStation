#ifndef _SYSTEM_SETTINGS_H_
#define _SYSTEM_SETTINGS_H_

#include <Arduino.h>
#include <LittleFS.h>

#include <rtcm_link_settings.h>
#include <gnss_settings.h>

class systemCal {
public:
  char lastConfig[32] = "default";
  float touchCal[6] = { 1.0, 0.0, 0.0, 0.0, 1.0, 0.0 };   // Touchscreen calibration
  bool _dirty = false;
  
  void load();
  void save();
  void defaultCal();

  LittleFS_QSPIFlash* fileSystem() {return _systemFS; }
  void fileSystem(LittleFS_QSPIFlash* fs) { _systemFS = fs; }

private:
  char _fileName[32] = "cal";
  LittleFS_QSPIFlash* _systemFS;
  

protected:
};

class gnssBaseSettings {
public:
  RTCMSettings radioSettings;
  GNSSSettings gnssSettings;

  void load();
  void save();
  bool dirty();
  
  LittleFS_QSPIFlash* fileSystem() {return _systemFS; }
  void fileSystem(LittleFS_QSPIFlash* fs) { _systemFS = fs; }

  const char* fileName() {return _fileName; };
  void fileName(const char* fileName) { strcpy(_fileName, fileName); }

private:
  char _fileName[32] = "default";
  LittleFS_QSPIFlash* _systemFS;

protected:
};

#endif