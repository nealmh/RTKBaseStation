#include "ui/screens/load_settings_screen.h"

#include <Arduino.h>


void LoadSettingsScreen::enter(ScreenManager* screenManager) {
  screen_ = lv_obj_create(NULL);
  screenManager->setPageLabel("Load Settings");
  pageArea_ = lv_obj_create(screen_);
  lv_obj_set_size(pageArea_, 240, 320-35);
  lv_obj_set_pos(pageArea_, 0, 35);
  lv_obj_set_flex_flow(pageArea_, LV_FLEX_FLOW_COLUMN);

  lv_obj_t* fileList = lv_list_create(pageArea_);
  File dir = systemFS.open("/settings");

  while(true) {
    File entry = dir.openNextFile();
    if (! entry) {
      //Serial.println("** no more files **");
      break;
    }
    else if (!entry.isDirectory()) {
      char fileName[32];
      sprintf(fileName, "%.*s", strlen(entry.name())-6, entry.name());
      lv_obj_t* newFileBtn = lv_list_add_btn(fileList, nullptr, fileName);
  
      if (fileName == systemSettings.fileName()) {
        lv_obj_add_state(newFileBtn, LV_STATE_CHECKED);
      }
    }
    entry.close();
  }  
}

void LoadSettingsScreen::update(ScreenManager* screenManager) {

}

void LoadSettingsScreen::exit(ScreenManager* screenManager) { 
  lv_obj_del(screen_);
}

Screen& LoadSettingsScreen::getInstance() {
	static LoadSettingsScreen singleton;
	return singleton;
}
