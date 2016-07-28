#include "Service.h"

Service::Service() :
		enabled(true), serviceBusListener(this) {
}

Service::~Service() {

}

void Service::ServiceBusListener::onEvent(BusEvent event, va_list ap) {
	if (!eb_inGroup(event, SERVICE)) {
		return;
	}
	if (event == SERVICE_RESUME) {
		service->enabled = true;
	} else {
		service->enabled = false;
	}
#if LOG
	log(F("%s service %d"), service->enabled ? "Enable" : "Disable", service->deviceId());
#endif
}

Service::ServiceBusListener::ServiceBusListener(Service* service) :
		service(service) {
}

void Service::cycle() {
	if (enabled) {
		onCycle();
	}
}

