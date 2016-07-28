#ifndef TEMPSENSOR_H_
#define TEMPSENSOR_H_

#include "ArdLog.h"
#include "Arduino.h"
#include "OneWire.h"
#include "Util.h"
#include "DallasTemperature.h"
#include "Service.h"
#include "Config.h"

class TempSensor: public Service {
public:
	TempSensor();
	void cycle();
	uint8_t getTemp();


protected:
	uint8_t deviceId();

private:

	/**
	 * We take #PROBES_SIZE probes from temp sensor, each one with delay of #PROBE_DELAY milliseconds.
	 * After collecting all required probes we calculate median and this is the temperature.
	 */
	const static uint8_t PROBES_SIZE = 6;
	const static uint8_t PROBES_MED_IDX = PROBES_SIZE / 2;
	const static uint32_t PROBE_FREQ_MS = 2000;
	int8_t probes[PROBES_SIZE];
	uint8_t probeIdx;

	int8_t curentTemp;

	uint32_t lastProbeTime;

	OneWire oneWire;
	DallasTemperature dallasTemperature;
	boolean enabled;

	inline int8_t readTemp();
};

#endif /* TEMPSENSOR_H_ */
