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
	buttonSetup(BUTTON_NEXT_PIN);

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
	if (digitalRead(BUTTON_NEXT_PIN) == 0) {
#if LOG
		log(F("Next button pressed"));
#endif
//TODO
	}

}

