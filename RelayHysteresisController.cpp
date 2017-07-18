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
#include "RelayHysteresisController.h"

RelayHysteresisController::RelayHysteresisController(TempSensor* ts, int16_t tempSetPoint) :
		RelayController(ts, tempSetPoint), lastSwitchMs(0), state(Relay::State::OFF) {
}

RelayHysteresisController::~RelayHysteresisController() {
}

Relay::State RelayHysteresisController::execute() {
	uint32_t millis = util_millis();

	if (lastSwitchMs != 0 && (millis - lastSwitchMs) < RHC_RELAY_MIN_SWITCH_MS) {
		return Relay::State::NO_CHANGE;
	}

	Relay::State newState = Relay::State::NO_CHANGE;
	int16_t temp = tempSensor->getQuickTemp();

	if (temp <= tempSetPoint) {
		newState = Relay::State::OFF;

	} else if (temp > tempSetPoint) {
		newState = Relay::State::ON;

	} else {
		newState = Relay::State::NO_CHANGE;
	}

	if (newState != state && newState != Relay::State::NO_CHANGE) {
		lastSwitchMs = millis;
		state = newState;
	}

	return state;
}
