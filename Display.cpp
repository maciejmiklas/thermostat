#include "Display.h"

Display::Display(TempSensor* tempSensor, Stats* stats) :
		lcd(7, 6, 5, 4, 3, 2), tempSensor(tempSensor), stats(stats) {
	lcd.begin(16, 2);
	lcd.noAutoscroll();

	lcd.setCursor(0, 0);
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

inline void Display::clcd(uint8_t row) {
	lcd.setCursor(0, row);
	lcd.print("                ");
	lcd.setCursor(0, row);
}

