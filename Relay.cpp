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
#include "Relay.h"

Relay::Relay(TempSensor* ts, uint8_t pin, uint8_t threshold) :
		tempSensor(ts), pin(pin), threshold(threshold), on(false) {
	lastSwitch = 0;
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
}

boolean Relay::isOn() {
	return on;
}

boolean Relay::drive() {
	uint32_t millis = util_millis();
	if (millis - lastSwitch < MIN_SWITCH_MS) {
		return false;
	}
	lastSwitch = millis;

	boolean changed = false;
	uint8_t temp = tempSensor->getTemp();
	if (on && temp < threshold) {
		digitalWrite(pin, LOW);
		on = false;
		changed = true;
#if LOG
		log(F("Relay %d OFF by %d deg"), pin, temp);
#endif
	} else if (!on && temp >= threshold) {
		digitalWrite(pin, HIGH);
		on = true;
		changed = true;
#if LOG
		log(F("Relay %d ON by %d deg"), pin, temp);
#endif
	}

	return changed;
}
