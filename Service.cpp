#include "Service.h"

Service::Service() :
		enabled(true) {
}

Service::~Service() {

}

void Service::onEvent(BusEvent event, va_list ap) {
	if (event == SERVICE_RESUME) {
		enabled = true;
	} else if (event == SERVICE_SUSPEND) {
		enabled = false;
	}
}

