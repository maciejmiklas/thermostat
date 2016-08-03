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
#include "Display.h"

Display::Display(TempSensor* tempSensor, Stats* stats) :
		lcd(7, 6, 5, 4, 3, 2), tempSensor(tempSensor), stats(stats) {
	lcd.begin(16, 2);
	lcd.noAutoscroll();

	lcd.setCursor(0, 0);
}

void Display::onEvent(BusEvent event, va_list ap) {

}

inline void Display::println(uint8_t row, const char *fmt, ...) {
	lcd.setCursor(0, row);

	va_list va;
	va_start(va, fmt);
	short chars = vsprintf(lcdBuf, fmt, va);
	va_end(va);

	cleanRight(lcdBuf, chars, LINE_LENGTH);
	lcd.print(lcdBuf);
}

inline void Display::cleanRight(char *array, short from, short size) {
	for (short int i = from; i < size; i++) {
		array[i] = ' ';
	}
	array[size] = '\0';
}

inline void Display::clcd(uint8_t row) {
	lcd.setCursor(0, row);
	lcd.print("                ");
	lcd.setCursor(0, row);
}

// ##################### MainState #####################
Display::MainState::MainState(Display* display) :
		display(display), lastUpdateMs(0), lastTemp(-99) {
}

Display::MainState::~MainState() {
}

inline void Display::MainState::update() {
	int8_t tempNow = display->tempSensor->getTemp();
	if (lastTemp == tempNow) {
		return;
	}
	lastTemp = tempNow;
	Temp* actual = display->stats->getActual();
	display->println(1, "%d|%d|%d|%d", tempNow, actual->min, actual->max, actual->avg);
}

uint8_t Display::MainState::execute(BusEvent event) {
	if (event != NO_EVENT) {
		if (event == BUTTON_NEXT) {
			return STATE_RUNTIME;

		} else if (event == BUTTON_PREV) {
			return STATE_DAY_STATS;
		}
	}

	uint32_t millis = util_millis();
	if (millis - lastUpdateMs >= UPDATE_FREQ) {
		update();
		lastUpdateMs = millis;
	}

	return STATE_NOCHANGE;
}

void Display::MainState::init() {
#if LOG
	log(F("Display - showStartScreen"));
#endif

	display->println(0, "NOW|MIN|MAX|AVG");
	update();
}

void Display::MainState::reset() {
	lastUpdateMs = 0;
	lastTemp = -99;
}

// ##################### RuntimeState #####################
Display::RuntimeState::RuntimeState(Display* display) :
		display(display) {
}

Display::RuntimeState::~RuntimeState() {
}

uint8_t Display::RuntimeState::execute(BusEvent event) {
	return STATE_NOCHANGE;
}

void Display::RuntimeState::init() {
}

void Display::RuntimeState::reset() {
}

// ##################### DayStatsState #####################
Display::DayStatsState::DayStatsState(Display* display) :
		display(display) {
}

Display::DayStatsState::~DayStatsState() {
}

uint8_t Display::DayStatsState::execute(BusEvent event) {
	return STATE_NOCHANGE;
}

void Display::DayStatsState::init() {
}

void Display::DayStatsState::reset() {
}
