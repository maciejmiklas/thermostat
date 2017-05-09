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
#ifndef RELAY_H_
#define RELAY_H_

#include "Arduino.h"
#include "TempSensor.h"

class Relay {
public:
	Relay(TempSensor* ts, uint8_t pin, uint8_t tempSetPoint);
	boolean isOn();
	boolean drive();

	enum class State {
		OFF = 0, ON = 1
	};

private:
	TempSensor* const tempSensor;
	const uint8_t pin;
	const uint8_t tempSetPoint;
	boolean on;
	uint32_t lastSwitch;

	/** Prevents frequent switches of the relay. */
	const static uint32_t MIN_SWITCH_MS = 600.000;
};

#endif /* RELAY_H_ */
