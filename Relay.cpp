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

Relay::Relay(uint8_t pin) :
		pin(pin), state(Relay::State::OFF) {
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
}

void Relay::execute(State st) {
	if (st == State::NO_CHANGE) {
		return;
	}
	state = st;
	if (st == State::ON) {
#if LOG
		log(F("Relay %d ON"), pin);
#endif
		digitalWrite(pin, HIGH);
	} else if (st == State::OFF) {

#if LOG
		log(F("Relay %d OFF"), pin);
#endif

		digitalWrite(pin, HIGH);
	}
}

Relay::State Relay::getState() {
	return state;
}
