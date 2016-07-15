#ifndef RELAY_H_
#define RELAY_H_

#include "Arduino.h"
#include "TempSensor.h"

class Relay {
public:
	Relay(TempSensor* ts, uint8_t pin, uint8_t threshold);
	boolean isEnabled();
	boolean drive();

private:
	TempSensor* const tempSensor;
	const uint8_t pin;
	const uint8_t threshold;
	boolean enabled;
	uint32_t lastSwitch;

	/** Prevents frequent switches of the relay. */
	const static uint32_t MIN_SWITCH_MS = 600.000;
};

#endif /* RELAY_H_ */
