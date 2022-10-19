#include <Arduino.h>
#include <lvgl.h>
#include "ui/widgets/spinbox_widgets.h"

void lv_spinbox_increment_event_cb(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  if(code == LV_EVENT_SHORT_CLICKED || code  == LV_EVENT_LONG_PRESSED_REPEAT) {
    lv_obj_t* spinbox = (lv_obj_t*) lv_event_get_user_data(e);
    lv_spinbox_increment(spinbox);
  }
}

void lv_spinbox_decrement_event_cb(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  if(code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
    lv_obj_t* spinbox = (lv_obj_t*) lv_event_get_user_data(e);
    lv_spinbox_decrement(spinbox);
  }
}

void SpinboxBase::setFormat(uint8_t digits, uint8_t pos) {
  lv_spinbox_set_digit_format(_spinbox, digits, pos);
}

void SpinboxBase::setCursorPos(uint8_t pos) {
  lv_spinbox_set_cursor_pos(_spinbox, pos);
}