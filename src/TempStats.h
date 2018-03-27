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
#ifndef TEMPSTATS_H_
#define TEMPSTATS_H_

#include "Arduino.h"
#include "Timer.h"
#include "EventBus.h"
#include "Config.h"
#include "Service.h"
#include "TempSensor.h"
#include "ArdLog.h"
#include "Storage.h"
#include "StatsData.h"

class TempStats: public Service, public BusListener {
public:
	TempStats(TempSensor* tempSensor, Storage* storage);
	Temp* actual();

	/**
	 * Day history starts from now, and goes over past days: now, yesterday, before yesterday, ...., 33 days ago, ...
	 */
	class DayIteroator {
	public:
		DayIteroator(TempStats* ts);
		/** Go to next entry in history, meaning go back by one day. After reaching history limit dit rotates to "now". */
		Temp* next();

		boolean hasNext();

		/** Go back in history to more recent day. After reaching "now" dit rotates to the end of the history. */
		Temp* prev();

		boolean hasPrev();

		/** Resets iterator to day one (now). */
		void reset();

		/** Amount of history entries. */
		uint8_t size();
	private:
		TempStats* ts;
		Temp temp;
		uint8_t dIdx;

		inline void updateDayTemp(Temp* temp);
	};
	DayIteroator* di();
	void init();
private:
	typedef struct {
		/** FIFO Queue containing statistics for each day. Top of the queue has the oldest day, bottom most recent. */
		int8_t probes[ST_PROBES_PER_DAY];
		uint8_t probeIdx;
		uint32_t lastProbeMs;
	} DayProbe;

	typedef struct {
		uint32_t lastProbeMs;
		Temp temp; // actual temp, only min and max are updated.
	} ActualProbe;

	TempSensor* tempSensor;
	Storage* storage;
	DayIteroator _dit;
	DayIteroator* _ditp;
	DayProbe dp;
	ActualProbe ap;

	uint8_t deviceId();
	void clearStats();
	void cycle();
	uint8_t listenerId();
	void onEvent(BusEvent event, va_list ap);
	inline void probeDayTemp();
	inline void probeActualTemp();
	inline void initTemp(Temp* temp);
};

#endif /* TEMPSTATS_H_ */
