#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "Arduino.h"
#include "ArdLog.h"
#include "EventBus.h"
#include "Config.h"

class Buttons {
public:
	Buttons();
	void onISR();
private:
	const static uint8_t PRESS_MS = 100;
	uint32_t pressMs;
	void buttonSetup(uint8_t pin);
};

void buttons_setup(Buttons* btn);

#endif /* BUTTONS_H_ */
