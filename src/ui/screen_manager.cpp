#include "ui/screen_manager.h"
#include <Arduino.h>
#include "ui/screens/home_screen.h"


void ScreenManager::setup() {
  // Load initial screen.
  Serial.println("Loading the initial screen");
  _statusBar.init(this);

  _screen = &HomeScreen::getInstance();
  _screen->enter(this);
  lv_scr_load(_screen->lv_scr());
  Serial.println("ScreenManager Setup complete.");
}

void ScreenManager::update() {
  // Service the current screen.
  _screen->update(this);
  _statusBar.setGNSSStatus(zedf9p.getFixType());
}

void ScreenManager::changeScreen(Screen& newScreen) {
  _screen->exit(this);
  _screen = &newScreen;
  _screen->enter(this);
  lv_scr_load(_screen->lv_scr());
}

  void ScreenManager::setPageLabel(const char* label) {
    _statusBar.setPageLabel(label);
  }

  void ScreenManager::setRXLed(bool status) {
    _statusBar.setRXLed(status);
  }

  void ScreenManager::setTXLed(bool status) {
    _statusBar.setTXLed(status);
  }