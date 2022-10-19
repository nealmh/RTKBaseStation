#include "gnss_settings.h"

void GNSSSettings::defaultSettings(){
  _gps = true;
  _glonass = true;
  _galileo = true;
  _beidou = true;

  _lat = 333208339;
  _lat_hp = 75;
  _lon = -1044633856;
  _lon_hp = -66;
  _alt = 108900;
  _alt_hp = 0;
}

void GNSSSettings::gps(bool use) {
  if (_gps != use) _dirty = true;
  _gps = use; 
};
void GNSSSettings::glonass(bool use) { 
  if (_glonass != use) _dirty = true;
  _glonass = use; 
};
void GNSSSettings::galileo(bool use) { 
  if (_galileo != use) _dirty = true;
  _galileo = use; 
};
void GNSSSettings::beidou(bool use) { 
  if (_beidou != use) _dirty = true;
  _beidou = use; 
};

void GNSSSettings::lat(int32_t val) {
  if (-1800000000>val) val = -1800000000;
  if (1800000000<val) val = 1800000000;
  if (val != _lat) _dirty = true;
  _lat = val;
}

void GNSSSettings::latHP(int8_t val) {
  if (-99>val) val = -99;
  if (99<val) val = 99;
  if (val != _lat_hp) _dirty = true;
  _lat_hp = val;
}

void GNSSSettings::lon(int32_t val) {
  if (-900000000>val) val = -900000000;
  if (900000000<val) val = 900000000;
  if (val != _lon) _dirty = true;
  _lon = val;
}

void GNSSSettings::lonHP(int8_t val) {
  if (-99>val) val = -99;
  if (99<val) val = 99;
  if (val != _lon_hp) _dirty = true;
  _lon_hp = val;
}

void GNSSSettings::alt(int32_t val) {
  if (-900000000>val) val = -900000000;
  if (900000000<val) val = 900000000;
  if (val != _alt) _dirty = true;
  _alt = val;
}

void GNSSSettings::altHP(int8_t val) {
  if (-99>val) val = -99;
  if (99<val) val = 99;
  if (val != _alt_hp) _dirty = true;
  _alt_hp = val;
}

void GNSSSettings::dirty(bool state) {
_dirty = state;
}

bool GNSSSettings::dirty() {
return _dirty;
}

