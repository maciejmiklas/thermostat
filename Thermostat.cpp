#include "Thermostat.h"

static TempSensor* tempSensor;
static Stats* stats;
static RelayDriver* relayDriver;
static Display* display;
static ServiceSuspender* serviceSuspender;
static Buttons* buttons;
void setup() {
	log_setup();
	util_setup();

	tempSensor = new TempSensor();
	stats = new Stats();
	relayDriver = new RelayDriver(tempSensor);
	display = new Display(tempSensor, stats);
	serviceSuspender = new ServiceSuspender();
	buttons = new Buttons();
	buttons_setup(buttons);
}

boolean sent = false;
void loop() {
	util_cycle();
	log_cycle();
	serviceSuspender->cycle();
	tempSensor->cycle();
	relayDriver->cycle();
	if(!sent){
		sent = true;
		eb_fire(BUTTON_NEXT);
	}
}
