#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <rtcm_link.h>
#include <LittleFS.h>
#include <ILI9341_T4.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>

#include "system.h"
#include "settings.h"
#include "ui/screen_manager.h"
#include "ui/gui_setup.h"

extern RTCM_Link radioLink;
extern LittleFS_QSPIFlash systemFS;
extern ILI9341_T4::ILI9341Driver tft;
systemCal calibration;
gnssBaseSettings systemSettings;
extern ScreenManager screenManager;

SFE_UBLOX_GNSS zedf9p;

void setup() {
  Serial.begin(115200);
  while(!Serial){}
  beginDisplay();

  Wire.begin();
  Wire.setClock(400000);

  beginFileSystem();
  calibration.fileSystem(&systemFS);
  calibration.load();
  tft.setTouchCalibration(calibration.touchCal);

  systemSettings.fileSystem(&systemFS);
  systemSettings.fileName(calibration.lastConfig);
  systemSettings.load();

  if (zedf9p.begin(Wire) == false) { //Connect to the u-blox module using Wire port
    Serial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  } else {
    Serial.println(F("u-blox GNSS started."));
  }

  zedf9p.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  zedf9p.setNavigationFrequency(20); //Set output to 20 times a second

  beginWDT();
  //beginRTCMLink(systemSettings.radioSettings);
  Serial.println("Configuration Complete.");
}

void loop() {
  petWDT();
  lv_task_handler();
  screenManager.update();

  //radioLink.update();
}