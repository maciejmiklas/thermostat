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
#include "Buttons.h"

static volatile boolean pressIRQ = false;

//TODO try following:
// 1) read pin input in IRQ
// 2) in loop react to this input and reset it

static void onIRQ() {
	pressIRQ = true;
}

inline static void enableIRQ() {
#if LOG
	log(F("BT IRQ ON"));
#endif
	attachInterrupt(BUTTON_NEXT_IRQ, onIRQ, FALLING);
}

inline static void disableIRQ() {
#if LOG
	log(F("BT IRQ OFF"));
#endif
	detachInterrupt(BUTTON_NEXT_IRQ);
}

void Buttons::init() {
#if TRACE
	log(F("BT init"));
#endif

	setuButton(DIG_PIN_BUTTON_NEXT);
	setuButton(DIG_PIN_BUTTON_PREV);

	enableIRQ();
}

Buttons::Buttons() :
		pressMs(0) {
}

uint8_t Buttons::listenerId() {
	return LISTENER_ID_BUTTONS;
}

void inline Buttons::setuButton(uint8_t pin) {
	pinMode(pin, INPUT);

	// enable pull-up resistor
	digitalWrite(pin, HIGH);
}

inline void Buttons::cycle() {
	uint32_t ms = util_millis();
	if (ms - pressMs < PRESS_MS) {
		return;
	}
	pressMs = ms;

	// TODO IRQ processing makes no sense, because we are reading ping inputs within a loop
	if (pressIRQ) {
		pressIRQ = false;
		eb_fire(BusEvent::BUTTON_NEXT);
	}
	if (digitalRead(DIG_PIN_BUTTON_NEXT) == 0) {
		eb_fire(BusEvent::BUTTON_NEXT);

	} else if (digitalRead(DIG_PIN_BUTTON_PREV) == 0) {
		eb_fire(BusEvent::BUTTON_PREV);
	}
}

void Buttons::onEvent(BusEvent event, va_list ap) {
	if (event == BusEvent::CYCLE) {
		cycle();

	} else if (eb_inGroup(event, BusEventGroup::SERVICE)) {
		return;

		if (event == BusEvent::SERVICE_RESUME) {
			enableIRQ();

		} else if (event == BusEvent::SERVICE_SUSPEND) {
			disableIRQ();
		}
	}
}
