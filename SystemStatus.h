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
#ifndef SYSTEMSTATUS_H_
#define SYSTEMSTATUS_H_

#include "EventBus.h"
#include "Config.h"
#include "Util.h"

class SystemStatus: public BusListener {
public:
	SystemStatus();
	virtual ~SystemStatus();

private:
	/* LED on time for 3x long flash. */
	static const uint16_t SOS_ON_LONG_DURATION = 1200;

	/* LED off time for 3x long flash. */
	static const uint16_t SOS_OFF_LONG_DURATION = 300;

	/* LED on time for 3x short flash. */
	static const uint16_t SOS_ON_SHORT_DURATION = 150;

	/* LED off time for 3x short flash. */
	static const uint16_t SOS_OFF_SHORT_DURATION = 50;

	/** Pause time after single SOS message (3x long, 3x short). */
	static const uint16_t SOS_PAUSE_DONE = 3000;

	/* Pause time between 3x long and 3x short flash. */
	static const uint16_t SOS_PAUSE_MIDLE = 200;

	uint8_t state;
	uint32_t switchMs;
	uint8_t lastPinVal;
	boolean sosEnabled;

	void onEvent(BusEvent event, va_list ap);
	uint8_t listenerId();
	void sosOn();
	void sosOff();
	void doSwitch(uint16_t duration);
	inline void cycle();
};

#endif /* SYSTEMSTATUS_H_ */
