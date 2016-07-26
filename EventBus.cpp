#include "EventBus.h"

const static uint8_t LISTNERS_MAX = 10;
static BusListener* listeners[LISTNERS_MAX];
static uint8_t listenersAmount = 0;

BusListener::~BusListener() {
}

BusListener::BusListener() {
	//eb_register(this);
	log(F("AAAAA"));
}

void eb_register(BusListener* listener) {
	if (listenersAmount == LISTNERS_MAX) {
#if LOG
		log(F("### TOO MANY LISTENERS (%d) ###"), listenersAmount);
#endif
		return;
	}

#if LOG
	log(F("Register %d bus listener"), listenersAmount);
#endif
	listeners[listenersAmount++] = listener;
}

void eb_fire(BusEvent event, ...) {
#if LOG
	log(F("Fire event: %d"), event);
#endif

	for (uint8_t idx = 0; idx < listenersAmount; listenersAmount++) {
		va_list ap;
		va_start(ap, event);
		listeners[idx]->onEvent(event, ap);
		va_end(ap);
	}
}

boolean eb_inGroup(BusEvent event, BusEventGroup group) {
	boolean in = false;
	switch (group) {
	case RELAY:
		in = event >= 10 && event < 20;
		break;
	case BUTTON:
		in = event >= 20 && event < 30;
		break;
	case SERVICE:
		in = event >= 30 && event < 40;
		break;
	}

	return in;
}

