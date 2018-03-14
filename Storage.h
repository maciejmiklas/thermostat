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
#ifndef STORAGE_H_
#define STORAGE_H_

#include "Arduino.h"
#include "StatsData.h"
#include "EEPROM.h"
#include "ArdLog.h"
#include "Config.h"

/**
 * Days in the history are being stored as FIFO, starting form 0 up to ST_DAY_HISTORY_SIZE.
 *
 */
class Storage {
public:
	Storage();
	virtual ~Storage();

	// idx starts from 0

	// FIFO queue: stores given temp at 0, and shifts already stored data to the right.
	void dh_store(Temp* temp);

	uint8_t dh_readDays();

	/* Most recent day is on #dIdx = 0, yesterday on #dIdx = 1, oldest day is at #dIdx = # dh_readDays() */
	void dh_read(Temp* dayTemp, uint8_t dIdx);

	void dh_clear();

private:

	// eIdx - index in EEPROM, starting from 0, each byte is given by this position.
	// dIdx - day index in history, starting from 0 until ST_DAY_HISTORY_SIZE

	/* amount of days in history, max: dh_size = EIDX_DAYS */
	uint8_t dh_days;

	// EIDX_XX - static data at the beginning of the EEPROM
	const static uint8_t EIDX_INIT_BYTE = 0;
	const static uint8_t EIDX_DAYS = 1;

	/* amount of static data written at the beginning of EEPROM */
	const static uint8_t EIDX_SIZE = 2;

	/** Amount of bytes taken by one temperature (Temp) entry */
	const static uint8_t TEMP_SIZE = 3;
	const static uint8_t STORAGE_BYTES = EIDX_SIZE + TEMP_SIZE * ST_DAY_HISTORY_SIZE;

	/* first byte of EEPROM indicating that it has been already initialised */
	const static uint8_t INIT_BYTE = 124;

	/* starting index of EEPROM for given day, counting from 0. */
	inline uint8_t dh_eIdx(uint8_t dIdx);
	inline uint8_t dh_eRead(Temp* temp, uint8_t eIdx);
	inline uint8_t dh_eStore(Temp* temp, uint8_t eIdx);

	/*
	 * shift day stats in EEPROM to the right, so that last day get's removed and most recent day is
	 * being moved from idx 0 to idx 1
	 */
	inline void dh_dShift();

};

#endif /* STORAGE_H_ */
