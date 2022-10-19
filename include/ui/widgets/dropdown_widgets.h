#ifndef _DROPDOWN_WIDGETS_H_
#define _DROPDOWN_WIDGETS_H_

#include <Arduino.h>
#include <lvgl.h>

struct Dropdown{
  lv_obj_t* container;
  lv_obj_t* label;
  lv_obj_t* dropdown;
};

Dropdown dropdown(lv_obj_t* parent, const char* label);

#endif