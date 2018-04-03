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

#define ENABLE_TEST_STATS false

#if ENABLE_TEST_STATS

#include "Arduino.h"
#include "ArduinoUnit.h"
#include "Config.h"
#include "ArdLog.h"
#include "Storage.h"
#include "Util.h"
#include "TempSensor.h"
#include "TempStats.h"

class DummyTempSensor: public TempSensor {
public:
	int8_t temp;
	int8_t getTemp() {
		return temp;
	}
	int8_t getQuickTemp() {
		return temp;
	}
};

static Storage *storage;
static TempStats *tempStats;
static DummyTempSensor *tempSens;

static void createDayStats() {
	eb_fire(BusEvent::CLEAR_STATS);

	assertEqual(0, storage->dh_readDays());

	uint32_t ms = ST_DAY_PROBE_MS + 100;
	util_setCycleMs(ms);

	// first day
	for (uint8_t dd = 0; dd <= ST_PROBES_PER_DAY; dd++) {
		uint8_t mod = dd % 3;
		if (mod == 0) {
			tempSens->temp = -2;

		} else if (mod == 1) {
			tempSens->temp = 5;

		} else if (mod == 2) {
			tempSens->temp = 21;
		}

		util_setCycleMs(ms += ST_DAY_PROBE_MS + 100);
		eb_fire(BusEvent::CYCLE);
	}

	// second day
	for (uint8_t dd = 0; dd <= ST_PROBES_PER_DAY; dd++) {
		uint8_t mod = dd % 3;
		if (mod == 0) {
			tempSens->temp = 21;

		} else if (mod == 1) {
			tempSens->temp = 5;

		} else if (mod == 2) {
			tempSens->temp = 33;
		}

		util_setCycleMs(ms += ST_DAY_PROBE_MS + 100);
		eb_fire(BusEvent::CYCLE);
	}

	// third day
	for (uint8_t dd = 0; dd <= ST_PROBES_PER_DAY; dd++) {
		uint8_t mod = dd % 3;
		if (mod == 0) {
			tempSens->temp = 21;

		} else if (mod == 1) {
			tempSens->temp = 31;
		}

		util_setCycleMs(ms += ST_DAY_PROBE_MS + 100);
		eb_fire(BusEvent::CYCLE);
	}

	// fourth day
	for (uint8_t dd = 0; dd <= ST_PROBES_PER_DAY; dd++) {
		uint8_t mod = dd % 3;
		if (mod == 0) {
			tempSens->temp = 13;

		} else if (mod == 1) {
			tempSens->temp = 25;
		}

		util_setCycleMs(ms += ST_DAY_PROBE_MS + 100);
		eb_fire(BusEvent::CYCLE);
	}

	assertEqual(4, storage->dh_readDays());
}

static void testTempFirstDay(Temp *temp) {
	assertEqual(-2, temp->min);
	assertEqual(21, temp->max);
	assertEqual(8, temp->avg);
}

static void testTempSecondDay(Temp *temp) {
	assertEqual(5, temp->min);
	assertEqual(33, temp->max);
	assertEqual(20, temp->avg);
}

static void testTempThirdDay(Temp *temp) {
	assertEqual(21, temp->min);
	assertEqual(31, temp->max);
	assertEqual(28, temp->avg);
}

static void testTempFourthdDay(Temp *temp) {
	assertEqual(13, temp->min);
	assertEqual(25, temp->max);
	assertEqual(21, temp->avg);
}

static void testForward() {
	TempStats::DayIteroator* it = tempStats->di();

	assertEqual(true, it->hasNext());
	assertEqual(false, it->hasPrev());

	// last saved record
	Temp *temp = it->next();
	testTempFourthdDay(temp);
	//assertEqual(0, temp->day);

	// next
	assertEqual(true, it->hasNext());
	assertEqual(true, it->hasPrev());
	temp = it->next();
	testTempThirdDay(temp);

	// next
	assertEqual(true, it->hasNext());
	assertEqual(true, it->hasPrev());
	temp = it->next();
	testTempSecondDay(temp);

	// next
	assertEqual(true, it->hasNext());
	assertEqual(true, it->hasPrev());
	temp = it->next();
	testTempFirstDay(temp);

	assertEqual(false, it->hasNext());
	assertEqual(true, it->hasPrev());
}

static void testBackwards() {
	TempStats::DayIteroator* it = tempStats->di();

	assertEqual(false, it->hasNext());
	assertEqual(true, it->hasPrev());

	// last saved record
	Temp *temp = it->prev();
	testTempFirstDay(temp);

	// next
	assertEqual(true, it->hasPrev());
	temp = it->prev();
	testTempSecondDay(temp);

	// next
	assertEqual(true, it->hasPrev());
	assertEqual(true, it->hasNext());
	temp = it->prev();
	testTempThirdDay(temp);

	// next
	assertEqual(true, it->hasPrev());
	assertEqual(true, it->hasNext());
	temp = it->prev();
	testTempFourthdDay(temp);

	assertEqual(false, it->hasPrev());
	assertEqual(true, it->hasNext());
}

test(TempStats_it_loop) {
	createDayStats();
	for (uint8_t i = 0; i < 3; i++) {
		testForward();
		testBackwards();
	}
}

test(TempStats_it_backwards) {
	eb_fire(BusEvent::CLEAR_STATS);
	TempStats::DayIteroator* it = tempStats->di();
	createDayStats();

	for (uint8_t i = 0; i < 4; i++) {
		assertEqual(true, it->hasNext());
		it->next();
	}
	testBackwards();
}

test(TempStats_it_forward) {
	createDayStats();
	testForward();
}

test(TempStats_dayTemp) {
	createDayStats();
	Temp temp;

	// last saved record
	storage->dh_read(&temp, 3);
	testTempFirstDay(&temp);

	// next
	storage->dh_read(&temp, 2);
	testTempSecondDay(&temp);

	// next
	storage->dh_read(&temp, 1);
	testTempThirdDay(&temp);

	// next
	storage->dh_read(&temp, 0);
	testTempFourthdDay(&temp);
}

test(TempStats_actualTemp) {
	uint32_t ms = ST_ACTUAL_PROBE_MS + 100;
	util_setCycleMs(ms);
	eb_fire(BusEvent::CLEAR_STATS);

	// probe -13
	tempSens->temp = -13;
	eb_fire(BusEvent::CYCLE);
	Temp *at = tempStats->actual();
	assertEqual(-13, at->max);
	assertEqual(-13, at->min);

	// probe 21
	util_setCycleMs(ms += ST_ACTUAL_PROBE_MS + 100);
	tempSens->temp = 21;
	eb_fire(BusEvent::CYCLE);
	at = tempStats->actual();
	assertEqual(21, at->max);
	assertEqual(-13, at->min);
}

test(DummyTempSensor) {
	tempSens->temp = -13;
	assertEqual(-13, tempSens->getTemp());
	assertEqual(-13, tempSens->getQuickTemp());

	tempSens->temp = 72;
	assertEqual(72, tempSens->getTemp());
	assertEqual(72, tempSens->getQuickTemp());
}

void setup() {
#if ENABLE_LOGGER
	log_setup();
#endif

	storage = new Storage();
	tempSens = new DummyTempSensor();
	tempStats = new TempStats(tempSens, storage);

	tempStats->init();

	Serial.begin(SERIAL_SPEED);
	while (!Serial) {
	}
}

void loop() {
	Test::run();
}

#endif

