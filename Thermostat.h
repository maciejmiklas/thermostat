#ifndef Thermostat_H_
#define Thermostat_H_

#include "Arduino.h"
#include "Timer.h"
#include "ArdLog.h"
#include "TempSensor.h"
#include "RelayDriver.h"
#include "Stats.h"
#include "Buttons.h"
#include "ServiceSuspender.h"
#include "Timer.h"
#include "EventBus.h"

#ifdef __cplusplus
extern "C" {
#endif

void loop();
void setup();

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* Thermostat_H_ */
