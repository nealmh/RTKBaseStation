
#include "system.h"
#include "Watchdog_t4.h"
#include "LittleFS.h"

//Watchdog
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
WDT_T4<WDT1> systemWDT;

uint16_t petTimeoutHalf = 0; //Half the amount of time before WDT. Helps reduce amount of time spent petting.
elapsedMillis lastPet = 0; //Remebers time of last WDT pet.
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//Configuration and Calibration File System
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LittleFS_QSPIFlash systemFS;
File configFile;  // The system configuration file
File calFile;     // The hardware calibration file
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void beginWDT() {
  WDT_timings_t config;
  config.timeout = 1; /* in seconds, 0->128 */
  petTimeoutHalf = 500;
  systemWDT.begin(config);
  Serial.println("WDT initialized");
}

//Delay with pets of WDT when needed
void delayWDT(uint16_t delayAmount) {
  elapsedMillis startTime = 0;
  while (startTime < delayAmount)   {
    delay(1);
    petWDT();
  }
}

void petWDT() {
  //don't always clear the WDT, only after we've passed the half way point
  if (lastPet > petTimeoutHalf) {
    lastPet = 0;
    systemWDT.feed();
  }
}

void beginFileSystem() {
  if (!systemFS.begin()) {
    Serial.println("Error starting Config Memory");
    while (1) {
      // Error, so don't do anything more - stay stuck here
    }
  } else {
    if (!systemFS.exists("/settings")) {
      Serial.println("Settings folder not found, creating now.");
      systemFS.mkdir("/settings");
    }
    Serial.println("Config memeory initialized.");
  }
}

//Platform specific reset commands
void systemReset() {
  SCB_AIRCR = 0x05FA0004;
}

