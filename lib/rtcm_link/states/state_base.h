#ifndef _RADIO_STATE_H_
#define _RADIO_STATE_H_

#include <Arduino.h>
#include "../rtcm_link.h"

class RTCM_Link;

class RadioStateBase {
public:
	virtual void enter(RTCM_Link* radio) = 0;
	virtual void update(RTCM_Link* radio) = 0;
	virtual void exit(RTCM_Link* radio) = 0;
	virtual ~RadioStateBase() {}
};

#endif