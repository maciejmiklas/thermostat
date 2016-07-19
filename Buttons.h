#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "Arduino.h"
#include "ArdLog.h"

void buttons_setup(Buttons* btn);

class Buttons {
public:
	Buttons();
	void onISR();
private:
	const static uint8_t PRESS_MS = 100;
	const static uint8_t BUTTON_NEXT_PIN = 3;
	uint32_t pressMs;

	void buttonSetup(uint8_t pin);
};

#endif /* BUTTONS_H_ */
