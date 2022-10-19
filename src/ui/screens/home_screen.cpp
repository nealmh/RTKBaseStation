#include "ui/screens/home_screen.h"

#include <Arduino.h>



void HomeScreen::enter(ScreenManager* screenManager) {
  Serial.println("Entering the Home screen");
  screen_ = lv_obj_create(NULL);
  screenManager->setPageLabel("Home");
  pageArea_ = lv_obj_create(screen_);
  lv_obj_set_size(pageArea_, 240, 320-35);
  lv_obj_set_pos(pageArea_, 0, 35);
  lv_obj_set_flex_flow(pageArea_, LV_FLEX_FLOW_COLUMN);


  lv_obj_t* latLabel = lv_label_create(pageArea_);
  lv_label_set_text(latLabel, "Lat:");
  _latVal = lv_label_create(pageArea_);

  lv_obj_t* lonLabel = lv_label_create(pageArea_);
  lv_label_set_text(lonLabel, "Lon:");
  _lonVal = lv_label_create(pageArea_);

  lv_obj_t* accLabel = lv_label_create(pageArea_);
  lv_label_set_text(accLabel, "Accuracy(meters):");
  _accVal = lv_label_create(pageArea_);
  
}

void HomeScreen::update(ScreenManager* screenManager) {
  if (_lastUpdate > 1000) {
    _lastUpdate = 0;
    // First, let's collect the position data
    int32_t latitude = zedf9p.getHighResLatitude();
    int8_t latitudeHp = zedf9p.getHighResLatitudeHp();
    int32_t longitude = zedf9p.getHighResLongitude();
    int8_t longitudeHp = zedf9p.getHighResLongitudeHp();
    uint32_t accuracy = zedf9p.getHorizontalAccuracy();

    
    // Defines storage for the lat and lon units integer and fractional parts
    int32_t lat_int; // Integer part of the latitude in degrees
    int32_t lat_frac; // Fractional part of the latitude
    int32_t lon_int; // Integer part of the longitude in degrees
    int32_t lon_frac; // Fractional part of the longitude
    
    float f_accuracy;

    // Calculate the latitude and longitude integer and fractional parts
    lat_int = latitude / 10000000; // Convert latitude from degrees * 10^-7 to Degrees
    lat_frac = latitude - (lat_int * 10000000); // Calculate the fractional part of the latitude
    lat_frac = (lat_frac * 100) + latitudeHp; // Now add the high resolution component
    if (lat_frac < 0) // If the fractional part is negative, remove the minus sign
    {
      lat_frac = 0 - lat_frac;
    }
    lon_int = longitude / 10000000; // Convert latitude from degrees * 10^-7 to Degrees
    lon_frac = longitude - (lon_int * 10000000); // Calculate the fractional part of the longitude
    lon_frac = (lon_frac * 100) + longitudeHp; // Now add the high resolution component
    if (lon_frac < 0) // If the fractional part is negative, remove the minus sign
    {
      lon_frac = 0 - lon_frac;
    }

    // Convert the horizontal accuracy (mm * 10^-1) to a m
    f_accuracy = accuracy  / 10000.0;

    // Print the lat and lon
    lv_label_set_text_fmt(_latVal, "%d.%ld", lat_int, lat_frac);
    lv_label_set_text_fmt(_lonVal, "%d.%ld", lon_int, lon_frac);
    lv_label_set_text_fmt(_accVal, "%.4f", f_accuracy);
  }
}

void HomeScreen::exit(ScreenManager* screenManager) { 
  lv_obj_del(screen_);
}

Screen& HomeScreen::getInstance() {
	static HomeScreen singleton;
	return singleton;
}
