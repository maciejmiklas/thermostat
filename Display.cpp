#include "Display.h"

Display::Display(TempSensor* tempSensor, Stats* stats) :
		lcd(7, 6, 5, 4, 3, 2), tempSensor(tempSensor), stats(stats), screenId(0) {
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
	screenId--;
#if LOG
	log(F("Display - showPrevScreen: %d"), screenId);
#endif
	showScreenById(screenId);
}

inline void Display::showNextScreen() {
	screenId++;
#if LOG
	log(F("Display - showNextScreen: %d"), screenId);
#endif
	showScreenById(screenId);
}

inline void Display::showScreenById(uint8_t screenId) {

	switch (screenId) {
	case 1:
		showScreen_systemOnTime();
		break;
	default:
		screenId = 0;
		showStartScreen();
		break;
	}
}

inline void Display::showScreen_systemOnTime() {

}

inline void Display::showStartScreen() {
#if LOG
	log(F("Display - showStartScreen"));
#endif
	screenId = 0;

	// line 0
	println(0, "NOW|MIN|MAX|AVG");

	// line 1
	int8_t tempNow = tempSensor->getTemp();
	Temp* actual = stats->getActual();
	println(1, "%d|%d|%d|%d", tempNow, actual->min, actual->max, actual->avg);
}

inline void Display::println(uint8_t row, const char *fmt, ...) {
	lcd.setCursor(0, row);

	va_list va;
	va_start(va, fmt);
	short chars = vsprintf(lcdBuf, fmt, va);
	va_end(va);

	cleanRight(lcdBuf, chars, LINE_LENGTH);
	lcd.print(lcdBuf);
}

inline void Display::cleanRight(char *array, short from, short size) {
	for (short int i = from; i < size; i++) {
		array[i] = ' ';
	}
	array[size] = '\0';
}

inline void Display::clcd(uint8_t row) {
	lcd.setCursor(0, row);
	lcd.print("                ");
	lcd.setCursor(0, row);
}

