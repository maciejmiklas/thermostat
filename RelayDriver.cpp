#include "RelayDriver.h"

RelayDriver::RelayDriver(TempSensor* ts) :
		relay1(ts, PIN_RELAY_1, THRESHOLD_RELAY_1), relay2(ts, PIN_RELAY_2, THRESHOLD_RELAY_2) {
}

void RelayDriver::cycle() {
	if (!enabled) {
		return;
	}
	driveRelay(&relay1, 0);
	driveRelay(&relay2, 1);
}

inline void RelayDriver::driveRelay(Relay* relay, uint8_t id) {
	boolean changed = relay->drive();
	if (changed) {
		eb_fire(relay->isEnabled() ? RELAY_ON : RELAY_OFF, id);
		delay(DELAY_AFTER_SWITCH_MS);
	}
}

