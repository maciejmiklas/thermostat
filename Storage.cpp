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

#if LOG
	log(F("ST SA %u"), history->dayHistoryIdx);
#endif

	EEPROM.write(eIdx++, history->dayHistoryIdx);
	EEPROM.write(eIdx++, history->dayHistoryFull ? 1 : 0);

	for (uint8_t hIdx = 0; hIdx < history->dayHistoryIdx; hIdx++) {
		Temp temp = history->dayHistory[hIdx];
		EEPROM.write(eIdx++, temp.avg);
		EEPROM.write(eIdx++, temp.min);
		EEPROM.write(eIdx++, temp.max);

#if LOG
		log(F("ST SD %u->%u %u %u"), hIdx, temp.avg, temp.min, temp.max);
#endif
	}
}
void Storage::clear() {
	for (uint8_t i = 0; i < STORAGE_BYTES; i++) {
		EEPROM.write(i, 0);
	}
}
void Storage::readStats(StatsHistory* history) {
	if (EEPROM.length() == 0) {
		return;
	}

	uint8_t eIdx = 0;
	uint8_t _dayHistoryIdx = EEPROM.read(eIdx++);
	uint8_t _dayHistoryFull = EEPROM.read(eIdx++);
#if LOG
	log(F("ST RD %u %u"), _dayHistoryIdx, _dayHistoryFull);
#endif

	// dayHistoryIdx
	if (_dayHistoryIdx > ST_DAY_HISTORY_SIZE || _dayHistoryIdx == 0) {
		return;
	}
	history->dayHistoryIdx = _dayHistoryIdx;

	// dayHistoryFull
	if (_dayHistoryFull != 1 && _dayHistoryFull != 0) {
		return;
	}
	history->dayHistoryFull = _dayHistoryFull == 1;

	for (uint8_t hIdx = 0; hIdx < _dayHistoryIdx; hIdx++) {
		Temp& temp = history->dayHistory[hIdx];
		temp.avg = EEPROM.read(eIdx++);
		temp.min = EEPROM.read(eIdx++);
		temp.max = EEPROM.read(eIdx++);
		temp.day = hIdx;

#if LOG
		log(F("ST RD %u->%u %u %u"), temp.day, temp.avg, temp.min, temp.max);
#endif
	}
}

