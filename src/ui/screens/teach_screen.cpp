#include "ui/screens/teach_screen.h"

#include <Arduino.h>

#include "ui/screen_manager.h"


void TeachScreen::enter(ScreenManager* screenManager) {
  Serial.println("Entering the Teach screen");
  screen_ = lv_obj_create(NULL);
  screenManager->setPageLabel("Teaching");
  pageArea_ = lv_obj_create(screen_);
  lv_obj_set_size(pageArea_, 240, 320-35);
  lv_obj_set_pos(pageArea_, 0, 35);

  lv_obj_set_flex_align(pageArea_, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
  spinner = lv_spinner_create(pageArea_, 1000, 60);
  lv_obj_set_size(spinner, 100, 100);
  lv_obj_center(spinner);

  teachStatusText = lv_label_create(pageArea_);
  lv_label_set_text(teachStatusText, "Waiting for ping....");
};

void TeachScreen::exit(ScreenManager* screenManager) {
  lv_obj_del(screen_);
};

void TeachScreen::update(ScreenManager* screenManager) {

}

Screen& TeachScreen::getInstance() {
	static TeachScreen singleton;
	return singleton;
}