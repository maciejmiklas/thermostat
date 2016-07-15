#include "RelayDriver.h"

RelayDriver::RelayDriver(TempSensor* ts, Stats* stats) :
		stats(stats), relay1(ts, RELAY_PIN_1, THRESHOLD_RELAY_1), relay2(ts, RELAY_PIN_2, THRESHOLD_RELAY_2) {
}

void RelayDriver::cycle() {
	driveRelay(&relay1, 1);
	driveRelay(&relay2, 2);
}

void RelayDriver::driveRelay(Relay* relay, uint8_t id) {
	if (!relay->drive()) {
		return;
	}
	stats->relayChange(relay->isEnabled(), id);
}

