#include "ServiceSuspender.h"

ServiceSuspender::ServiceSuspender() :
		suspendStart(0) {
}

void ServiceSuspender::onEvent(BusEvent event, va_list ap) {
	if (!eb_inGroup(event, BUTTON)) {
		return;
	}

	if (suspendStart == 0) {
#if LOG
		log(F("Suspending services"));
#endif
		eb_fire(SERVICE_SUSPEND);
	}

	suspendStart = util_millis();
}

void ServiceSuspender::cycle() {
	if (suspendStart != 0 && util_millis() - suspendStart > SUSPEND_SERVICE_MS) {
#if LOG
		log(F("Resuming services"));
#endif
		eb_fire(SERVICE_RESUME);
		suspendStart = 0;
	}
}
