#ifndef TEMPSENSOR_H_
#define TEMPSENSOR_H_

#include "ArdLog.h"
#include "Arduino.h"
#include "OneWire.h"
#include "Util.h"
#include "DallasTemperature.h"

class TempSensor {
public:
	TempSensor();
	void cycle();

	// TODO provide average temp over last 10 minutes
	uint8_t getTemp();

private:

	/* Frequency to read temperature from the sensor. */
	const static uint16_t PROBE_FREQ_MS = 60000;

	const static uint8_t SENSOR_PIN = 11;

	uint8_t curentTemp;

	uint32_t lastProbeTime;

	OneWire oneWire;
	DallasTemperature dallasTemperature;
};

#endif /* TEMPSENSOR_H_ */
