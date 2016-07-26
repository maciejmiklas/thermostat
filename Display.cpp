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
#if LOG
	log(F("Display - showPrevScreen"));
#endif
}

inline void Display::showNextScreen() {
#if LOG
	log(F("Display - showNextScreen"));
#endif
}

inline void Display::showStartScreen() {
#if LOG
	log(F("Display - showStartScreen"));
#endif
}

