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
#include "TimerStats.h"

TimerStats::TimerStats(){
}

TimerStats::~TimerStats() {
}

void TimerStats::init() {
#if TRACE
	log(F("TSA init"));
#endif
	systemTimer.start();
}

uint8_t TimerStats::listenerId() {
	return DEVICE_ID_TIME_STATS;
}

// TODO store timers in EEPROM

void TimerStats::onEvent(BusEvent event, va_list ap) {
	if (eb_inGroup(event, BusEventGroup::RELAY)) {
		int relayId = va_arg(ap, int);

#if TRACE
		log(F("TSA RT:%d,%d"), relayId, event);
#endif
		if (event == BusEvent::RELAY_ON) {
			relayTimer[relayId].start();

		} else if (event == BusEvent::RELAY_OFF) {
			relayTimer[relayId].suspend();
		}
	} else if (event == BusEvent::CLEAR_STATS) {
		//	clearStats();
		//storage.clearDayHistory();
	}
}

Time* TimerStats::getRelayTime(uint8_t relayId) {
	Time* time = relayTimer[relayId].getTime();
#if LOG
	log(F("TSA RT(%d)%d:%d:%d"), relayId, time->hh, time->mm, time->ss);
#endif
	return time;
}

Time* TimerStats::getUpTime() {
	return systemTimer.getTime();
}
