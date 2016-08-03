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
		relay1(ts, PIN_RELAY_1, THRESHOLD_RELAY_1), relay2(ts, PIN_RELAY_2, THRESHOLD_RELAY_2) {
}

void RelayDriver::onCycle() {
	driveRelay(&relay1, 0);
	driveRelay(&relay2, 1);
}

inline void RelayDriver::driveRelay(Relay* relay, uint8_t id) {
	boolean changed = relay->drive();
	if (changed) {
		eb_fire(relay->isEnabled() ? RELAY_ON : RELAY_OFF, id);
		delay(DELAY_AFTER_SWITCH_MS);
	}
}

uint8_t RelayDriver::deviceId() {
	return SERVICE_ID_RELAY_DRIVER;
}

