#include "Thermostat.h"

static TempSensor tempSensor;
static StatsHolder stats;
static RelayDriver relayDriver(&tempSensor, &stats);
static Display display;
static MainController mainController(&relayDriver, &display, &tempSensor);
static Buttons buttons(&mainController);

void setup() {
	log_setup();
	util_setup();
	buttons_setup(&buttons);
}

void loop() {
	util_cycle();
	log_cycle();
	mainController.cycle();
}
