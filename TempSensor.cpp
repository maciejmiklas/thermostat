#include "TempSensor.h"

TempSensor::TempSensor() :
		probeIdx(0), curentTemp(0), lastProbeTime(0), oneWire(PIN_TEMP_SENSOR), dallasTemperature(&oneWire), enabled(
		true) {
	dallasTemperature.begin();
	curentTemp = readTemp();
}

uint8_t TempSensor::getTemp() {
	return curentTemp;
}

void TempSensor::onCycle() {
	uint32_t millis = util_millis();
	if (!enabled || millis - lastProbeTime < PROBE_FREQ_MS) {
		return;
	}
	lastProbeTime = millis;
	int8_t temp = readTemp();
	if (probeIdx == PROBES_SIZE) {
		util_sort_i8(probes, PROBES_SIZE);
		curentTemp = probes[PROBES_MED_IDX];
		probeIdx = 0;
#if LOG
		log(F("Temp: %d, probes: %d %d %d %d %d %d"), temp, probes[0], probes[1], probes[2], probes[3], probes[4],
				probes[5]);
#endif

	} else {
		probes[probeIdx++] = temp;
	}
}

uint8_t TempSensor::deviceId() {
	return SERVICE_ID_TEMP_SENSOR;
}

inline int8_t TempSensor::readTemp() {
	dallasTemperature.requestTemperatures();
	return (int8_t) (dallasTemperature.getTempCByIndex(0) + 0.5);
}
