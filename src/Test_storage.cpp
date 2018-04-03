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

#define ENABLE_TEST_STORAGE false

#if ENABLE_TEST_STORAGE

#include "Arduino.h"
#include "ArduinoUnit.h"
#include "Config.h"
#include "ArdLog.h"
#include "Storage.h"
#include "Util.h"
#include "TempSensor.h"
#include "TempStats.h"


static Storage *storage;
const static uint8_t STORAGE_DAYS = 10;

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

void setup() {
#if ENABLE_LOGGER
	log_setup();
#endif

	storage = new Storage();

	Serial.begin(SERIAL_SPEED);
	while (!Serial) {
	}
}

void loop() {
	Test::run();
}

#endif
