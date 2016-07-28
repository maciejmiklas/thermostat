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

	void onEvent(BusEvent event, va_list ap);
	inline void showNextScreen();
	inline void showPrevScreen();
	inline void showStartScreen();
	inline void clcd(uint8_t row);
};

#endif /* DISPLAY_H_ */
