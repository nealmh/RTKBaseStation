#ifndef _GNSS_SETTINGS_H_
#define _GNSS_SETTINGS_H_

#include <Arduino.h>


class GNSSSettings {
public:
  void defaultSettings();
  
  uint16_t sizeOfSettings();
  bool     gps() { return _gps; }
  void     gps(bool use);
  bool     glonass() { return _glonass; }
  void     glonass(bool use);
  bool     galileo() { return _galileo; }
  void     galileo(bool use);
  bool     beidou() { return _beidou; }
  void     beidou(bool use);

  int32_t  lat() { return _lat; }
  void     lat(int32_t val);
  int8_t   latHP() { return _lat_hp; }
  void     latHP(int8_t val);

  int32_t  lon() { return _lon; }
  void     lon(int32_t val);
  int8_t   lonHP() { return _lon_hp; }
  void     lonHP(int8_t val);
  
  int32_t  alt() { return _alt; }
  void     alt(int32_t val);
  int8_t   altHP() {return _alt_hp; }
  void     altHP(int8_t val);

  void     dirty(bool state);
  bool     dirty();
  
private:
  uint16_t _sizeOfSettings; //sizeOfSettings **must** be the first entry and must be int
  bool _dirty;  //indicates if the settings are changed

  bool _gps;
  bool _glonass;
  bool _galileo;
  bool _beidou;

  int32_t _lat;
  int8_t _lat_hp;
  int32_t _lon;
  int8_t _lon_hp;
  int32_t _alt;
  int8_t _alt_hp;
};

#endif