#include "Service.h"

Service::Service() :
		enabled(true) {
}

Service::~Service() {

}

void Service::onEvent(BusEvent event, va_list ap) {
	if (!eb_inGroup(event, SERVICE)) {
		return;
	}
	if (event == SERVICE_RESUME) {
		enabled = true;
	} else {
		enabled = false;
	}
#if LOG
	log(F("%s service %d"), enabled ? "Enable" : "Disable", setviceId());
#endif
}

