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
				&oneWire) {
	//TODO
	log(F("TS IN %d %d %d"), probes[0], probes[1], probes[2]);
}

int16_t TempSensor::getTemp() {
	//TODO
	DAY_CNT++;
	if (DAY_CNT > 7) {
		DAY_CNT = 0;
	}
	uint8_t temp1 = DAY + DAY_CNT;
	log(F("TS T %d"), temp1);
	return temp1;
	//return curentTemp;
}

int16_t TempSensor::getQuickTemp() {
	return lastTemp;
}

void TempSensor::init() {
	dallasTemperature.begin();
	curentTemp = readTemp();
}

void TempSensor::cycle() {
	uint32_t ms = util_ms();
	if (ms - lastProbeTime < PROBE_FREQ_MS) {
		return;
	}
	lastProbeTime = ms;
	int16_t temp = readTemp();
	lastTemp = temp;
	if (probeIdx == PROBES_SIZE) {
		util_sort_i16(probes, PROBES_SIZE);
		curentTemp = probes[PROBES_MED_IDX];
		probeIdx = 0;
#if TRACE_//TODO
		log(F("####### %d  #######"), DAY);//TODO
		log(F("TS CY %d->%d %d %d"), curentTemp, probes[0], probes[1], probes[2]);
#endif

	} else {
		probes[probeIdx++] = temp;
	}
}

uint8_t TempSensor::deviceId() {
	return DEVICE_ID_TEMP_SENSOR;
}

inline int16_t TempSensor::readTemp() {
	//TODO
	DAY_CNT++;
	if (DAY_CNT > 7) {
		DAY_CNT = 0;
	}
	uint8_t temp1 = DAY + DAY_CNT;
	log(F("TS T %d"), temp1);
	return temp1;

	dallasTemperature.requestTemperatures();
	int16_t temp = (int16_t) (dallasTemperature.getTempCByIndex(0) + 0.5);
#if USE_FEHRENHEIT
	temp = temp * 1.8 + 32;
#endif
	return temp;
}
