#include "ui/status_bar.h"
#include "ui/screens/menu_screen.h"
#include "ui/widgets/indicator_widgets.h"
#include "ui/screen_manager.h"

//FontAwsome symbols
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
LV_FONT_DECLARE(fa_custom);
#define MENU_SYMBOL "\xEF\x83\x89"
#define GNSS_SYMBOL "\xEF\x9E\xBF"
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//Callback for the menu button
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
static void menu_btn_handler(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);
  if(code == LV_EVENT_CLICKED) {
    ScreenManager* sm = (ScreenManager*) lv_event_get_user_data(e);
    if (*(sm->currentScreen()) != MenuScreen::getInstance()) {
      sm->changeScreen(MenuScreen::getInstance());
    }
  }
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void StatusBar::init(ScreenManager* screenManager) {
  screenManager_ = screenManager;

  static lv_style_t status_bar_style;
  lv_style_init(&status_bar_style);
  lv_style_set_border_width(&status_bar_style, 0);
  lv_style_set_pad_all(&status_bar_style,0);
  lv_style_set_outline_width(&status_bar_style, 0);
  lv_style_set_radius(&status_bar_style, 0);
  lv_style_set_bg_color(&status_bar_style, lv_palette_main(LV_PALETTE_LIGHT_BLUE));
  
  bar = lv_obj_create(lv_layer_top());
  lv_obj_clear_flag(bar, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_style(bar, &status_bar_style, LV_PART_MAIN);
  lv_obj_set_size(bar, LV_PCT(100), 35);
  lv_obj_set_pos(bar, 0, 0);
  
  //Create the transmit/receive indicators
  static lv_style_t led_base_style;
  lv_style_init(&led_base_style);
  lv_style_set_pad_all(&led_base_style, 0);
  lv_style_set_radius(&led_base_style, 0);

  txLED = smallLED(bar, lv_palette_main(LV_PALETTE_RED));
  lv_obj_align(txLED.indicator, LV_ALIGN_RIGHT_MID, -5, 0);
  lv_led_on(txLED.indicator);

  rxLED = smallLED(bar, lv_palette_main(LV_PALETTE_GREEN));
  lv_obj_align(rxLED.indicator, LV_ALIGN_RIGHT_MID, -20, 0);
  lv_led_off(rxLED.indicator);

  gpsInd = smallIndicator(bar, &fa_custom, GNSS_SYMBOL);
  lv_obj_align(gpsInd.indicator, LV_ALIGN_RIGHT_MID, -35, 7);
  lv_obj_set_style_bg_color(gpsInd.indicator, lv_palette_main(LV_PALETTE_LIGHT_BLUE), LV_PART_MAIN);
  lv_obj_set_style_text_color(gpsInd.indicator, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);

  //Create the page label
  pageLabel = lv_label_create(bar);
  lv_obj_set_style_text_align(pageLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
  lv_obj_set_style_text_font(pageLabel, &lv_font_montserrat_12, LV_PART_MAIN);
  lv_obj_set_align(pageLabel, LV_ALIGN_CENTER);
  
  //Create the menu button
  menuButton = lv_btn_create(bar);
  lv_obj_set_size(menuButton, 35, LV_PCT(100));
  lv_obj_set_style_pad_all(menuButton, 0, LV_PART_MAIN);
  lv_obj_set_style_border_width(menuButton, 0, LV_PART_MAIN);
  lv_obj_set_style_outline_width(menuButton, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_img_src(menuButton, MENU_SYMBOL, 0);
  lv_obj_set_style_text_font(menuButton, &fa_custom, 0);
  lv_obj_add_event_cb(menuButton, menu_btn_handler, LV_EVENT_ALL, screenManager_);
  lv_obj_set_align(menuButton, LV_ALIGN_LEFT_MID);
}

void StatusBar::setGNSSStatus(byte fixType){
  
  if(fixType == 2) lv_obj_set_style_text_color(gpsInd.indicator, lv_palette_main(LV_PALETTE_YELLOW), LV_PART_MAIN);
  else if(fixType == 3) lv_obj_set_style_text_color(gpsInd.indicator, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN);
  else lv_obj_set_style_text_color(gpsInd.indicator, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);
}

void StatusBar::setRXLed(bool status) {
  if (status) {
    lv_led_on(rxLED.indicator);
  } else {
    lv_led_off(rxLED.indicator);
  }
}

void StatusBar::setTXLed(bool status) {
  if (status) {
    lv_led_on(txLED.indicator);
  } else {
    lv_led_off(txLED.indicator);
  }
}

void StatusBar::setPageLabel(const char *text) {
  lv_label_set_text(pageLabel, text);
}