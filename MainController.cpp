#include "MainController.h"

MainController::MainController(RelayDriver* relayDriver, Display* display, TempSensor* tempSensor) :
		relayDriver(relayDriver), display(display), tempSensor(tempSensor), suspendStart(0) {
}

void MainController::onButtonNext() {
	if (suspendStart == 0) {
#if LOG
		log(F("Suspending services"));
#endif
		tempSensor->setEnabled(false);
		relayDriver->setEnabled(false);
	}

	suspendStart = util_millis();
	display->nextScreen();
}

void MainController::cycle() {
	tempSensor->cycle();
	relayDriver->cycle();

	if (suspendStart != 0 && util_millis() - suspendStart > SUSPEND_SERVICE_MS) {
#if LOG
		log(F("Resuming services"));
#endif
		tempSensor->setEnabled(true);
		relayDriver->setEnabled(true);
		display->reset();
		suspendStart = 0;
	}
}

