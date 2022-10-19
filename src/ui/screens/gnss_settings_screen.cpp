#include "ui/screens/gnss_settings_screen.h"
#include "ui/screens/save_as_screen.h"

#include <Arduino.h>
 
#include "ui/screen_manager.h"
#include "ui/widgets/spinbox_widgets.h"
#include "ui/widgets/dropdown_widgets.h"

static void top_btns_cb(lv_event_t * e) {
  lv_event_code_t code = lv_event_get_code(e);
  if(code == LV_EVENT_CLICKED) {
    lv_obj_t* obj = lv_event_get_target(e);
    uint32_t id = lv_btnmatrix_get_selected_btn(obj);
    ScreenManager* screenManager = (ScreenManager*) lv_event_get_user_data(e);
    switch (id) {
      case 0:
        ((GNSSSettingsScreen*)screenManager->currentScreen())->updateControls();
        break;
      case 1:
        ((GNSSSettingsScreen*)screenManager->currentScreen())->updateSettings();
        systemSettings.save();
        break;
      case 2:
        ((GNSSSettingsScreen*)screenManager->currentScreen())->updateSettings();
        screenManager->changeScreen(SaveAsScreen::getInstance());
        break;
    }
  }
}


void GNSSSettingsScreen::enter(ScreenManager* screenManager) {
  Serial.println("Entering the GNSS Settings screen");
  screen_ = lv_obj_create(NULL);
  screenManager->setPageLabel("GNSS Settings");
  pageArea_ = lv_obj_create(screen_);
  lv_obj_set_size(pageArea_, 240, 320-35);
  lv_obj_set_pos(pageArea_, 0, 35);
  lv_obj_set_flex_flow(pageArea_, LV_FLEX_FLOW_COLUMN);

  static const char * topButtons_map[] = {"Reset","Save", "Save As",""};

  lv_obj_t* topButtons = lv_btnmatrix_create(pageArea_);
  lv_btnmatrix_set_map(topButtons, topButtons_map);
  lv_obj_set_size(topButtons, LV_PCT(100), 35);
  lv_obj_set_style_pad_all(topButtons, 0, LV_PART_MAIN);
  lv_obj_set_style_border_width(topButtons, 0, LV_PART_MAIN);
  lv_obj_add_event_cb(topButtons, top_btns_cb, LV_EVENT_ALL, screenManager);

  gps_cb = lv_checkbox_create(pageArea_);
  lv_checkbox_set_text(gps_cb, "GPS");
  
  glonass_cb = lv_checkbox_create(pageArea_);
  lv_checkbox_set_text(glonass_cb, "GLONASS");
  
  galileo_cb = lv_checkbox_create(pageArea_);
  lv_checkbox_set_text(galileo_cb, "Galileo");
  
  beidou_cb = lv_checkbox_create(pageArea_);
  lv_checkbox_set_text(beidou_cb, "BeiDou");
  
  lat = SpinboxInt32(pageArea_, "Lat");
  lat.setRange(-900000000, 900000000);
  lat.setFormat(9, 0);
  latHP = SpinboxInt8(pageArea_, "Lat HP");
  latHP.setRange(-99, 99);

  lon = SpinboxInt32(pageArea_, "Lon");
  lon.setRange(-1800000000, 1800000000);
  lon.setFormat(10, 0);
  lonHP = SpinboxInt8(pageArea_, "Lon HP");
  lonHP.setRange(-99, 99);

  alt = SpinboxInt32(pageArea_, "Alt");
  alt.setRange(-900000000, 900000000);
  alt.setFormat(9, 0);
  altHP = SpinboxInt8(pageArea_, "Alt HP");
  altHP.setRange(-99, 99);

  updateControls();
}

void GNSSSettingsScreen::update(ScreenManager* screenManager) {

}

void GNSSSettingsScreen::exit(ScreenManager* screenManager) { 
  lv_obj_del(screen_);
}

Screen& GNSSSettingsScreen::getInstance() {
	static GNSSSettingsScreen singleton;
	return singleton;
}

void GNSSSettingsScreen::updateControls() { 
  if (systemSettings.gnssSettings.gps()) {
    lv_obj_add_state(gps_cb, LV_STATE_CHECKED);
  } else {
    lv_obj_clear_state(gps_cb, LV_STATE_CHECKED);
  }

  if (systemSettings.gnssSettings.glonass()) {
    lv_obj_add_state(glonass_cb, LV_STATE_CHECKED);
  } else {
    lv_obj_clear_state(glonass_cb, LV_STATE_CHECKED);
  }

  if (systemSettings.gnssSettings.galileo()) {
    lv_obj_add_state(galileo_cb, LV_STATE_CHECKED);
  } else {
    lv_obj_clear_state(galileo_cb, LV_STATE_CHECKED);
  }

  if (systemSettings.gnssSettings.beidou()) {
    lv_obj_add_state(beidou_cb, LV_STATE_CHECKED);
  } else {
    lv_obj_clear_state(beidou_cb, LV_STATE_CHECKED);
  }
  
  lat.setValue(systemSettings.gnssSettings.lat());
  latHP.setValue(systemSettings.gnssSettings.latHP());
  lon.setValue(systemSettings.gnssSettings.lon());
  latHP.setValue(systemSettings.gnssSettings.lonHP());
  alt.setValue(systemSettings.gnssSettings.alt());
  altHP.setValue(systemSettings.gnssSettings.altHP());
  
}

void GNSSSettingsScreen::updateSettings() {
  lv_obj_get_state(gps_cb) & LV_STATE_CHECKED ? systemSettings.gnssSettings.gps(true) : systemSettings.gnssSettings.gps(false);
  lv_obj_get_state(glonass_cb) & LV_STATE_CHECKED ? systemSettings.gnssSettings.glonass(true) : systemSettings.gnssSettings.glonass(false);
  lv_obj_get_state(galileo_cb) & LV_STATE_CHECKED ? systemSettings.gnssSettings.galileo(true) : systemSettings.gnssSettings.galileo(false);
  lv_obj_get_state(beidou_cb) & LV_STATE_CHECKED ? systemSettings.gnssSettings.beidou(true) : systemSettings.gnssSettings.beidou(false);

  systemSettings.gnssSettings.lat(lat.getValue());
  systemSettings.gnssSettings.latHP(latHP.getValue());
  systemSettings.gnssSettings.lon(lon.getValue());
  systemSettings.gnssSettings.lonHP(lonHP.getValue());
  systemSettings.gnssSettings.alt(alt.getValue());
  systemSettings.gnssSettings.altHP(altHP.getValue());
}