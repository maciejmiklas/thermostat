#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "EventBus.h"
#include "ArdLog.h"
#include "LiquidCrystal.h"
#include "TempSensor.h"
#include "Stats.h"

class Display: public BusListener {
public:
	Display(TempSensor* tempSensor, Stats* stats);

private:
	LiquidCrystal lcd;
	TempSensor* const tempSensor;
	Stats* const stats;
	const static uint8_t LINE_LENGTH = 16;
	char lcdBuf[LINE_LENGTH + 1];
	uint8_t screenId;

	void onEvent(BusEvent event, va_list ap);
	inline void showNextScreen();
	inline void showPrevScreen();
	inline void showStartScreen();
	inline void clcd(uint8_t row);
	inline void println(uint8_t row, const char *fmt, ...);
	inline void cleanRight(char *array, short from, short size);
	inline void showScreenById(uint8_t screenId);
	inline void showScreen_systemOnTime();
};

#endif /* DISPLAY_H_ */
