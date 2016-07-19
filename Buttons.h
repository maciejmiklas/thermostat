#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "Arduino.h"
#include "ArdLog.h"
#include "MainController.h"

class Buttons {
public:
	Buttons(MainController* mainController);
	void onISR();
private:
	const static uint8_t PRESS_MS = 100;
	const static uint8_t BUTTON_NEXT_PIN = 10;
	uint32_t pressMs;
	MainController* mainController;

	void buttonSetup(uint8_t pin);
};

void buttons_setup(Buttons* btn);

#endif /* BUTTONS_H_ */
