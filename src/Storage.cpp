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
#include "Storage.h"
#include "StatsData.h"

Storage::Storage() :
		dh_days(0) {
}

Storage::~Storage() {
}

inline uint8_t Storage::dh_eIdx(uint8_t dIdx) {
	uint8_t eIdx = EIDX_SIZE + dIdx * TEMP_SIZE;
	return eIdx;
}

inline void Storage::dh_dShift() {
	uint8_t minEidx = dh_eIdx(0);
	uint8_t maxEidx = dh_eIdx(dh_days) - 1;

#if LOG
	log(F("ST SD %d %d"), minEidx, maxEidx);
#endif

	for (uint8_t eIdx = maxEidx; eIdx >= minEidx; eIdx--) {
		uint8_t val = EEPROM.read(eIdx);
		EEPROM.write(eIdx + TEMP_SIZE, val);
	}
}

inline uint8_t Storage::dh_eRead(Temp* temp, uint8_t eIdx) {
	temp->avg = EEPROM.read(eIdx++);
	temp->min = EEPROM.read(eIdx++);
	temp->max = EEPROM.read(eIdx++);
	return eIdx;
}

inline uint8_t Storage::dh_eStore(Temp* temp, uint8_t eIdx) {
	EEPROM.write(eIdx++, temp->avg);
	EEPROM.write(eIdx++, temp->min);
	EEPROM.write(eIdx++, temp->max);
	return eIdx;
}

void Storage::dh_store(Temp* temp) {
#if LOG
	log(F("ST WD(%d) %d,%d,%d"), dh_days, temp->min, temp->max, temp->avg);
#endif
	DAY = dh_days * 10;
	DAY_CNT = 0;

	// shift days by one, so that position 0 is free
	dh_dShift();

	// increase days after shift, so that we do not shift empty day.
	EEPROM.write(EIDX_DAYS, ++dh_days);

	// store current day on 0
	uint8_t eIdx = dh_eIdx(0);
	dh_eStore(temp, eIdx);
}

uint8_t Storage::dh_readDays() {
	if (EEPROM.read(EIDX_INIT_BYTE) != INIT_BYTE) {
		dh_clear();
		return 0;
	}
	uint8_t size = EEPROM.read(EIDX_DAYS);

#if LOG
	log(F("ST DS %u"), size);
#endif
	return size;
}

void Storage::dh_read(Temp* temp, uint8_t dIdx) {
	uint8_t eIdx = dh_eIdx(dIdx);
	temp->avg = EEPROM.read(eIdx++);
	temp->min = EEPROM.read(eIdx++);
	temp->max = EEPROM.read(eIdx);

#if LOG
	log(F("ST RD %u,%u->%d,%d,%d"), dIdx, eIdx, temp->min, temp->max, temp->avg);
#endif
}

void Storage::dh_clear() {
#if LOG
	log(F("ST CLR"));
#endif
	EEPROM.write(EIDX_INIT_BYTE, INIT_BYTE);
	EEPROM.write(EIDX_DAYS, 0);
}

