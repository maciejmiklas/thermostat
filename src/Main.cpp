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
#include "Main.h"

static TempSensor* tempSensor;
static TempStats* tempStats;
static RelayDriver* relayDriver;
static Display* display;
static Storage* storage;
static ServiceSuspender* serviceSuspender;
static Buttons* buttons;
static SystemStatus* systemStatus;
static TimerStats* timerStats;

uint8_t DAY = 0;
uint8_t DAY_CNT = 0;

static void init(Initializable* ini) {
	ini->init();
}

static uint8_t days = 10; //TODO
Temp* tt;//TODO

static void writeStorage() {//TODO
	storage->dh_clear();
	for (uint8_t i = 0; i < days; i++) {
		tt->avg = i;
		tt->min = 10+i;
		tt->max = 20+i;
		storage->dh_store(tt);
	}
}

static void readStorage() {//TODO
	//log(F("###############"));
	//log(F(">B> %d"), storage->dh_readDays());
	//Temp* tt = new Temp();
	for (uint8_t i = 0; i < days; i++) {
		storage->dh_read(tt, i);
		//log(F(">C> %d %d %d %d %d"), i, tt->day, tt->min, tt->max, tt->avg);
	}
	//log(F("###############"));
}
void setup() {
	//Serial.begin(SERIAL_SPEED);
	util_setup();
#if ENABLE_LOGGER
	log_setup();
#endif


	int8_t probes[3] = {};
	probes[0] = 6;
	probes[1] = 0;
	probes[2] = 1;
	util_sort_i8(probes, 3);
	//log(F(">A> %d %d %d"), probes[0],probes[1],probes[2]);

	probes[0] = 1;
	probes[1] = 2;
	probes[2] = 3;
	util_sort_i8(probes, 3);
	//log(F(">B> %d %d %d"), probes[0],probes[1],probes[2]);

	probes[0] = 3;
	probes[1] = 1;
	probes[2] = 2;
	util_sort_i8(probes, 3);
	//log(F(">C> %d %d %d"), probes[0],probes[1],probes[2]);

	probes[0] = -1;
	probes[1] = 0;
	probes[2] = 21;
	util_sort_i8(probes, 3);
	//log(F(">D> %d %d %d"), probes[0],probes[1],probes[2]);

	probes[0] = 4;
	probes[1] = 6;
	probes[2] = 2;
	util_sort_i8(probes, 3);
	//log(F(">E> %d %d %d"), probes[0],probes[1],probes[2]);

	probes[0] = 0;
	probes[1] = -6;
	probes[2] = -11;
	util_sort_i8(probes, 3);
	//log(F(">F> %d %d %d"), probes[0],probes[1],probes[2]);

	tt = new Temp();//TODO
	storage = new Storage();
	tempSensor = new TempSensor();
	tempStats = new TempStats(tempSensor, storage);
	relayDriver = new RelayDriver(tempSensor);
	serviceSuspender = new ServiceSuspender();
	systemStatus = new SystemStatus();
	timerStats = new TimerStats();
	display = new Display(tempSensor, tempStats, timerStats, relayDriver);
	buttons = new Buttons();

	// init phase
	init(tempSensor);
	init(tempStats);
	init(relayDriver);
	init(display);
	init(buttons);
	timerStats->init();
	//writeStorage();
	//readStorage();
}

uint32_t lms = 0;
void loop() {
	util_cycle();
#if ENABLE_LOGGER
	log_cycle();
#endif

	eb_fire(BusEvent::CYCLE);

/*
	//TODO
	if (util_ms() - lms > 3000) {
		lms = util_ms();
		log_freeRAM("LOOP");
		timerStats->getUpTime();
	}
	// TODO
*/
	//Serial.println(util_freeRam());
}
