#include <Arduino.h>
#include <lvgl.h>
#include "ui/widgets/indicator_widgets.h"



Indicator smallIndicator(lv_obj_t* parent, const lv_font_t* font, const char* symbol){
  Indicator newIndicator;
  

  newIndicator.indicator = lv_label_create(parent);
  lv_obj_set_size(newIndicator.indicator, 30, 30);
  lv_obj_set_style_text_align(newIndicator.indicator, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
  lv_obj_set_style_text_font(newIndicator.indicator, font, 0);
  lv_label_set_text(newIndicator.indicator, symbol);

  return newIndicator;
}

Indicator smallLED(lv_obj_t* parent, lv_color_t color) {
  //Create the LED base style
  static lv_style_t led_base_style;
  lv_style_init(&led_base_style);
  lv_style_set_pad_all(&led_base_style, 0);
  lv_style_set_radius(&led_base_style, 0);
  
  Indicator newLED;
  newLED.indicator = lv_led_create(parent);
  lv_obj_add_style(newLED.indicator, &led_base_style, LV_PART_MAIN);
  lv_obj_set_size(newLED.indicator, 10, 30);
  lv_led_set_color(newLED.indicator, color);
  lv_led_off(newLED.indicator);

  return newLED;
}