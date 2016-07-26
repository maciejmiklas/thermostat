#ifndef TIMER_H_
#define TIMER_H_

#include "Arduino.h"
#include "Util.h"
#include "ArdLog.h"

typedef struct {
	uint16_t dd;
	uint8_t hh;
	uint8_t mm;
	uint8_t ss;
} Time;

class Timer {

public:
	Timer();
	Time* getTime();
	void start();
	void suspend();
	void reset();

private:
	uint32_t runtimeMs;
	uint32_t timeMs;
	Time time;

	const static uint16_t TR__MS_SEC = 1000;
	const static uint32_t TR__SEC_DD = 86400;
	const static uint16_t TR__SEC_HH = 3600;
	const static uint8_t TR__SEC_MM = 60;

	inline void update();
	inline void sample(boolean keep);
};


#endif /* TIMER_H_ */
