#include "Thermostat.h"

TempSensor tempsensor;
StatsHolder stats;
RelayDriver relayDriver(&tempsensor, &stats);
Buttons buttons;

void setup() {
	log_setup();
	util_setup();
	buttons_setup(&buttons);
}

void loop() {
	util_cycle();
	log_cycle();
	tempsensor.cycle();
	relayDriver.cycle();
}
