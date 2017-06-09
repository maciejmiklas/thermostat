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
#include "Timer.h"

Timer::Timer() :
		runtimeMs(0), timeMs(0), time(), running(false) {
}

void Timer::start() {
	running = true;
	sample();
}

void Timer::suspend() {
	running = false;
	sample();
	update();

#if TRACE
	log(F("TI suspend %lu"), timeMs);
#endif
}

Time* Timer::getTime() {
	if (running) {
		sample();
		update();
	}
	return &time;
}

inline void Timer::update() {
	uint32_t sec = timeMs / TR__MS_SEC;

	// days
	time.dd = sec / TR__SEC_DD;
	uint32_t tmp = sec - time.dd * TR__SEC_DD;

	// hours of a day
	time.hh = tmp / TR__SEC_HH;
	tmp -= time.hh * TR__SEC_HH;

	// minutes of an hour
	time.mm = tmp / TR__SEC_MM;
	tmp -= time.mm * TR__SEC_MM;

	// seconds of a minute
	time.ss = tmp;
}

inline void Timer::sample() {
	uint32_t ms = util_millis();
	if (runtimeMs > 0) {
		timeMs += ms - runtimeMs;
	}
	runtimeMs = ms;
}
