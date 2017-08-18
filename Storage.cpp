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

Storage::Storage() {
}

Storage::~Storage() {
}

void Storage::storeStats(StatsHistory* history) {
	uint8_t eIdx = 0;

	EEPROM.write(eIdx++, history->dayHistoryIdx);
	EEPROM.write(eIdx++, history->dayHistoryFull ? 1 : 0);

	for (uint8_t hIdx = 0; hIdx < history->dayHistoryIdx; hIdx++) {
		Temp temp = history->dayHistory[hIdx];
		EEPROM.write(eIdx++, temp.avg);
		EEPROM.write(eIdx++, temp.min);
		EEPROM.write(eIdx++, temp.max);
		EEPROM.write(eIdx++, temp.day);
	}
}
void Storage::clear() {
	for (uint8_t i = 0; i < EEPROM.length(); i++) {
		EEPROM.write(i, 0);
	}
}
void Storage::readStats(StatsHistory* history) {
	if (EEPROM.length() == 0) {
		return;
	}

	uint8_t eIdx = 0;

	// dayHistoryIdx
	uint8_t _dayHistoryIdx = EEPROM.read(eIdx++);
	if (_dayHistoryIdx > DAY_HISTORY_SIZE || _dayHistoryIdx == 0) {
#if TRACE
		log(F("ST HIST ERR"));
#endif
		return;
	}
	history->dayHistoryIdx = _dayHistoryIdx;

	// dayHistoryFull
	uint8_t _dayHistoryFull = EEPROM.read(eIdx++);
	if (_dayHistoryFull != 1 && _dayHistoryFull != 0) {
#if TRACE
		log(F("ST HF ERR"));
#endif
		return;
	}
	history->dayHistoryFull = _dayHistoryFull == 1;

	for (uint8_t hIdx = 0; hIdx < _dayHistoryIdx; hIdx++) {
		Temp temp = history->dayHistory[hIdx];
		temp.avg = EEPROM.read(eIdx++);
		temp.min = EEPROM.read(eIdx++);
		temp.max = EEPROM.read(eIdx++);
		temp.day = EEPROM.read(eIdx++);
	}
}

