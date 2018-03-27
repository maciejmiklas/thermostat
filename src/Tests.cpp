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
#include "Arduino.h"
#include "ArduinoUnit.h"
#include "Config.h"
#include "ArdLog.h"
#include "Storage.h"
#include "Util.h"
#include "TempSensor.h"
#include "TempStats.h"

const static uint8_t STORAGE_DAYS = 10;

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

test(TempStats_dayTemp) {
	uint32_t ms = ST_DAY_PROBE_MS + 100;
	util_setCycleMs(ms);
	eb_fire(BusEvent::CLEAR_STATS);

	assertEqual(0, storage->dh_readDays());

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

	//test
	assertEqual(3, storage->dh_readDays());
	Temp temp;

	// first day (last one saved)
	storage->dh_read(&temp, 2);
	assertEqual(-2, temp.min);
	assertEqual(21, temp.max);
	assertEqual(8, temp.avg);

	// second day
	storage->dh_read(&temp, 1);
	assertEqual(5, temp.min);
	assertEqual(33, temp.max);
	assertEqual(20, temp.avg);

	// third day
	storage->dh_read(&temp, 0);
	assertEqual(21, temp.min);
	assertEqual(31, temp.max);
	assertEqual(28, temp.avg);
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
	util_setCycleMs(ms += ST_ACTUAL_PROBE_MS+100);
	tempSens->temp = 21;
	eb_fire(BusEvent::CYCLE);
	at = tempStats->actual();
	assertEqual(21, at->max);
	assertEqual(-13, at->min);
}

test(storage) {
	Temp temp;

	// init storage
	storage->dh_clear();
	for (uint8_t i = 0; i < STORAGE_DAYS; i++) {
		temp.avg = i;
		temp.min = 10 + i;
		temp.max = 20 + i;
		storage->dh_store(&temp);
	}

	// read data
	uint8_t tempCnt = STORAGE_DAYS - 1;
	assertEqual(storage->dh_readDays(), STORAGE_DAYS);
	for (uint8_t i = 0; i < STORAGE_DAYS; i++) {
		storage->dh_read(&temp, i);
		assertEqual(tempCnt, temp.avg);
		assertEqual(10 + tempCnt, temp.min);
		assertEqual(20 + tempCnt, temp.max);
		tempCnt--;
	}
}

test(DummyTempSensor) {
	tempSens->temp = -13;
	assertEqual(-13, tempSens->getTemp());
	assertEqual(-13, tempSens->getQuickTemp());

	tempSens->temp = 72;
	assertEqual(72, tempSens->getTemp());
	assertEqual(72, tempSens->getQuickTemp());
}

test(util_abs8) {
	assertEqual(25, util_abs8(-25));
	assertEqual(25, util_abs8(25));
}

test(util_abs16) {
	assertEqual(2500, util_abs16(-2500));
	assertEqual(2500, util_abs16(2500));
}

test(util_min_i8) {
	int8_t arr[5];

	arr[0] = 25;
	arr[1] = -1;
	arr[2] = 14;
	arr[3] = -16;
	arr[4] = 2;

	int8_t val = util_min_i8(arr, 5);
	assertEqual(-16, val);
}

test(util_max_i8) {
	int8_t arr[5];

	arr[0] = 25;
	arr[1] = -1;
	arr[2] = 14;
	arr[3] = -16;
	arr[4] = 2;

	int8_t val = util_max_i8(arr, 5);
	assertEqual(25, val);
}

test(util_avg_i8) {
	int8_t arr[5];

	arr[0] = 25;
	arr[1] = -1;
	arr[1] = -1;
	arr[1] = -1;
	arr[2] = 14;
	arr[3] = -16;
	arr[4] = 2;

	int8_t val = util_avg_i8(arr, 5);
	assertEqual(5, val);
}

test(util_sort_i8_t1) {
	int8_t arr[5];

	arr[0] = 25;
	arr[1] = -1;
	arr[2] = 14;
	arr[3] = -16;
	arr[4] = 2;

	util_sort_i8(arr, 5);

	assertEqual(-16, arr[0]);
	assertEqual(-1, arr[1]);
	assertEqual(2, arr[2]);
	assertEqual(14, arr[3]);
	assertEqual(25, arr[4]);
}

test(util_sort_i8_t2) {
	int8_t arr[2];

	arr[0] = 25;
	arr[1] = 2;

	util_sort_i8(arr, 2);

	assertEqual(2, arr[0]);
	assertEqual(25, arr[1]);
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

