#include "ui/screens/save_as_screen.h"

#include <Arduino.h>

static void ta_event_cb(lv_event_t * e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * ta = lv_event_get_target(e);
  lv_obj_t * kb = (lv_obj_t*)lv_event_get_user_data(e);
  if(code == LV_EVENT_FOCUSED) {
    lv_keyboard_set_textarea(kb, ta);
    lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
  }

  if(code == LV_EVENT_DEFOCUSED) {
    lv_keyboard_set_textarea(kb, NULL);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
  }
}

static void kb_event_cb(lv_event_t * e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * kb = lv_event_get_target(e);
  LV_UNUSED(kb);
  lv_obj_t * ta = (lv_obj_t*)lv_event_get_user_data(e);
  if(code == LV_EVENT_READY) {
    systemSettings.fileName(lv_textarea_get_text(ta));
    systemSettings.save();
  }

  if(code == LV_EVENT_CANCEL) {
    lv_textarea_set_text(ta, systemSettings.fileName());
  }
}

void SaveAsScreen::enter(ScreenManager* screenManager) {
  screen_ = lv_obj_create(NULL);
  screenManager->setPageLabel("Save As");
  pageArea_ = lv_obj_create(screen_);
  lv_obj_set_size(pageArea_, 240, 320-35);
  lv_obj_set_pos(pageArea_, 0, 35);
  lv_obj_set_flex_flow(pageArea_, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(pageArea_, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

  /*Create a text area. The keyboard will write here*/
  lv_obj_t * ta;
  ta = lv_textarea_create(pageArea_);
  lv_obj_set_size(ta, LV_PCT(100), 36);
  lv_textarea_set_placeholder_text(ta, "File Name");
  lv_textarea_set_text(ta, systemSettings.fileName());

  /*Create a keyboard to use it with an of the text areas*/
  lv_obj_t * kb = lv_keyboard_create(pageArea_);
  lv_obj_add_event_cb(ta, ta_event_cb, LV_EVENT_ALL, kb);
  lv_obj_add_event_cb(kb, kb_event_cb, LV_EVENT_ALL, ta);
  lv_keyboard_set_textarea(kb, ta);
  

  lv_obj_add_state(ta, LV_STATE_FOCUSED);
}

void SaveAsScreen::update(ScreenManager* screenManager) {

}

void SaveAsScreen::exit(ScreenManager* screenManager) { 
  lv_obj_del(screen_);
}

Screen& SaveAsScreen::getInstance() {
	static SaveAsScreen singleton;
	return singleton;
}
