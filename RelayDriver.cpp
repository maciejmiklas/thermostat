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
#include "RelayDriver.h"

RelayDriver::RelayDriver(TempSensor* ts) :
		tempSensor(ts), lastSwitchMs(0) {
}

void RelayDriver::init() {
#if TRACE
	log(F("RD init"));
#endif
	relays[0].controller = new RelayHysteresisController(tempSensor, THRESHOLD_RELAY_0);
	relays[0].relay = new Relay(DIG_PIN_RELAY_0);
	relays[0].pin = DIG_PIN_RELAY_0;
	initRelayData(&relays[0]);

	relays[1].controller = new RelayHysteresisController(tempSensor, THRESHOLD_RELAY_1);
	relays[1].relay = new Relay(DIG_PIN_RELAY_1);
	relays[1].pin = DIG_PIN_RELAY_1;
	initRelayData(&relays[1]);
}

void RelayDriver::initRelayData(RelayData* val) {
	val->state =  Relay::State::OFF;
}

boolean RelayDriver::isOn(uint8_t relayId) {
	return relays[relayId].relay->getState() == Relay::State::ON;
}

void RelayDriver::cycle() {
	for (uint8_t i = 0; i < RELAYS_AMOUNT; i++) {
		executeRelay(i);
	}
}

inline void RelayDriver::executeRelay(uint8_t id) {
	RelayData& rd = relays[id];
	Relay::State state = rd.controller->execute();
	uint32_t time = util_millis();
	if (state == Relay::State::NO_CHANGE || state == rd.state || time - lastSwitchMs < RELAY_DELAY_AFTER_SWITCH_MS) {
		return;
	}
	lastSwitchMs = time;
	rd.state = state;
	rd.relay->onState(state);

	eb_fire(state == Relay::State::ON ? BusEvent::RELAY_ON : BusEvent::RELAY_OFF, id);
}

uint8_t RelayDriver::deviceId() {
	return DEVICE_ID_RELAY_DRIVER;
}

RelayDriver::~RelayDriver() {
	for (uint8_t i = 0; i < RELAYS_AMOUNT; i++) {
		delete (relays[i].controller);
		delete (relays[i].relay);
	}
}
