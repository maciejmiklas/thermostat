#ifndef STATS_H_
#define STATS_H_

#include "Arduino.h"
#include "Timer.h"
#include "EventBus.h"
#import "Config.h"

class Stats: public BusListener {
public:
	Stats();

private:
	void onEvent(BusEvent event, va_list ap);

	Timer systemTimer;
	Timer relayTimer[RELAYS_AMOUNT];
};

#endif /* STATS_H_ */
