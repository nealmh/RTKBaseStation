#ifndef _BASE_STATES_H_
#define _BASE_STATES_H_

#include <Arduino.h>
#include "state_base.h"
#include "../rtcm_link.h"

class RXStandby : public RadioStateBase {
public:
	void enter(RTCM_Link* radio);
	void update(RTCM_Link* radio);
	void exit(RTCM_Link* radio) {}
	static RadioStateBase& getInstance();

private:
	RXStandby() {}
	RXStandby(const RXStandby& other);
	RXStandby& operator=(const RXStandby& other);
};

class RXPacket : public RadioStateBase {
public:
	void enter(RTCM_Link* radio) {}
	void update(RTCM_Link* radio);
	void exit(RTCM_Link* radio) {}
	static RadioStateBase& getInstance();

private:
	RXPacket() {}
	RXPacket(const RXPacket& other);
	RXPacket& operator=(const RXPacket& other);
};

class TX : public RadioStateBase {
public:
	void enter(RTCM_Link* radio);
	void update(RTCM_Link* radio);
	void exit(RTCM_Link* radio) {}
	static RadioStateBase& getInstance();

private:
	TX() {}
	TX(const TX& other);
	TX& operator=(const TX& other);
};

#endif