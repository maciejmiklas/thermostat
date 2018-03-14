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
#include "Thermostat.h"

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

void setup() {
	//Serial.begin(SERIAL_SPEED);
	util_setup();
#if ENABLE_LOGGER
	log_setup();
#endif
	storage = new Storage();
	/*
	tempSensor = new TempSensor();
	tempStats = new TempStats(tempSensor, storage);
	relayDriver = new RelayDriver(tempSensor);
	serviceSuspender = new ServiceSuspender();
	systemStatus = new SystemStatus();
	//display = new Display(tempSensor, tempStats, timerStats, relayDriver);
	buttons = new Buttons();
	timerStats = new TimerStats();

	// init phase
	init(tempSensor);
	init(tempStats);
	init(relayDriver);
	//init(display);
	init(buttons);
	timerStats->init();
*/
	storage->dh_clear();
	log(F(">A> %d"), storage->dh_readDays());
	Temp* tt = new Temp();
	for (uint8_t i = 0; i < 60; i++) {
		tt->avg = i;
		tt->min = 10+i;
		tt->max = 20+i;
		storage->dh_store(tt);
	}


	log(F(">B> %d"), storage->dh_readDays());
	//Temp* tt = new Temp();
	for (uint8_t i = 0; i < 60; i++) {
		storage->dh_read(tt, i);
		//log(F(">C> %d %d %d %d %d"), i, tt->day, tt->min, tt->max, tt->avg);
	}

}

uint32_t lms = 0;
void loop() {
	util_cycle();
#if ENABLE_LOGGER
	log_cycle();
#endif
	// Hart beat
	//eb_fire(BusEvent::CYCLE);

	/*
	//TODO
	if (util_ms() - lms > 3000) {
		lms = util_ms();
		timerStats->getUpTime();
	}
	// TODO
	 */
	//Serial.println(util_freeRam());
}
