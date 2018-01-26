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

class Storage {
public:
	Storage();
	virtual ~Storage();

	void storeStats(StatsHistory* history);
	void readStats(StatsHistory* history);
	void clear();

private:
	const static uint8_t STORAGE_BYTES = 2 + 3 * ST_DAY_HISTORY_SIZE;

};

#endif /* STORAGE_H_ */
