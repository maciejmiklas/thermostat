#include "Relay.h"

Relay::Relay(TempSensor* ts, uint8_t pin, uint8_t threshold) :
		tempSensor(ts), pin(pin), threshold(threshold) {
	enabled = false;
	lastSwitch = 0;
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
}

boolean Relay::isEnabled() {
	return enabled;
}

boolean Relay::drive() {
	uint32_t millis = util_millis();
	if (millis - lastSwitch < MIN_SWITCH_MS) {
		return false;
	}
	lastSwitch = millis;

	boolean changed = false;
	uint8_t temp = tempSensor->getTemp();
	if (enabled && temp < threshold) {
		digitalWrite(pin, LOW);
		enabled = false;
		changed = true;
#if LOG
		log(F("Disable relay on: %d by %d deg"), pin, temp);
#endif
	} else if (!enabled && temp >= threshold) {
		digitalWrite(pin, HIGH);
		enabled = true;
		changed = true;
#if LOG
		log(F("Enable relay on: %d by %d deg"), pin, temp);
#endif
	}

	return changed;
}
