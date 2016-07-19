#ifndef RELAYDRIVER_H_
#define RELAYDRIVER_H_

#import "TempSensor.h"
#import "Stats.h"
#import "Relay.h"
#import "Service.h"

/**
 * We are supporting two fans: first will go on after reaching first temperature threshold, and second one after
 * reaching second threshold.
 */
class RelayDriver: public Service {
public:
	RelayDriver(TempSensor* ts, Stats* stats);

	void cycle();

private:
	Stats* const stats;

	/* Temperature threshold to enable first relay (RELAY_PIN_1) and start cooling. */
	const static uint8_t THRESHOLD_RELAY_1 = 20;
	const static uint8_t RELAY_PIN_1 = 8;

	/* Temperature threshold to enable second relay (RELAY_PIN_2) and start cooling. */
	const static uint8_t THRESHOLD_RELAY_2 = 24;
	const static uint8_t RELAY_PIN_2 = 9;

	const static uint16_t DELAY_AFTER_SWITCH_MS = 1000;

	Relay relay1;
	Relay relay2;

	inline void driveRelay(Relay* relay, uint8_t id);
};

#endif /* RELAYDRIVER_H_ */
