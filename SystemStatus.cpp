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
#include "SystemStatus.h"

SystemStatus::SystemStatus() :
		state(0), switchMs(0), lastPinVal(LOW), sosEnabled(false) {
	pinMode(DIG_PIN_SYSTEM_STATUS_LED, OUTPUT);
	sosOn();
}

SystemStatus::~SystemStatus() {
}

void SystemStatus::onEvent(BusEvent event, va_list ap) {
	if (event == BusEvent::SERVICE_SUSPEND) {
		sosOff();
	} else if (event == BusEvent::SERVICE_RESUME) {
		sosOn();
	}
}

void SystemStatus::sosOn() {
	sosEnabled = true;
}

void SystemStatus::sosOff() {
	if (sosEnabled) {
		state = 1;
		digitalWrite(DIG_PIN_SYSTEM_STATUS_LED, LOW);
		switchMs = util_millis();
	}
	sosEnabled = false;
}

void SystemStatus::doSwitch(uint16_t duration) {
	if (util_millis() - switchMs >= duration) {
		state++;
		switchMs = util_millis();
	}
}
// States:
// 1 - LED ON for long time
// 2 - LED OFF
// 3 - LED ON for long time
// 4 - LED OFF
// 5 - LED ON for long time
// 6 - LED OFF
// 7 - LED ON for short time
// 8 - LED OFF
// 9 - LED ON for short time
// 10 - LED OFF
// 11 - LED ON for short time
// 12 - LED OFF
void SystemStatus::cycle() {
	if (!sosEnabled) {
		return;
	}
	uint8_t pinVal = HIGH;

	switch (state) {

	// led off long duration
	case 2:
	case 4:
		doSwitch(SOS_OFF_LONG_DURATION);
		pinVal = LOW;
		break;

		// pause between 3x long and 3x short
	case 6:
		doSwitch(SOS_PAUSE_MIDLE);
		pinVal = LOW;
		break;

		// led off short duration
	case 8:
	case 10:
		doSwitch(SOS_OFF_SHORT_DURATION);
		pinVal = LOW;
		break;

		// led on for long time
	case 1:
	case 3:
	case 5:
		doSwitch(SOS_ON_LONG_DURATION);
		pinVal = HIGH;
		break;

		// led on for short time
	case 7:
	case 9:
	case 11:
		doSwitch(SOS_ON_SHORT_DURATION);
		pinVal = HIGH;
		break;

		// pause after whole SOS
	case 12:
		doSwitch(SOS_PAUSE_DONE);
		pinVal = LOW;
		break;

	default:
		state = 1;
		break;
	}

	if (lastPinVal != pinVal) {
		digitalWrite(DIG_PIN_SYSTEM_STATUS_LED, pinVal);
	}
	lastPinVal = pinVal;

}

