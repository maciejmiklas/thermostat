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
#include "TempSensor.h"

TempSensor::TempSensor() :
		probeIdx(0), curentTemp(0), lastTemp(0), lastProbeTime(0), oneWire(DIG_PIN_TEMP_SENSOR), dallasTemperature(
				&oneWire), enabled(
		true) {
	dallasTemperature.begin();
	curentTemp = readTemp();
}

int16_t TempSensor::getTemp() {
	return curentTemp;
}

int16_t TempSensor::getQuickTemp() {
	return lastTemp;
}

void TempSensor::onCycle() {
	uint32_t millis = util_millis();
	if (!enabled || millis - lastProbeTime < PROBE_FREQ_MS) {
		return;
	}
	lastProbeTime = millis;
	int8_t temp = readTemp();
	lastTemp = temp;
	if (probeIdx == PROBES_SIZE) {
		util_sort_i8(probes, PROBES_SIZE);
		curentTemp = probes[PROBES_MED_IDX];
		probeIdx = 0;
#if TRACE
		log(F("Temp: %d, probes: %d %d %d %d %d %d"), temp, probes[0], probes[1], probes[2], probes[3], probes[4],
				probes[5]);
#endif

	} else {
		probes[probeIdx++] = temp;
	}
}

uint8_t TempSensor::deviceId() {
	return SERVICE_ID_TEMP_SENSOR;
}

inline int8_t TempSensor::readTemp() {
	dallasTemperature.requestTemperatures();
	return (int8_t) (dallasTemperature.getTempCByIndex(0) + 0.5);
}

// TODO support for farenheit
