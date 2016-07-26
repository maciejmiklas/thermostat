#include "Thermostat.h"

static TempSensor* tempSensor;
static Stats stats;
static RelayDriver relayDriver(&tempSensor);
static Display display;
static ServiceSuspender mainController;
static Buttons buttons;

void setup() {
	log_setup();
	util_setup();
	buttons_setup(&buttons);
}

boolean sent = false;
void loop() {
	util_cycle();
	log_cycle();
	mainController.cycle();
	tempSensor.cycle();
	relayDriver.cycle();
	if(!sent){
		sent = true;
		eb_fire(BUTTON_NEXT);
	}
}
