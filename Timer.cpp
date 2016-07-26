#include "Timer.h"

Timer::Timer() :
		runtimeMs(0), timeMs(0), time() {
}

void Timer::start() {
	sample(true);
}

void Timer::suspend() {
	sample(false);

#if LOG
	log(F("Suspended timer on %lu ms"), timeMs);
#endif
}

void Timer::reset() {
	timeMs = 0;
	runtimeMs = 0;
}

Time* Timer::getTime() {
	sample(true);
	update();
	return &time;
}

//long milis =  126000000 + 1440000 + 17000;// 1d 11h 24m 17s
//long milis =  345600000 + 1440000 + 17000;// 4d 0h 24m 17s
//long milis =  446400000 + 1440000 + 17000;// 5d 4h 24m 17s
inline void Timer::update() {
	uint32_t sec = timeMs / TR__MS_SEC;

	// days
	time.dd = sec / TR__SEC_DD;
	uint32_t tmp = sec - time.dd * TR__SEC_DD;

	// hours of a day
	time.hh = tmp / TR__SEC_HH;
	tmp -= time.hh * TR__SEC_HH;

	// minutes of an hour
	time.mm = tmp / TR__SEC_MM;
	tmp -= time.mm * TR__SEC_MM;

	// seconds of a minute
	time.ss = tmp;
}

inline void Timer::sample(boolean keep) {
	uint32_t ms = util_millis();
	if (runtimeMs > 0) {
		timeMs += ms - runtimeMs;
	}
	if (keep) {
		runtimeMs = ms;
	} else {
		runtimeMs = 0;
	}
}
