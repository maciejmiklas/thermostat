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
#include "TempStats.h"

TempStats::TempStats(TempSensor* tempSensor, Storage* storage) :
		tempSensor(tempSensor), storage(storage),_dit(this), _ditp(&_dit), dp( { { }, 0, 0 }) {
}

void TempStats::init() {
	initTemp(&ap.temp);
}

TempStats::DayIteroator* TempStats::di() {
	return _ditp;
}

void TempStats::clearStats() {
	storage->dh_clear();
	initTemp(&ap.temp);
}

inline void TempStats::initTemp(Temp* temp) {
	temp->day = 99;
	temp->avg = 99;
	temp->min = 99;
	temp->max = -99;
}

void TempStats::onEvent(BusEvent event, va_list ap) {
	if (event == BusEvent::CLEAR_STATS) {
		clearStats();
	}
}

uint8_t TempStats::listenerId() {
	return deviceId();
}

Temp* TempStats::actual() {
	return &ap.temp;
}

void TempStats::cycle() {
	probeActualTemp();
	probeDayTemp();
}

uint8_t TempStats::deviceId() {
	return DEVICE_ID_STATS;
}

void TempStats::probeActualTemp() {
	uint32_t ms = util_ms();
	if (ms - ap.lastProbeMs < ST_ACTUAL_PROBE_MS) {
		return;
	}
	ap.lastProbeMs = ms;

	int8_t actTemp = tempSensor->getTemp();
	ap.temp.min = min(ap.temp.min, actTemp);
	ap.temp.max = max(ap.temp.max, actTemp);

#if TRACE_//TODO
	log(F("TES ACT:%d,%d,%d"), actTemp, ap.temp.min, ap.temp.max);
#endif
}

void TempStats::probeDayTemp() {
	uint32_t ms = util_ms();
	if (ms - dp.lastProbeMs < ST_DAY_PROBE_MS) {
		return;
	}
	dp.lastProbeMs = ms;

	if (dp.probeIdx == ST_PROBES_PER_DAY) {
		Temp temp = { 0, 0, 0, 0 };
		temp.avg = util_avg_i8(dp.probes, ST_PROBES_PER_DAY);
		temp.min = util_min_i8(dp.probes, ST_PROBES_PER_DAY);
		temp.max = util_max_i8(dp.probes, ST_PROBES_PER_DAY);
		dp.probeIdx = 0;
		storage->dh_store(&temp);
	} else {
		int8_t temp = tempSensor->getTemp();
		dp.probes[dp.probeIdx++] = temp;
#if TRACE_//TODO
		log(F("TES DP[%d]=%d"), dp.probeIdx, temp);
#endif
	}
}

// ################################ DayIteroator ################################
TempStats::DayIteroator::DayIteroator(TempStats* ts) :
		ts(ts), dIdx(0) {
}

boolean TempStats::DayIteroator::hasNext() {
	boolean has = dIdx < ts->storage->dh_readDays();
	return has;
}

boolean TempStats::DayIteroator::hasPrev() {
	boolean has = dIdx > 0;
	return has;
}

Temp* TempStats::DayIteroator::next() {
	ts->storage->dh_read(&temp, ++dIdx);
	updateDayTemp(&temp);
	return &temp;
}

Temp* TempStats::DayIteroator::prev() {
	ts->storage->dh_read(&temp, --dIdx);
	updateDayTemp(&temp);
	return &temp;
}

inline void TempStats::DayIteroator::updateDayTemp(Temp* temp) {
	temp->day = dIdx;
}

void TempStats::DayIteroator::reset() {
	dIdx = 0;
}

uint8_t TempStats::DayIteroator::size() {
	return ts->storage->dh_readDays();
}
// ##############################################################################
