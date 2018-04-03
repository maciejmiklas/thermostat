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
#define ENABLE_TEST_UTIL false

#if ENABLE_TEST_UTIL

#include "Arduino.h"
#include "ArduinoUnit.h"
#include "Config.h"
#include "ArdLog.h"
#include "Storage.h"
#include "Util.h"
#include "TempSensor.h"
#include "TempStats.h"


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

	Serial.begin(SERIAL_SPEED);
	while (!Serial) {
	}
}

void loop() {
	Test::run();
}

#endif

