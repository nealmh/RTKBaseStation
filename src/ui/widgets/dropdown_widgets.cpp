#include <Arduino.h>
#include <lvgl.h>
#include "ui/widgets/dropdown_widgets.h"

Dropdown dropdown(lv_obj_t* parent, const char* label){
  //Create the Net ID Configuration
  Dropdown newDropdown;
  newDropdown.container = lv_obj_create(parent);
  lv_obj_set_size(newDropdown.container, LV_PCT(100), 35);
  lv_obj_set_style_border_width(newDropdown.container, 0, LV_STATE_DEFAULT);
  lv_obj_set_flex_flow(newDropdown.container, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(newDropdown.container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_pad_all(newDropdown.container, 0, LV_STATE_DEFAULT);

  newDropdown.label = lv_label_create(newDropdown.container);
  lv_label_set_text(newDropdown.label, label);
  
  newDropdown.dropdown = lv_dropdown_create(newDropdown.container);
  
  return newDropdown;
}