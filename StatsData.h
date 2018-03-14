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
#ifndef STATSDATA_H_
#define STATSDATA_H_

#include "Config.h"

typedef struct {
	int16_t avg;
	int16_t min;
	int16_t max;
	uint8_t day; // day number in history. 0 - now, 1 - yesterday, 2 - before yesterday, and so on.
} Temp;

typedef struct {
	uint8_t size;
	boolean full;
} DayHistory;

#endif /* STATSDATA_H_ */
