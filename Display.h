#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "EventBus.h"
#include "ArdLog.h"
#include "LiquidCrystal.h"

class Display: public BusListener {
public:
	Display();

private:
	LiquidCrystal lcd;

	void onEvent(BusEvent event, va_list ap);
	inline void showNextScreen();
	inline void showPrevScreen();
	inline void showStartScreen();
	inline void clcd(uint8_t row);
};

#endif /* DISPLAY_H_ */
