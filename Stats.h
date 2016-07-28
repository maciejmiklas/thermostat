#ifndef STATS_H_
#define STATS_H_

#include "Arduino.h"
#include "Timer.h"
#include "EventBus.h"
#import "Config.h"
#import "Service.h"

class Stats: public Service, public BusListener {
public:
	Stats();
	Time* getRelayTime(uint8_t relayId);
	Time* getUpTime();

protected:
	uint8_t deviceId();

private:
	void onEvent(BusEvent event, va_list ap);
	void onCycle();
	Timer systemTimer;
	Timer relayTimer[RELAYS_AMOUNT];
};

#endif /* STATS_H_ */
