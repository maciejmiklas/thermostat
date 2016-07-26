#ifndef MAINCONTROLLER_H_
#define MAINCONTROLLER_H_

#import "RelayDriver.h"
#import "Display.h"
#import "TempSensor.h"
#import "Util.h"
#import "EventBus.h"

class ServiceSuspender: public BusListener {
public:
	ServiceSuspender();

	void cycle();

private:
	void onEvent(BusEvent event, va_list ap);

	/** After button has been pressed suspend all services but display to provide fluent user feedback in GUI */
	const static uint16_t SUSPEND_SERVICE_MS = 60000;
	uint32_t suspendStart;
};

#endif /* MAINCONTROLLER_H_ */
