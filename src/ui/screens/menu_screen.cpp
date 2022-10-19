#include "ui/screens/menu_screen.h"
#include "ui/screens/radio_settings_screen.h"
#include "ui/screens/gnss_settings_screen.h"
#include "ui/screens/system_screen.h"

static void menu_cb(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);

  if (code == LV_EVENT_CLICKED) {
    lv_obj_t* obj = lv_event_get_target(e);
    uint32_t id = lv_btnmatrix_get_selected_btn(obj);
    ScreenManager* screenManager = (ScreenManager*) lv_event_get_user_data(e);
    switch (id) {
      case 0:
        break;
      case 1:
        screenManager->changeScreen(SettingsScreen::getInstance());
        break;
      case 2:
        screenManager->changeScreen(GNSSSettingsScreen::getInstance());
        break;
      case 3:
        screenManager->changeScreen(SystemScreen::getInstance());
        break;
    }
  }
}

void MenuScreen::enter(ScreenManager* screenManager) {
  Serial.println("Entering the menu screen");
  screen_ = lv_obj_create(NULL);
  screenManager->setPageLabel("Main Menu");
  pageArea_ = lv_obj_create(screen_);
  lv_obj_set_size(pageArea_, 240, 320-35);
  lv_obj_set_pos(pageArea_, 0, 35);
  lv_obj_set_flex_flow(pageArea_, LV_FLEX_FLOW_COLUMN);

  static const char * menuButtons_map[] = {"Home","\n",
                                           "Radio Settings","\n",
                                           "GNSS Settings","\n",
                                           "System", ""
                                         };

  lv_obj_t * menuButtons = lv_btnmatrix_create(pageArea_);
  lv_btnmatrix_set_map(menuButtons, menuButtons_map);
  lv_obj_align(menuButtons, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_width(menuButtons, LV_PCT(100));
  lv_obj_set_style_pad_all(menuButtons, 0, LV_PART_MAIN);
  lv_obj_set_style_border_width(menuButtons, 0, LV_PART_MAIN);
  lv_obj_add_event_cb(menuButtons, menu_cb, LV_EVENT_ALL, screenManager);
}

void MenuScreen::update(ScreenManager* sm) {

}

void MenuScreen::exit(ScreenManager* sm) {
  lv_obj_del(screen_);
}

Screen& MenuScreen::getInstance() {
	static MenuScreen singleton;
	return singleton;
}

