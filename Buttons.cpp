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

const static uint8_t PRESS_MS = 50;
const static uint8_t BUTTON_NONE_MASK = B00000000;
const static uint8_t BUTTON_NEXT_MASK = B00000001;
const static uint8_t BUTTON_PREV_MASK = B00000010;
const static uint8_t BUTTON_CLEAR_STATS_MASK = B00000011;

/* Mask to record pressed button. */
static volatile uint8_t butPressed = BUTTON_NONE_MASK;
static volatile uint32_t pressMs = 0;

static void onNextIRQ() {
	uint32_t ms = util_millis();
	if (ms - pressMs < PRESS_MS) {
		return;
	}
	pressMs = ms;

	butPressed = BUTTON_NEXT_MASK;
}

static void onPrevIRQ() {
	uint32_t ms = util_millis();
	if (ms - pressMs < PRESS_MS) {
		return;
	}
	pressMs = ms;

	butPressed = BUTTON_PREV_MASK;
}

static void onClearStatsIRQ() {
	uint32_t ms = util_millis();
	if (ms - pressMs < PRESS_MS) {
		return;
	}
	pressMs = ms;

	butPressed = BUTTON_CLEAR_STATS_MASK;
}

void Buttons::init() {
#if TRACE
	log(F("BT init"));
#endif

	setupButton(DIG_PIN_BUTTON_NEXT);
	setupButton(DIG_PIN_BUTTON_PREV);
	setupButton(DIG_PIN_BUTTON_RESET);

	attachInterrupt(digitalPinToInterrupt(DIG_PIN_BUTTON_NEXT), onNextIRQ, FALLING);
	attachInterrupt(digitalPinToInterrupt(DIG_PIN_BUTTON_PREV), onPrevIRQ, FALLING);
	attachInterrupt(digitalPinToInterrupt(DIG_PIN_BUTTON_CLEAR_STATS), onClearStatsIRQ, FALLING);
}

Buttons::Buttons() {
}

uint8_t Buttons::listenerId() {
	return LISTENER_ID_BUTTONS;
}

void inline Buttons::setupButton(uint8_t pin) {
	pinMode(pin, INPUT);

	// enable pull-up resistor
	digitalWrite(pin, HIGH);
}

inline void Buttons::cycle() {
	if (butPressed == BUTTON_NEXT_MASK) {
		eb_fire(BusEvent::BUTTON_NEXT);

	} else if (butPressed == BUTTON_PREV_MASK) {
		eb_fire(BusEvent::BUTTON_PREV);

	} else if (butPressed == BUTTON_CLEAR_STATS_MASK) {
		eb_fire(BusEvent::CLEAR_STATS);
	}
	butPressed = BUTTON_NONE_MASK;
}

void Buttons::onEvent(BusEvent event, va_list ap) {
	if (event == BusEvent::CYCLE) {
		cycle();
	}
}
