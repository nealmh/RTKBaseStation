#ifndef _INDICATOR_WIDGETS_H_
#define _INDICATOR_WIDGETS_H_

#include <Arduino.h>
#include <lvgl.h>

struct Indicator{
  lv_obj_t* indicator;
};

Indicator smallIndicator(lv_obj_t* parent, const lv_font_t* font, const char* symbol);
Indicator smallLED(lv_obj_t* parent, lv_color_t color);

#endif