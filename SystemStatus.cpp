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
	if (event == BusEvent::CYCLE) {
		cycle();
	} else if (event == BusEvent::SERVICE_SUSPEND) {
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

uint8_t SystemStatus::listenerId() {
	return LISTENER_ID_STATUS;
}
void SystemStatus::doSwitch(uint16_t duration) {
	uint32_t ms = millis();
	if (ms - switchMs >= duration) {
		state++;
		switchMs = ms;
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

//   1 - '.' -> LED ON for SOS_ON_SHORT_DURATION
//   2 - ' ' -> LED OFF for SOS_OFF_DURATION
//   3 - '.' -> LED ON for SOS_ON_SHORT_DURATION
//   4 - ' ' -> LED OFF for SOS_OFF_DURATION
//   5 - '.' -> LED ON for SOS_ON_SHORT_DURATION
//   6 - ' ' -> LED OFF for SOS_OFF_DURATION
//
//   7 - '_' -> LED ON for SOS_ON_LONG_DURATION
//   8 - ' ' -> LED OFF for SOS_OFF_DURATION
//   9 - '_' -> LED ON for SOS_ON_LONG_DURATION
//  10 - ' ' -> LED OFF for SOS_OFF_DURATION
//  11 - '_' -> LED ON for SOS_ON_LONG_DURATION
//  12 - ' ' -> LED OFF for SOS_OFF_DURATION
//
//  13 - '.' -> LED ON for SOS_ON_SHORT_DURATION
//  14 - ' ' -> LED OFF for SOS_OFF_DURATION
//  15 - '.' -> LED ON for SOS_ON_SHORT_DURATION
//  16 - ' ' -> LED OFF for SOS_OFF_DURATION
//  17 - '.' -> LED ON for SOS_ON_SHORT_DURATION
//  18 - ' ' -> LED OFF for SOS_OFF_DURATION
//
//  19 - ' ' -> LED OFF for SOS_PAUSE_DONE
inline void SystemStatus::cycle() {
	if (!sosEnabled) {
		return;
	}
	uint8_t pinVal = HIGH;

	switch (state) {

	// '.' -> LED ON for SOS_ON_SHORT_DURATION
	case 1:
	case 3:
	case 5:
	case 13:
	case 15:
	case 17:
		doSwitch(SOS_ON_SHORT_DURATION);
		pinVal = HIGH;
		break;

	// '_' -> LED ON for SOS_ON_LONG_DURATION
	case 7:
	case 9:
	case 11:
		doSwitch(SOS_ON_LONG_DURATION);
		pinVal = HIGH;
		break;

	// ' ' -> LED OFF for SOS_OFF_DURATION
	case 2:
	case 4:
	case 6:
	case 8:
	case 10:
	case 12:
	case 14:
	case 16:
	case 18:
		doSwitch(SOS_OFF_DURATION);
		pinVal = LOW;
		break;

	case 19:
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


