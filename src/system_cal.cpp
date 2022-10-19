
#include "settings.h"
#include <LittleFS.h>
#include <ArduinoJson.h>

// Loads the configuration from a file
void systemCal::load() {
  // Open file for reading
  if(!_systemFS->exists(_fileName)) {
    defaultCal();
    return;
  }
  // Open file for reading
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
  strcpy(lastConfig, doc["lastConfig"]);

  for (int i=0; i<6; i++) {
    touchCal[i] = doc["touchCal"][i];
  }
  
  // Close the file (Curiously, File's destructor doesn't close the file)
  configFile.close();
}

// Loads the configuration from a file
void systemCal::save() {
  if (!_dirty) return;

  // Open file for reading
  File configFile = _systemFS->open(_fileName, FILE_WRITE);
  configFile.truncate(0);

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<512> doc;

  // Save the name of the config file in use
  doc["lastConfig"] = lastConfig;

  // create the touch calibration array and populate it
  JsonArray tcal = doc.createNestedArray("touchCal");
  for (int i=0; i<6; i++) {
    tcal.add(touchCal[i]);
  }
  

  // Serialize JSON to file
  if (serializeJson(doc, configFile) == 0) {
    Serial.println(F("Failed to write to file"));
  }
  
  // Close the file (Curiously, File's destructor doesn't close the file)
  configFile.close();
}

void systemCal::defaultCal() {
  strcpy(_fileName, "cal");
  strcpy(lastConfig, "default");
  touchCal[0] = 1.0;
  touchCal[1] = 0.0;
  touchCal[2] =  0.0;
  touchCal[3] = 0.0;
  touchCal[4] = 1.0;
  touchCal[5] = 0.0;
}