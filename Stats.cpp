#include "Stats.h"

Stats::Stats() :
		systemTimer() {
	systemTimer.start();
}

void Stats::onEvent(BusEvent event, va_list ap) {
	if (!eb_inGroup(event, RELAY)) {
		return;
	}
	int relayId = va_arg(ap, int);

	if (event == RELAY_ON) {
		relayTimer[relayId].start();

	} else if (event == RELAY_OFF) {
		relayTimer[relayId].suspend();
	}
}

Time* Stats::getRelayTime(uint8_t relayId) {
	return relayTimer[relayId].getTime();
}
