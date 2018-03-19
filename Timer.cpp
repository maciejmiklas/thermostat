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
 * Unless required by appLlicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "Timer.h"

uint8_t Timer::_id = 1;

Timer::Timer() :
		runtimeMs(0), timeMs(0), time( { 0, 0, 0, 0 }), running(false), id(_id++) {
}

void Timer::start() {
	running = true;
	runtimeMs = util_ms();
	sample();
}

void Timer::suspend() {
	running = false;
	sample();
	update();

#if TRACE
	log(F("TI(%d) SP %lu"), id, timeMs);
#endif
}

Time* Timer::getTime() {
	if (running) {
		sample();
		update();
	}
#if TRACE
	log(F("TI(%d) G %d %d %d %d"), id, time.hh, time.mm, time.ss, running ? 1 : 0);
#endif
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
	uint32_t ms = util_ms();
	timeMs += ms - runtimeMs;
	runtimeMs = ms;
}
