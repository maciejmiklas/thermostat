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
#ifndef RELAYDRIVER_H_
#define RELAYDRIVER_H_

#include "Relay.h"
#include "RelayController.h"
#include "RelayHysteresisController.h"
#include "Arduino.h"

/**
 * We are supporting two fans: first will go on after reaching first temperature threshold, and second one after
 * reaching second threshold.
 */
class RelayDriver: public Service {
public:
	RelayDriver(TempSensor* ts);
	~RelayDriver();
	boolean isOn(uint8_t relayId);

protected:


private:
	typedef struct {
		Relay* relay;
		uint8_t pin;
		RelayController* controller;
	} RelayData;
	RelayData relays[RELAYS_AMOUNT];

	TempSensor* const tempSensor;

	inline void executeRelay(RelayData* rd, uint8_t id);

	/* In this method you can define your relay setup! */
	void init();
	uint8_t deviceId();
	void cycle();
};

#endif /* RELAYDRIVER_H_ */
