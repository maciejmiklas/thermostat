#ifndef MAINCONTROLLER_H_
#define MAINCONTROLLER_H_

#import "RelayDriver.h"
#import "Display.h"
#import "TempSensor.h"
#import "Util.h"

class MainController {
public:
	MainController(RelayDriver* relayDriver, Display* display, TempSensor* tempSensor);

	void onButtonNext();
	void cycle();

private:
	RelayDriver* relayDriver;
	Display* display;
	TempSensor* tempSensor;

	/** After button has been pressed suspend all services but display to provide fluent user feedback in GUI */
	const static uint16_t SUSPEND_SERVICE_MS = 60000;
	uint32_t suspendStart;
};

#endif /* MAINCONTROLLER_H_ */
