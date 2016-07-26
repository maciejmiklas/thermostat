#include "Buttons.h"

static Buttons* buttons;

static void _onISR() {
	buttons->onISR();
}

void buttons_setup(Buttons* btn) {
	buttons = btn;
	attachInterrupt(0, _onISR, CHANGE);
}

Buttons::Buttons() :
		pressMs(0) {
	buttonSetup(PIN_BUTTON_NEXT);
	buttonSetup(PIN_BUTTON_PREV);
}

void Buttons::buttonSetup(uint8_t pin) {
	pinMode(pin, INPUT);   // set Pin as Input (default)
	digitalWrite(pin, HIGH);  // enable pullup resistor
}

void Buttons::onISR() {
	uint32_t ms = millis();
	if (ms - pressMs < PRESS_MS) {
		return;
	}
	pressMs = ms;
	if (digitalRead(PIN_BUTTON_NEXT) == 0) {
#if LOG
		log(F("Next button pressed"));
#endif
		eb_fire(BUTTON_NEXT);
	} else if (digitalRead(PIN_BUTTON_PREV) == 0) {
#if LOG
		log(F("Prev button pressed"));
#endif
		eb_fire(BUTTON_PREV);
	}

}

