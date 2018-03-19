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
#ifndef DOMAIN_H_
#define DOMAIN_H_

#include "Arduino.h"
#include "ArdLog.h"

void util_setup();
void util_cycle();

uint32_t util_ms();

inline void util_sort_i8(int8_t arr[], uint8_t size) {
	int8_t i, temp, j;
	for (i = 1; i < size; i++) {
		temp = arr[i];
		j = i - 1;
		while (temp < arr[j] && j > 0) {
			arr[j + 1] = arr[j];
			j = j - 1;
		}
		arr[j + 1] = temp;
	}
}

inline int8_t util_avg_i8(int8_t arr[], uint8_t size) {
	if (size == 0) {
		return 0;
	}
	int8_t tmp = 0;
	for (uint8_t i = 0; i < size; i++) {
		tmp += arr[i];
	}
	return tmp / size;
}

inline int8_t util_max_i8(int8_t arr[], uint8_t size) {
	if (size == 0) {
		return 0;
	}
	int8_t tmp = arr[0];
	for (uint8_t i = 1; i < size; i++) {
		int8_t next = arr[i];
		if (next > tmp) {
			tmp = next;
		}
	}
	return tmp;
}

inline int8_t util_min_i8(int8_t arr[], uint8_t size) {
	if (size == 0) {
		return 0;
	}
	int8_t tmp = arr[0];
	for (uint8_t i = 1; i < size; i++) {
		int8_t next = arr[i];
		if (next < tmp) {
			tmp = next;
		}
	}
	return tmp;
}

inline uint16_t util_freeRam() {
	extern int __heap_start, *__brkval;
	int v;
	return (uint16_t) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

inline uint16_t util_abs16(int8_t val) {
	return val > 0 ? val : val * -1;
}

inline uint8_t util_abs8(int8_t val) {
	return val > 0 ? val : val * -1;
}

#endif /* DOMAIN_H_ */
