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

Display::Display(TempSensor *tempSensor, Stats *stats, RelayDriver* relayDriver) :
		lcd(DIG_PIN_LCD_RS, DIG_PIN_LCD_ENABLE, DIG_PIN_LCD_D4, DIG_PIN_LCD_D5, DIG_PIN_LCD_D6, DIG_PIN_LCD_D7), tempSensor(
				tempSensor), stats(stats), relayDriver(relayDriver), mainState(this), runtimeState(this), relayTimeState(
				this), dayStatsState(this), driver(4, &mainState, &runtimeState, &relayTimeState, &dayStatsState) {
	lcd.begin(16, 2);
	lcd.noAutoscroll();

	lcd.setCursor(0, 0);
	driver.changeState(STATE_MAIN);
}

void Display::onEvent(BusEvent event, va_list ap) {
	if (event == BusEvent::SERVICE_RESUME) {
		driver.changeState(STATE_MAIN);
	}
	driver.execute(event);
}

inline void Display::printlnNa(uint8_t row, const char *fmt) {
	// TODO implement it without sprintf
	println(row, fmt);
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

void Display::printTime(uint8_t row, Time* time) {
	println(row, "%04d -> %02d:%02d:%02d", time->dd, time->hh, time->mm, time->ss);
}

// ##################### DisplayState #####################
Display::DisplayState::DisplayState(Display* display) :
		display(display), lastUpdateMs(0) {
}

Display::DisplayState::~DisplayState() {
}

boolean Display::DisplayState::shouldUpdate() {
	boolean should = false;
	uint32_t millis = util_millis();
	if (millis - lastUpdateMs >= UPDATE_FREQ) {
		should = true;
		lastUpdateMs = millis;
	}
	return should;
}

void Display::DisplayState::init() {
	lastUpdateMs = 0;
}

// ##################### MainState #####################
Display::MainState::MainState(Display* display) :
		DisplayState(display) {
}

Display::MainState::~MainState() {
}

inline void Display::MainState::update() {
	int8_t tempNow = display->tempSensor->getQuickTemp();
	Temp* actual = display->stats->getActual();
	display->println(1, "%3d|%3d|%3d|%3d", tempNow, actual->min, actual->max, actual->avg);
}

uint8_t Display::MainState::execute(BusEvent event) {
	if (event == BusEvent::CYCLE) {
		if (shouldUpdate()) {
			update();
		}
	} else {
		if (event == BusEvent::BUTTON_NEXT) {
			return STATE_RUNTIME;

		} else if (event == BusEvent::BUTTON_PREV) {
			return STATE_DAY_STATS;
		}
	}
	return STATE_NOCHANGE;
}

void Display::MainState::init() {
#if LOG
	log(F("Display - main state"));
#endif
	DisplayState::init();
	display->printlnNa(0, "NOW|MIN|MAX|AVG");
	update();
}

// ##################### RuntimeState #####################
Display::RuntimeState::RuntimeState(Display* display) :
		DisplayState(display) {
}

Display::RuntimeState::~RuntimeState() {
}

uint8_t Display::RuntimeState::execute(BusEvent event) {
	if (event != BusEvent::CYCLE) {
		if (event == BusEvent::BUTTON_NEXT) {
			return STATE_RELAY_TIME;

		} else if (event == BusEvent::BUTTON_PREV) {
			return STATE_MAIN;
		}
	}

	if (shouldUpdate()) {
		update();
	}

	return STATE_NOCHANGE;
}

inline void Display::RuntimeState::update() {
	display->printTime(1, display->stats->getUpTime());
}

void Display::RuntimeState::init() {
#if LOG
	log(F("Display - on time"));
#endif
	DisplayState::init();
	display->printlnNa(0, "System on time");
	update();
}

// ##################### RelayTimeState #####################
Display::RelayTimeState::RelayTimeState(Display* display) :
		DisplayState(display), relayIdx(0) {
}

Display::RelayTimeState::~RelayTimeState() {
}

uint8_t Display::RelayTimeState::execute(BusEvent event) {
	if (event == BusEvent::BUTTON_NEXT) {
		relayIdx++;
		if (relayIdx == RELAYS_AMOUNT) {
			return STATE_DAY_STATS;
		}
		updateDisplay();

	} else if (event == BusEvent::BUTTON_PREV) {
		// cannot decrease before checking because it's unsigned int
		if (relayIdx == 0) {
			return STATE_RUNTIME;
		}
		relayIdx--;
		updateDisplay();

	} else if (shouldUpdate()) {
		updateDisplayTime();
	}
	return STATE_NOCHANGE;
}

inline void Display::RelayTimeState::updateDisplay() {
	display->println(0, "Relay %d is %s", relayIdx + 1, (display->relayDriver->isOn(relayIdx) ? "on" : "off"));
	updateDisplayTime();
}

inline void Display::RelayTimeState::updateDisplayTime() {
	Time* rt = display->stats->getRelayTime(relayIdx);
	display->printTime(1, rt);
}

void Display::RelayTimeState::init() {
#if LOG
	log(F("Display - relay time"));
#endif
	DisplayState::init();
	relayIdx = 0;
	updateDisplay();
}

// ##################### DayStatsState #####################
Display::DayStatsState::DayStatsState(Display* display) :
		display(display), daySize(0), showedInfo(false) {
}

Display::DayStatsState::~DayStatsState() {
}

uint8_t Display::DayStatsState::execute(BusEvent event) {
	if (event == BusEvent::CYCLE) {
		return STATE_NOCHANGE;
	}
	if (event == BusEvent::BUTTON_NEXT) {
		if (daySize == 0) {
			return STATE_MAIN;
		}
		if (!showedInfo) {
			showedInfo = true;
			showInfo();
		} else {
			if (!display->stats->dit_hasNext()) {
				return STATE_MAIN;
			}
			updateDisplay(display->stats->dit_next());
		}
	} else if (event == BusEvent::BUTTON_PREV) {
		if (daySize == 0 || !showedInfo || !display->stats->dit_hasPrev()) {
			return STATE_RELAY_TIME;
		}
		updateDisplay(display->stats->dit_prev());
	}
	return STATE_NOCHANGE;
}

inline void Display::DayStatsState::updateDisplay(Temp* temp) {
	display->println(0, "%2d-> %2d|%2d|%2d", temp->day, temp->min, temp->max, temp->avg);

	// TODO support flexible amount of relays
	if (RELAYS_AMOUNT == 2) {
		display->println(1, "%2d:%2d, %2d:%2d", temp->realyOnHH[0], temp->realyOnMM[0], temp->realyOnHH[1],
				temp->realyOnMM[1]);
	}
}

void Display::DayStatsState::showInfo() {
	display->printlnNa(0, "DD-> MIN|MAX|AVG");
	display->printlnNa(1, "ON times[HH:MM,]");
}

void Display::DayStatsState::init() {
#if LOG
	log(F("Display - day stats"));
#endif
	showedInfo = false;
	display->stats->dit_reset();
	daySize = display->stats->dit_size();
	if (daySize == 0) {
		display->println(0, "Day statistics");
		display->println(1, "  is empty");
	} else {
		display->println(0, "Statistics");
		display->println(1, "  for %d days", daySize);
	}
}
