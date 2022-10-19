#ifndef _TEACH_STATES_H_
#define _TEACH_STATES_H_

#include <Arduino.h>
#include "state_base.h"
#include "../rtcm_link.h"


class TeachRXWait : public RadioStateBase {
public:
	void enter(RTCM_Link* radio);
	void update(RTCM_Link* radio);
	void exit(RTCM_Link* radio) {}
	static RadioStateBase& getInstance();

private:
	TeachRXWait() {}
	TeachRXWait(const TeachRXWait& other);
	TeachRXWait& operator=(const TeachRXWait& other);
};

class TeachRXPacket : public RadioStateBase {
public:
	void enter(RTCM_Link* radio) {}
	void update(RTCM_Link* radio);
	void exit(RTCM_Link* radio);
	static RadioStateBase& getInstance();

private:
	TeachRXPacket() {}
	TeachRXPacket(const TeachRXPacket& other);
	TeachRXPacket& operator=(const TeachRXPacket& other);
};

class TeachTX : public RadioStateBase {
public:
	void enter(RTCM_Link* radio) {}
	void update(RTCM_Link* radio);
	void exit(RTCM_Link* radio) {}
	static RadioStateBase& getInstance();

private:
	TeachTX() {}
	TeachTX(const TeachTX& other);
	TeachTX& operator=(const TeachTX& other);
};

class TeachACKWait : public RadioStateBase {
public:
	void enter(RTCM_Link* radio);
	void update(RTCM_Link* radio);
	void exit(RTCM_Link* radio) {}
	static RadioStateBase& getInstance();

private:
	TeachACKWait() {}
	TeachACKWait(const TeachACKWait& other);
	TeachACKWait& operator=(const TeachACKWait& other);
};

#endif