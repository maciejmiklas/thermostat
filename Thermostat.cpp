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
static Stats* stats;
static RelayDriver* relayDriver;
static Display* display;
static ServiceSuspender* serviceSuspender;
static Buttons* buttons;

void setup() {
	log_setup();
	util_setup();

	tempSensor = new TempSensor();
	stats = new Stats(tempSensor);
	relayDriver = new RelayDriver(tempSensor);
	display = new Display(tempSensor, stats);
	serviceSuspender = new ServiceSuspender();
	buttons = new Buttons();
	buttons_setup(buttons);
}

void loop() {
	util_cycle();
	log_cycle();
	serviceSuspender->cycle();
	tempSensor->cycle();
	relayDriver->cycle();
	display->cycle();
}
