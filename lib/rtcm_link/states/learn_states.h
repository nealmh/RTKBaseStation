#ifndef _LEARN_STATES_H_
#define _LEARN_STATES_H_

#include <Arduino.h>
#include "state_base.h"
#include "../rtcm_link.h"

class LearnDataWait : public RadioStateBase {
public:
	void enter(RTCM_Link* radio) {}
	void update(RTCM_Link* radio);
	void exit(RTCM_Link* radio) {}
	static RadioStateBase& getInstance();

private:
	LearnDataWait() {}
	LearnDataWait(const LearnDataWait& other);
	LearnDataWait& operator=(const LearnDataWait& other);
};

class LearnRXPacket : public RadioStateBase {
public:
	void enter(RTCM_Link* radio) {}
	void update(RTCM_Link* radio);
	void exit(RTCM_Link* radio) {}
	static RadioStateBase& getInstance();

private:
	LearnRXPacket() {}
	LearnRXPacket(const LearnRXPacket& other);
	LearnRXPacket& operator=(const LearnRXPacket& other);
};

class LearnTX : public RadioStateBase {
public:
	void enter(RTCM_Link* radio) {}
	void update(RTCM_Link* radio);
	void exit(RTCM_Link* radio);
	static RadioStateBase& getInstance();

private:
	LearnTX() {}
	LearnTX(const LearnTX& other);
	LearnTX& operator=(const LearnTX& other);
};

#endif