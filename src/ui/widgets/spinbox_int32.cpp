#include <Arduino.h>
#include <lvgl.h>
#include "ui/widgets/spinbox_widgets.h"


SpinboxInt32::SpinboxInt32(lv_obj_t* parent, const char* label) {
  _container = lv_obj_create(parent);
  lv_obj_set_size(_container, LV_PCT(100), 35);
  lv_obj_set_style_border_width(_container, 0, LV_STATE_DEFAULT);
  lv_obj_set_flex_flow(_container, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(_container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_pad_all(_container, 0, LV_STATE_DEFAULT);

  _label = lv_label_create(_container);
  lv_label_set_text(_label, label);
  
  _minusBtn = lv_btn_create(_container);
  lv_obj_set_style_bg_img_src(_minusBtn, LV_SYMBOL_MINUS, 0);
  
  _spinbox = lv_spinbox_create(_container);
  lv_spinbox_set_range(_spinbox, -128, 127);
  _min = -128;
  _max = 127;
  lv_spinbox_set_digit_format(_spinbox, 3, 0);
  lv_spinbox_step_prev(_spinbox);
  lv_obj_set_width(_spinbox, 45);
  lv_spinbox_set_rollover(_spinbox, false);

  _plusBtn = lv_btn_create(_container);
  lv_obj_set_style_bg_img_src(_plusBtn, LV_SYMBOL_PLUS, 0);

  lv_obj_add_event_cb(_minusBtn, lv_spinbox_decrement_event_cb, LV_EVENT_ALL, _spinbox);
  lv_obj_add_event_cb(_plusBtn, lv_spinbox_increment_event_cb, LV_EVENT_ALL, _spinbox);
}

void SpinboxInt32::setRange(int32_t min, int32_t max) {
  lv_spinbox_set_range(_spinbox, min, max);
  _min = min;
  _max = max;
}

void SpinboxInt32::setMax(int32_t max) {
  _max = max;
  lv_spinbox_set_range(_spinbox, _min, max);
}

void SpinboxInt32::setMin(int32_t min) {
  _min = min;
  lv_spinbox_set_range(_spinbox, min, _max);
}

void SpinboxInt32::setValue(int32_t val) {
  lv_spinbox_set_value(_spinbox, val);
}

int32_t SpinboxInt32::getValue(){
    return static_cast<int32_t>(lv_spinbox_get_value(_spinbox));
}
