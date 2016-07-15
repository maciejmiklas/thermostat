#include "Thermostat.h"

TempSensor tempsensor;
StatsHolder stats;
RelayDriver relayDriver(&tempsensor, &stats);

void setup() {
	log_setup();
	util_setup();
}

void loop() {
	util_cycle();
	log_cycle();
	tempsensor.cycle();
	relayDriver.cycle();
}
