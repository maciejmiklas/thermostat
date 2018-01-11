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
#include "RelayDriver.h"
#include "Initializable.h"

class Display: public BusListener, public Initializable {
public:
	Display(TempSensor* tempSensor, Stats* stats, RelayDriver* relayDriver);
	uint8_t listenerId();
private:

	enum DisplayStates {
		STATE_MAIN = 0, STATE_RUNTIME = 1, STATE_RELAY_TIME = 2, STATE_DAY_STATS = 3, STATE_CLEAR_STATS = 4
	};

	class DisplayState: public StateMachine {
	public:
		DisplayState(Display* display);
		virtual ~DisplayState();
	protected:
		inline boolean shouldUpdate();
		Display* display;
		virtual void init();
	private:
		const static uint16_t UPDATE_FREQ = 500;
		uint32_t lastUpdateMs;
	};

	/** Shows main/start screen */
	class MainState: public DisplayState {
	public:
		MainState(Display* display);
		virtual ~MainState();
		virtual uint8_t execute(BusEvent event);
	private:
		virtual void init();
		inline void update();
	};

	/** Shows runtime for system . */
	class RuntimeState: public DisplayState {
	public:
		RuntimeState(Display* display);
		virtual ~RuntimeState();
		virtual uint8_t execute(BusEvent event);
	private:
		virtual void init();
		inline void update();
	};

	/** Shows runtime for each relay. */
	class RelayTimeState: public DisplayState {
	public:
		RelayTimeState(Display* display);
		virtual ~RelayTimeState();
		virtual uint8_t execute(BusEvent event);
	private:
		virtual void init();
		uint8_t relayIdx;
		inline void updateDisplay();
		inline void updateDisplayTime();
	};

	/** Shows statistics for each last xx days. */
	class DayStatsState: public StateMachine {
	public:
		DayStatsState(Display* display);
		virtual ~DayStatsState();
		virtual uint8_t execute(BusEvent event);
	private:
		Display* display;
		uint8_t daySize;
		virtual void init();
		inline void updateDisplay(Temp* temp);

		const static uint32_t MS_HH = 3600000; // 1000 * 60 * 60
		const static uint32_t MS_MM = 60000; // 1000 * 60
		const static uint32_t MM_HH = 60;

		inline uint8_t getMM(uint32_t durationMs);
		inline uint8_t getHH(uint32_t durationMs);
	};

	/** Clears Statistics. */
	class ClearStatsState: public DisplayState {
	public:
		ClearStatsState(Display* display);
		virtual ~ClearStatsState();
		virtual uint8_t execute(BusEvent event);
	private:
		virtual void init();
		uint32_t showMs;
	};

	LiquidCrystal lcd;
	TempSensor* const tempSensor;
	Stats* const stats;
	RelayDriver* const relayDriver;
	const static uint8_t LINE_LENGTH = 16;

	// buffer has to be at lest 1 character larger than line due to terminating character.
	// we add few more bytes in case one string would be a bit longer than expected.
	char lcdBuf[LINE_LENGTH + 4];
	MainState mainState;
	RuntimeState runtimeState;
	RelayTimeState relayTimeState;
	DayStatsState dayStatsState;
	ClearStatsState clearStatsState;
	MachineDriver driver;

	void init();
	void onEvent(BusEvent event, va_list ap);
	inline void clrow(uint8_t row);
	inline void println(uint8_t row, const char *fmt, ...);
	inline void printlnNa(uint8_t row, const char *fmt);
	inline void lcdBufClRight(uint8_t from);
	void printTime(uint8_t row, Time* time);
};

#endif /* DISPLAY_H_ */
