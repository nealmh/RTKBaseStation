#include "ui/screens/radio_settings_screen.h"
#include "ui/screen_manager.h"

#include <Arduino.h>
 
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
        //sm->changeScreen(ScreenId::TEACH);
        break;
      case 1:
        ((SettingsScreen*)screenManager->currentScreen())->updateControls();
        break;
      case 2:
        ((SettingsScreen*)screenManager->currentScreen())->updateSettings();
        systemSettings.save();
        break;
    }
  }
}

void SettingsScreen::enter(ScreenManager* screenManager) {
  Serial.println("Entering the Settings screen");
  screen_ = lv_obj_create(NULL);
  screenManager->setPageLabel("Settings");  
  pageArea_ = lv_obj_create(screen_);
  lv_obj_set_size(pageArea_, 240, 320-35);
  lv_obj_set_pos(pageArea_, 0, 35);
  lv_obj_set_flex_flow(pageArea_, LV_FLEX_FLOW_COLUMN);

  static const char * topButtons_map[] = {"Teach","Reset","Save",""};

  lv_obj_t * topButtons = lv_btnmatrix_create(pageArea_);
  lv_btnmatrix_set_map(topButtons, topButtons_map);
  lv_obj_set_size(topButtons, LV_PCT(100), 35);
  lv_obj_set_style_pad_all(topButtons, 0, LV_PART_MAIN);
  lv_obj_set_style_border_width(topButtons, 0, LV_PART_MAIN);
  lv_obj_add_event_cb(topButtons, top_btns_cb, LV_EVENT_ALL, screenManager);

  //Create the Net ID Configuration
  netID = SpinboxUInt8(pageArea_, "Net ID");

  //Create the Air Speed Configuration
  airSpeed = dropdown(pageArea_, "Air Speed");
  lv_dropdown_set_options(airSpeed.dropdown, "0\n"
                                             "40\n"
                                             "150\n"
                                             "400\n"
                                             "1200\n"
                                             "2400\n"
                                             "4800\n"
                                             "9600\n"
                                             "19200\n"
                                             "28800\n"
                                             "38400");
  
  //Create the Radio Bandwidth Configuration
  bandwidth = dropdown(pageArea_, "Bandwidth");
  lv_dropdown_set_options(bandwidth.dropdown, "10.4\n"
                                              "15.6\n"
                                              "20.8\n"
                                              "31.25\n"
                                              "41.7\n"
                                              "62.5\n"
                                              "125.0\n"
                                              "250.0\n"
                                              "500.0");
                                                   
  //Create the Spread Factor Configuration
  spreadFactor =SpinboxUInt8(pageArea_, "Spread Factor");
  spreadFactor.setRange(6, 12);
  spreadFactor.setFormat(2, 0);
  spreadFactor.setCursorPos(0);
                                      
  //Create the Coding Rate Configuration
  codingRate = SpinboxUInt8(pageArea_, "Coding Rate");
  codingRate.setRange(5, 8);
  codingRate.setFormat(1, 0);
  codingRate.setCursorPos(0);

  //Create the Preamble Length Configuration
  preambleLength = SpinboxUInt16(pageArea_, "Preamble");
  preambleLength.setRange(6, 65535);
  
  /*
  void setEncryptionKey(AESKey key);
  void setRadioBroadcastPower_dbm(uint16_t power);
  */

 updateControls();
};

void SettingsScreen::update(ScreenManager* sm) {
}

void SettingsScreen::exit(ScreenManager* sm) {
  lv_obj_del(screen_);
}

Screen& SettingsScreen::getInstance() {
	static SettingsScreen singleton;
	return singleton;
}

void SettingsScreen::updateControls() { 
  //Set the Net ID to match the settings
  netID.setValue(systemSettings.radioSettings.netID());


  //Set the air speed control to match the settings
  switch (systemSettings.radioSettings.airSpeed()) {
    case 0:
      lv_dropdown_set_selected(airSpeed.dropdown, 0);
      break;
    case 40:
      lv_dropdown_set_selected(airSpeed.dropdown, 1);
      break;
    case 150:
      lv_dropdown_set_selected(airSpeed.dropdown, 2);
      break;
    case 400:
      lv_dropdown_set_selected(airSpeed.dropdown, 3);
      break;
    case 1200:
      lv_dropdown_set_selected(airSpeed.dropdown, 4);
      break;
    case 2400:
      lv_dropdown_set_selected(airSpeed.dropdown, 5);
      break;
    case 4800:
      lv_dropdown_set_selected(airSpeed.dropdown, 6);
      break;
    case 9600:
      lv_dropdown_set_selected(airSpeed.dropdown, 7);
      break;
    case 19200:
      lv_dropdown_set_selected(airSpeed.dropdown, 8);
      break;
    case 28800:
      lv_dropdown_set_selected(airSpeed.dropdown, 9);
      break;
    case 38400:
      lv_dropdown_set_selected(airSpeed.dropdown, 10);
      break;
    default:
      break;
  }
  
  //Set the Radio Bandwidth to match the settings
  switch ((int)(100 * systemSettings.radioSettings.bandwidth())) {
    case 1040:
      lv_dropdown_set_selected(bandwidth.dropdown, 0);
      break;
    case 1560:
      lv_dropdown_set_selected(bandwidth.dropdown, 1);
      break;
    case 2080:
      lv_dropdown_set_selected(bandwidth.dropdown, 2);
      break;
    case 3125:
      lv_dropdown_set_selected(bandwidth.dropdown, 3);
      break;
    case 4170:
      lv_dropdown_set_selected(bandwidth.dropdown, 4);
      break;
    case 6250:
      lv_dropdown_set_selected(bandwidth.dropdown, 5);
      break;
    case 12500:
      lv_dropdown_set_selected(bandwidth.dropdown, 6);
      break;
    case 25000:
      lv_dropdown_set_selected(bandwidth.dropdown, 7);
      break;
    case 50000:
      lv_dropdown_set_selected(bandwidth.dropdown, 8);
      break;
    default:
      break;
  }
                                                   
  //Set the Spread Factor to match the settings
  spreadFactor.setValue(systemSettings.radioSettings.spreadFactor());
                                      
  //Set the Coding Rate to match the settings
  codingRate.setValue(systemSettings.radioSettings.codingRate());

  //Set the Preamble Length to match the settings
  preambleLength.setValue(systemSettings.radioSettings.preambleLength());
  
  /*
  void setRadioBroadcastPower_dbm(uint16_t power);
  */
}

void SettingsScreen::updateSettings() {
  //Set the Net ID to match the settings
  systemSettings.radioSettings.netID(netID.getValue());


  //Set the air speed control to match the settings
  switch (lv_dropdown_get_selected(airSpeed.dropdown)) {
    case 0:
      systemSettings.radioSettings.airSpeed(0);
      break;
    case 1:
      systemSettings.radioSettings.airSpeed(40);
      break;
    case 2:
      systemSettings.radioSettings.airSpeed(150);
      break;
    case 3:
      systemSettings.radioSettings.airSpeed(400);
      break;
    case 4:
      systemSettings.radioSettings.airSpeed(1200);
      break;
    case 5:
      systemSettings.radioSettings.airSpeed(2400);
      break;
    case 6:
      systemSettings.radioSettings.airSpeed(4800);
      break;
    case 7:
      systemSettings.radioSettings.airSpeed(9600);
      break;
    case 8:
      systemSettings.radioSettings.airSpeed(19200);
      break;
    case 9:
      systemSettings.radioSettings.airSpeed(28800);
      break;
    case 10:
      systemSettings.radioSettings.airSpeed(38400);
      break;
    default:
      break;
  }
  
  //Set the Radio Bandwidth to match the settings
  switch (lv_dropdown_get_selected(bandwidth.dropdown)) {
    case 0:
      systemSettings.radioSettings.bandwidth(1040);
      break;
    case 1:
      systemSettings.radioSettings.bandwidth(1560);
      break;
    case 2:
      systemSettings.radioSettings.bandwidth(2080);
      break;
    case 3:
      systemSettings.radioSettings.bandwidth(3125);
      break;
    case 4:
      systemSettings.radioSettings.bandwidth(4170);
      break;
    case 5:
      systemSettings.radioSettings.bandwidth(6250);
      break;
    case 6:
      systemSettings.radioSettings.bandwidth(12500);
      break;
    case 7:
      systemSettings.radioSettings.bandwidth(25000);
      break;
    case 8:
      systemSettings.radioSettings.bandwidth(50000);
      break;
    default:
      break;
  }
                                                   
  //Set the Spread Factor to match the settings
  systemSettings.radioSettings.spreadFactor(spreadFactor.getValue());
                                      
  //Set the Coding Rate to match the settings
  systemSettings.radioSettings.codingRate(codingRate.getValue());

  //Set the Preamble Length to match the settings
  systemSettings.radioSettings.preambleLength(preambleLength.getValue());
  
  /*
  void setEncryptionKey(AESKey key);
  void setRadioBroadcastPower_dbm(uint16_t power);
  */

}