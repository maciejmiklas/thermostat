#include "Service.h"

Service::Service() :
		enabled(true) {
}

Service::~Service() {

}

void Service::setEnabled(boolean enabledPar) {
	enabled = enabledPar;
}

