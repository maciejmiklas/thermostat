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
#ifndef TEMPSENSOR_H_
#define TEMPSENSOR_H_

#include "ArdLog.h"
#include "Arduino.h"
#include "OneWire.h"
#include "Util.h"
#include "DallasTemperature.h"
#include "Service.h"
#include "Config.h"

class TempSensor: public Service {
public:
	TempSensor();
	int16_t getTemp();
	int16_t getQuickTemp();

protected:
	uint8_t deviceId();

private:
	/**
	 * We take #PROBES_SIZE probes from temp sensor, each one with delay of #PROBE_DELAY milliseconds.
	 * After collecting all required probes we calculate median and this is the temperature.
	 */
	const static uint8_t PROBES_SIZE = 6;
	const static uint8_t PROBES_MED_IDX = PROBES_SIZE / 2;
	const static uint32_t PROBE_FREQ_MS = 1000;

	int8_t probes[PROBES_SIZE];
	uint8_t probeIdx;
	int8_t curentTemp;
	int8_t lastTemp;
	uint32_t lastProbeTime;
	OneWire oneWire;
	DallasTemperature dallasTemperature;
	boolean enabled;

	inline int8_t readTemp();
	void onCycle();
};

#endif /* TEMPSENSOR_H_ */
