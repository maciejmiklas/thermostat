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
#include "EventBus.h"

const static uint8_t LISTNERS_MAX = 7;
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
		log(F("### TOO MANY LISTENERS (%d) ###"), listenersAmount);
#endif
		return;
	}

#if TRACE
	log(F("Register %d bus listener"), listenersAmount);
#endif
	listeners[listenersAmount++] = listener;
}

void eb_fire(BusEvent event, ...) {
#if LOG
	log(F("Fire event: %d"), event);
#endif

	for (uint8_t idx = 0; idx < listenersAmount; idx++) {
		va_list ap;
		va_start(ap, event);
#if TRACE
	log(F("Notify: %d with: %d"), idx, event);
#endif
		listeners[idx]->onEvent(event, ap);
		va_end(ap);
	}
}

boolean eb_inGroup(BusEvent event, BusEventGroup group) {
	boolean in = false;
	switch (group) {
	case RELAY:
		in = event >= 10 && event < 20;
		break;
	case BUTTON:
		in = event >= 20 && event < 30;
		break;
	case SERVICE:
		in = event >= 30 && event < 40;
		break;
	}

	return in;
}

