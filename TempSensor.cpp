#include "TempSensor.h"

TempSensor::TempSensor() :
		probeIdx(10), curentTemp(0), lastProbeTime(0), oneWire(SENSOR_PIN), dallasTemperature(&oneWire) {
	dallasTemperature.begin();
	cycle();
}

uint8_t TempSensor::getTemp() {
	return curentTemp;
}

void TempSensor::cycle() {
	uint32_t millis = util_millis();
	if (!enabled || millis - lastProbeTime < PROBE_FREQ_MS) {
		return;
	}
	lastProbeTime = millis;
	dallasTemperature.requestTemperatures();
	int8_t temp = (int8_t) (dallasTemperature.getTempCByIndex(0) + 0.5);

	if (probeIdx == PROBES_SIZE) {
		util_sort_i8(probes, PROBES_SIZE);
		curentTemp = probes[PROBES_MED_IDX];
#if LOG
		log(F("Sorted probes"), temp);
		for (uint8_t i = 0; i < PROBES_SIZE; i++) {
			log(F("-- %d"), probes[i]);
		}
#endif

	} else {
		probes[probeIdx++] = temp;
	}

#if LOG
	log(F("Temp reading: %d, current: %d"), temp, curentTemp);
#endif
}
