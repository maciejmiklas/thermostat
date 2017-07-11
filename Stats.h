/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef STATS_H_
#define STATS_H_

#include "Arduino.h"
#include "Timer.h"
#include "EventBus.h"
#include "Config.h"
#include "Service.h"
#include "TempSensor.h"
#include "ArdLog.h"

typedef struct {
	int16_t avg;
	int16_t min;
	int16_t max;
	uint8_t day; // day number in history. 0 - now, 1 - yesterday, 2 - before yesterday, and so on.
	uint8_t realyOnHH[RELAYS_AMOUNT];
	uint8_t realyOnMM[RELAYS_AMOUNT];
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
	boolean dit_hasNext();

	/** Go back in history to more recent day. After reaching "now" it rotates to the end of the history. */
	Temp* dit_prev();
	boolean dit_hasPrev();
	/** Resets iterator to day one (now). */
	void dit_reset();

	/** Amount of history entries. */
	uint8_t dit_size();

private:

	/** Take 12 probes per day to calculate agv/min/max per day*/
	const static uint8_t PROBES_PER_DAY = 12;

	/**
	 * Take probe every second hour - 12 probes gives us full day:
	 * #PROBES_PER_DAY * #DAY_PROBE_MS = 24h.
	 */
	const static uint32_t DAY_PROBE_MS = 1000; // 240000 = 1000 * 60 * 60 * 2;

	TempSensor* tempSensor;

	Timer systemTimer;
	Timer relayTimer[RELAYS_AMOUNT];

	/** FIFO Queue containing statistics for each day. Top of the queue has the oldest day, bottom most recent. */
	int16_t dayProbes[PROBES_PER_DAY];
	uint8_t dayProbeIdx;
	uint32_t lastProbeMs;

	Temp dayHistory[DAY_HISTORY_SIZE];
	uint8_t dayHistoryIdx;
	boolean dayHistoryFull;
	uint8_t dit_idx;
	Temp actualTemp;

	uint8_t deviceId();
	void init();
	void cycle();
	uint8_t listenerId();
	void onEvent(BusEvent event, va_list ap);
	inline void probeDayTemp();
	inline void probeActualTemp();
	inline void updateDayTemp(Temp* temp);
	/** Amount of history entries. */
	inline uint8_t _dit_size();
};

#endif /* STATS_H_ */
