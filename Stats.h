#ifndef STATS_H_
#define STATS_H_

#include "Arduino.h"
#include "Timer.h"
#include "EventBus.h"
#import "Config.h"
#import "Service.h"
#import "TempSensor.h"
#import "ArdLog.h"

typedef struct {
	int8_t avg;
	int8_t min;
	int8_t max;
} Temp;

class Stats: public Service, public BusListener {
public:
	Stats(TempSensor* tempSensor);
	Time* getRelayTime(uint8_t relayId);
	Time* getUpTime();

protected:
	uint8_t deviceId();

private:
	const static uint8_t DAY_PROBES_AMOUNT = 24;
	const static uint32_t DAY_PROBE_MS = 1000 * 60 * 60;

	void onEvent(BusEvent event, va_list ap);
	void onCycle();
	inline void probeTemp();

	TempSensor* tempSensor;

	Timer systemTimer;
	Timer relayTimer[RELAYS_AMOUNT];

	int8_t dayProbes[DAY_PROBES_AMOUNT];
	uint8_t dayProbeIdx;
	uint32_t lastProbeMs;
};

#endif /* STATS_H_ */
