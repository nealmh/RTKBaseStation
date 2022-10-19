#ifndef _SPINBOX_WIDGETS_H_
#define _SPINBOX_WIDGETS_H_

#include <Arduino.h>
#include <lvgl.h>

void lv_spinbox_increment_event_cb(lv_event_t * e);
void lv_spinbox_decrement_event_cb(lv_event_t * e);

class SpinboxBase {
public:
  SpinboxBase() {}

  void setFormat(uint8_t digits, uint8_t pos);
  void setCursorPos(uint8_t pos);

protected:
  lv_obj_t* _container;
  lv_obj_t* _label;
  lv_obj_t* _spinbox;
  lv_obj_t* _minusBtn;
  lv_obj_t* _plusBtn;

  uint8_t _min;
  uint8_t _max;

private:
};

class SpinboxUInt8 : public SpinboxBase{
public:
  SpinboxUInt8() {}
  SpinboxUInt8(lv_obj_t* parent, const char* label);

  void setRange(uint8_t min, uint8_t max);
  void setMax(uint8_t max);
  void setMin(uint8_t min);

  void setValue(uint8_t val);
  uint8_t getValue();

protected:

private:
};

class SpinboxInt8 : public SpinboxBase {
public:
  SpinboxInt8() {}
  SpinboxInt8(lv_obj_t* parent, const char* label);

  void setRange(int8_t min, int8_t max);
  void setMax(int8_t max);
  void setMin(int8_t min);

  void setValue(int8_t val);
  int8_t getValue();

protected:

private:
};

class SpinboxUInt16 : public SpinboxBase {
public:
  SpinboxUInt16() {}
  SpinboxUInt16(lv_obj_t* parent, const char* label);

  void setRange(uint16_t min, uint16_t max);
  void setMax(uint16_t max);
  void setMin(uint16_t min);

  void setValue(uint16_t val);
  uint16_t getValue();

protected:

private:
};

class SpinboxInt32 : public SpinboxBase {
public:
  SpinboxInt32() {}
  SpinboxInt32(lv_obj_t* parent, const char* label);

  void setRange(int32_t min, int32_t max);
  void setMax(int32_t max);
  void setMin(int32_t min);

  void setValue(int32_t val);
  int32_t getValue();;

protected:

private:
};
#endif