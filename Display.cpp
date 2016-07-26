#include "Display.h"

Display::Display() {
}

void Display::onEvent(BusEvent event, va_list ap) {
	if (event == BUTTON_NEXT) {
		showNextScreen();

	} else if (event == BUTTON_PREV) {
		showPrevScreen();

	} else if (event == SERVICE_RESUME) {
		showStartScreen();
	}
}

inline void Display::showPrevScreen() {
}

inline void Display::showNextScreen() {
}

inline void Display::showStartScreen() {
}

