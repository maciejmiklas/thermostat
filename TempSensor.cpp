#include "TempSensor.h"

TempSensor::TempSensor() :
		oneWire(SENSOR_PIN), dallasTemperature(&oneWire) {
	curentTemp = 0;
	lastProbeTime = 0;
	dallasTemperature.begin();

	cycle();
}

uint8_t TempSensor::getTemp() {
	return curentTemp;
}

void TempSensor::cycle() {
	uint32_t millis = util_millis();
	if (millis - lastProbeTime < PROBE_FREQ_MS) {
		return;
	}
	lastProbeTime = millis;
	dallasTemperature.requestTemperatures();
	curentTemp = (uint8_t) (dallasTemperature.getTempCByIndex(0) + 0.5);

#if LOG
	log(F("Current temp: %d deg"), curentTemp);
#endif
}
