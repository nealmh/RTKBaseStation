#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <lvgl.h>
#include "../screen_manager.h"

template< typename _T1, typename _T2 > bool CompareType( _T1 a, _T2 b ) {
  return false;
}
  
template< typename _A > bool CompareType( _A a, _A b ) {
  return true;
}

class ScreenManager;

class Screen {
 public:
  // Called before becoming the active screen.
  virtual void enter(ScreenManager* sm) = 0;
  // Called periodically when this is the active screen.
  virtual void update(ScreenManager* sm) = 0;
  // Called when the active screen before becoming inactive,
  // before calling the on_load() of next screen.
  virtual void exit(ScreenManager* sm) = 0;
  virtual ~Screen() {}

  lv_obj_t* lv_scr() { return screen_; }
 
  virtual bool operator==(const Screen& other) const {
    return CompareType(this, &other);
  }
 
  virtual bool operator!=(const Screen& other) const {
    return ~CompareType(this, &other);
  }

 protected:
  lv_obj_t* screen_ = nullptr;
  lv_obj_t* pageArea_ = nullptr;
 
  // Uses an internal temp formatting buffer that is
  // share by all screens.
  static const char* format(const char* format, ...);

 private:
};

#endif