/*
3 * Licensed to the Apache Software Foundation (ASF) under one or more
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
#include "EventBus.h"

const static uint8_t LISTNERS_MAX = 8;
static BusListener* listeners[LISTNERS_MAX];
static uint8_t listenersAmount = 0;

BusListener::~BusListener() {
}

BusListener::BusListener() {
	eb_register(this);
}

void eb_register(BusListener* listener) {
	if (listenersAmount == LISTNERS_MAX) {
#if LOG
		log(F("EB LISTENERS ERR (%d) !"), listenersAmount);
#endif
		return;
	}

#if TRACE
	log(F("EB Reg %d"), listenersAmount);
#endif
	listeners[listenersAmount++] = listener;
}

void eb_fire(BusEvent event, ...) {
#if LOG
	if (event != BusEvent::CYCLE) {
		log(F("EB Fire: %d"), event);
	}
#endif

	for (uint8_t idx = 0; idx < listenersAmount; idx++) {
		va_list ap;
		va_start(ap, event);
#if TRACE
		if (event != BusEvent::CYCLE) {
			log(F("EB Notify: %d"), listeners[idx]->listenerId());
		}
#endif
		listeners[idx]->onEvent(event, ap);
		va_end(ap);
	}
}

boolean eb_inGroup(BusEvent eventEnum, BusEventGroup group) {
	boolean in = false;
	uint8_t event = static_cast<uint8_t>(eventEnum);
	switch (group) {
	case BusEventGroup::RELAY:
		in = event >= 10 && event < 20;
		break;
	case BusEventGroup::BUTTON:
		in = event >= 20 && event < 30;
		break;
	case BusEventGroup::SERVICE:
		in = event >= 30 && event < 40;
		break;
	}

	return in;
}

