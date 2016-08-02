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
	uint8_t day; // day number in history. 0 - now, 1 - yesterday, 2 - before yesterday, and so on.
} Temp;

class Stats: public Service, public BusListener {
public:
	Stats(TempSensor* tempSensor);
	Time* getRelayTime(uint8_t relayId);
	Time* getUpTime();

	Temp* getActual();

	// dit_xxx provides functions for Day Iterator.
	// History starts from now, and goes over past days: now, yesterday, before yesterday, ...., 33 days ago, ...

	/** Go to next entry in history, meaning go back by one day. After reaching history limit it rotates to "now". */
	Temp* dit_next();

	/** Go back in history to more recent day. After reaching "now" it rotates to the end of the history. */
	Temp* dit_prev();

	/** Resets iterator to day one (now). */
	void dit_reset();

	/** Amount of history entries. */
	uint8_t dit_size();

protected:
	uint8_t deviceId();

private:

	/** Take 24 probes per day to calculate agv/min/max per day*/
	const static uint8_t PROBES_PER_DAY = 24;

	/** Take probe every hour - 24 probes gives us full day. */
	const static uint32_t DAY_PROBE_MS = 3600000; // 1000 * 60 * 60;

	/** Keep history for last 60 days. */
	const static uint8_t DAY_HISTORY_SIZE = 60;
	const static uint8_t ACTUAL_PROBES_SIZE = 24;

	void onEvent(BusEvent event, va_list ap);
	void onCycle();
	inline void probeDayTemp();
	inline void probeActualTemp();
	inline void updateTempDay(Temp* temp);

	TempSensor* tempSensor;

	Timer systemTimer;
	Timer relayTimer[RELAYS_AMOUNT];

	/** FIFO Queue containing statistics for each day. Top of the queue has the oldest day, bottom most recent. */
	int8_t dayProbes[PROBES_PER_DAY];
	uint8_t dayProbeIdx;
	uint32_t lastProbeMs;

	Temp dayHistory[DAY_HISTORY_SIZE];
	uint8_t dayHistoryIdx;

	uint8_t dit_idx;

	int8_t actualProbes[ACTUAL_PROBES_SIZE];
	uint8_t actualProbesIdx;
	boolean actualProbesFull;
	Temp actualTemp;
};

#endif /* STATS_H_ */
