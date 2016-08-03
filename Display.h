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
#include "StateMachine.h"

class Display: public BusListener {
public:
	Display(TempSensor* tempSensor, Stats* stats);

private:

	enum DisplayStates {
		STATE_MAIN = 0,
		STATE_RUNTIME = 1,
		STATE_DAY_STATS = 2
	};

	/** Shows main/start screen */
	class MainState: public StateMachine {
	public:
		MainState(Display* display);
		virtual ~MainState();
		virtual uint8_t execute(BusEvent event);
		virtual void init();
		virtual void reset();
	private:
		inline void update();

		const static uint16_t UPDATE_FREQ = 1000;
		uint32_t lastUpdateMs;
		Display* display;
		int8_t lastTemp;
	};

	/** Shows runtime for system and on/off time for each relay. */
	class RuntimeState: public StateMachine {
	public:
		RuntimeState(Display* display);
		virtual ~RuntimeState();
		virtual uint8_t execute(BusEvent event);
		virtual void init();
		virtual void reset();
	private:
		Display* display;
	};

	/** Shows statistics for each last xx days. */
	class DayStatsState: public StateMachine {
	public:
		DayStatsState(Display* display);
		virtual ~DayStatsState();
		virtual uint8_t execute(BusEvent event);
		virtual void init();
		virtual void reset();
	private:
		Display* display;
	};


	LiquidCrystal lcd;
	TempSensor* const tempSensor;
	Stats* const stats;
	const static uint8_t LINE_LENGTH = 16;
	char lcdBuf[LINE_LENGTH + 1];

	void onEvent(BusEvent event, va_list ap);
	inline void clcd(uint8_t row);
	inline void println(uint8_t row, const char *fmt, ...);
	inline void cleanRight(char *array, short from, short size);
};

#endif /* DISPLAY_H_ */
