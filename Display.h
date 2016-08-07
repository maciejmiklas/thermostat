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
#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "EventBus.h"
#include "ArdLog.h"
#include "LiquidCrystal.h"
#include "TempSensor.h"
#include "Stats.h"
#include "Config.h"
#include "StateMachine.h"
#include "MachineDriver.h"

class Display: public BusListener {
public:
	Display(TempSensor* tempSensor, Stats* stats);
	void cycle();

private:

	enum DisplayStates {
		STATE_MAIN = 0, STATE_RUNTIME = 1, STATE_RELAY_TIME = 2, STATE_DAY_STATS = 3
	};

	/** Shows main/start screen */
	class MainState: public StateMachine {
	public:
		MainState(Display* display);
		virtual ~MainState();
		virtual uint8_t execute(BusEvent event);
		virtual void init();
	private:
		inline void update();

		const static uint16_t UPDATE_FREQ = 1000;
		Display* display;
		uint32_t lastUpdateMs;
	};

	/** Shows runtime for system . */
	class RuntimeState: public StateMachine {
	public:
		RuntimeState(Display* display);
		virtual ~RuntimeState();
		virtual uint8_t execute(BusEvent event);
		virtual void init();
	private:
		Display* display;
	};

	/** Shows runtime for each relay. */
	class RelayTimeState: public StateMachine {
	public:
		RelayTimeState(Display* display);
		virtual ~RelayTimeState();
		virtual uint8_t execute(BusEvent event);
		virtual void init();
	private:
		Display* display;
		uint8_t relayIdx;
		inline void updateDisplay();
	};

	/** Shows statistics for each last xx days. */
	class DayStatsState: public StateMachine {
	public:
		DayStatsState(Display* display);
		virtual ~DayStatsState();
		virtual uint8_t execute(BusEvent event);
		virtual void init();
	private:
		Display* display;
		uint8_t daySize;
		boolean showedInfo;
		inline void updateDisplay(Temp* temp);
		void showInfo();
	};

	LiquidCrystal lcd;
	TempSensor* const tempSensor;
	Stats* const stats;
	const static uint8_t LINE_LENGTH = 16;
	char lcdBuf[LINE_LENGTH + 1];
	MainState mainState;
	RuntimeState runtimeState;
	RelayTimeState relayTimeState;
	DayStatsState dayStatsState;
	MachineDriver driver;

	void onEvent(BusEvent event, va_list ap);
	inline void clcd(uint8_t row);
	inline void println(uint8_t row, const char *fmt, ...);
	inline void printlnNa(uint8_t row, const char *fmt);
	inline void cleanRight(char *array, short from, short size);
	void printTime(uint8_t row, Time* time);
};

#endif /* DISPLAY_H_ */
