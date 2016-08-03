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
#ifndef TIMER_H_
#define TIMER_H_

#include "Arduino.h"
#include "Util.h"
#include "ArdLog.h"

typedef struct {
	uint16_t dd;
	uint8_t hh;
	uint8_t mm;
	uint8_t ss;
} Time;

class Timer {

public:
	Timer();
	Time* getTime();
	void start();
	void suspend();
	void reset();

private:
	uint32_t runtimeMs;
	uint32_t timeMs;
	Time time;

	const static uint16_t TR__MS_SEC = 1000;
	const static uint32_t TR__SEC_DD = 86400;
	const static uint16_t TR__SEC_HH = 3600;
	const static uint8_t TR__SEC_MM = 60;

	inline void update();
	inline void sample(boolean keep);
};


#endif /* TIMER_H_ */
