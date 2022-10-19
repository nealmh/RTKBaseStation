#include "ui/screens/system_screen.h"
#include "ui/screens/calibration_screen.h"
#include "ui/screens/save_as_screen.h"
#include "ui/screens/load_settings_screen.h"

#include <Arduino.h>

#include "ui/screen_manager.h"

static void calibrate_cb(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);
  
  if (code == LV_EVENT_CLICKED) {
    ScreenManager* sm = (ScreenManager*) lv_event_get_user_data(e);
    sm->changeScreen(CalibrationScreen::getInstance());
  }
}

static void save_as_cb(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);
  
  if (code == LV_EVENT_CLICKED) {
    ScreenManager* sm = (ScreenManager*) lv_event_get_user_data(e);
    sm->changeScreen(SaveAsScreen::getInstance());
  }
}

static void load_cb(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);
  
  if (code == LV_EVENT_CLICKED) {
    ScreenManager* sm = (ScreenManager*) lv_event_get_user_data(e);
    sm->changeScreen(LoadSettingsScreen::getInstance());
  }
}

void SystemScreen::enter(ScreenManager* screenManager) {
  Serial.println("Entering the System screen");
  screen_ = lv_obj_create(NULL);
  screenManager->setPageLabel("System");
  pageArea_ = lv_obj_create(screen_);
  lv_obj_set_size(pageArea_, 240, 320-35);
  lv_obj_set_pos(pageArea_, 0, 35);
  lv_obj_set_flex_flow(pageArea_, LV_FLEX_FLOW_COLUMN);

  lv_obj_t * calibrateButton = lv_btn_create(pageArea_);
  lv_obj_set_size(calibrateButton, LV_PCT(100), 35);
  lv_obj_add_event_cb(calibrateButton, calibrate_cb, LV_EVENT_ALL, screenManager);
  lv_obj_t* calibrateButtonLabel = lv_label_create(calibrateButton);
  lv_label_set_text(calibrateButtonLabel, "Calibrate");
  lv_obj_set_style_text_align(calibrateButtonLabel, LV_TEXT_ALIGN_CENTER, 0);

  lv_obj_t * saveAsButton = lv_btn_create(pageArea_);
  lv_obj_set_size(saveAsButton, LV_PCT(100), 35);
  lv_obj_add_event_cb(saveAsButton, save_as_cb, LV_EVENT_ALL, screenManager);
  lv_obj_t* saveAsButtonLabel = lv_label_create(saveAsButton);
  lv_label_set_text(saveAsButtonLabel, "Save Settings As");
  lv_obj_set_style_text_align(saveAsButtonLabel, LV_TEXT_ALIGN_CENTER, 0);

  lv_obj_t * loadButton = lv_btn_create(pageArea_);
  lv_obj_set_size(loadButton, LV_PCT(100), 35);
  lv_obj_add_event_cb(loadButton, load_cb, LV_EVENT_ALL, screenManager);
  lv_obj_t* loadButtonLabel = lv_label_create(loadButton);
  lv_label_set_text(loadButtonLabel, "Load Settings");
  lv_obj_set_style_text_align(loadButtonLabel, LV_TEXT_ALIGN_CENTER, 0);
};

void SystemScreen::exit(ScreenManager* screenManager) {
  lv_obj_del(screen_);
};

void SystemScreen::update(ScreenManager* screenManager) {

}

Screen& SystemScreen::getInstance() {
	static SystemScreen singleton;
	return singleton;
}