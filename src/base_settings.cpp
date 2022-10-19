#include "settings.h"

#include <LittleFS.h>
#include <ArduinoJson.h>

// Loads the configuration from a file
void gnssBaseSettings::load() {
  // Open file for reading
  if(!_systemFS->exists(_fileName)) {
    radioSettings.defaultSettings();
    gnssSettings.defaultSettings();
    return;
  }
  
  File configFile = _systemFS->open(_fileName);

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<512> doc;

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, configFile);
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));

  // Copy the radio settings
  radioSettings.netID(doc["rtcmLink"]["netID"]);

  radioSettings.broadcastPower_dbm(doc["rtcmLink"]["power"]);
  radioSettings.airSpeed(doc["rtcmLink"]["airSpeed"]);
  radioSettings.bandwidth(doc["rtcmLink"]["bandwidth"]);
  radioSettings.spreadFactor(doc["rtcmLink"]["spreadFactor"]);
  radioSettings.codingRate(doc["rtcmLink"]["codingRate"]);
  radioSettings.preambleLength(doc["rtcmLink"]["preambleLength"]);
  
  // Close the file (Curiously, File's destructor doesn't close the file)
  configFile.close();
}

// Loads the configuration from a file
void gnssBaseSettings::save() {
  if (!dirty()) return;

  // Open file for reading
  File configFile = _systemFS->open(_fileName, FILE_WRITE);
  configFile.truncate(0);

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<512> doc;

  // Copy the radio settings
  doc["rtcmLink"]["netID"] = radioSettings.netID();
  
  doc["rtcmLink"]["power"] = radioSettings.broadcastPower_dbm();
  doc["rtcmLink"]["airSpeed"] = radioSettings.airSpeed();
  doc["rtcmLink"]["bandwidth"] = radioSettings.bandwidth();
  doc["rtcmLink"]["spreadFactor"] = radioSettings.spreadFactor();
  doc["rtcmLink"]["codingRate"] = radioSettings.codingRate();
  doc["rtcmLink"]["preambleLength"] = radioSettings.preambleLength();

  // Serialize JSON to file
  if (serializeJson(doc, configFile) == 0) {
    Serial.println(F("Failed to write to file"));
  }
  
  // Close the file (Curiously, File's destructor doesn't close the file)
  configFile.close();
}

bool gnssBaseSettings::dirty() {
  if (radioSettings.dirty() | gnssSettings.dirty()) {
    return true;
  }

  return false;
}