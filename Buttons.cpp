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

static Buttons* buttons;

static void _onISR() {
	buttons->onISR();
}

void buttons_setup(Buttons* btn) {
	buttons = btn;
	attachInterrupt(0, _onISR, CHANGE);
}

Buttons::Buttons() :
		pressMs(0) {
	buttonSetup(PIN_BUTTON_NEXT);
	buttonSetup(PIN_BUTTON_PREV);
}

void Buttons::buttonSetup(uint8_t pin) {
	pinMode(pin, INPUT);   // set Pin as Input (default)
	digitalWrite(pin, HIGH);  // enable pullup resistor
}

void Buttons::onISR() {
	uint32_t ms = millis();
	if (ms - pressMs < PRESS_MS) {
		return;
	}
	pressMs = ms;
	if (digitalRead(PIN_BUTTON_NEXT) == 0) {
#if LOG
		log(F("Next button pressed"));
#endif
		eb_fire(BUTTON_NEXT);
	} else if (digitalRead(PIN_BUTTON_PREV) == 0) {
#if LOG
		log(F("Prev button pressed"));
#endif
		eb_fire(BUTTON_PREV);
	}

}

