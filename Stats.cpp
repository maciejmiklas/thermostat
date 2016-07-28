#include "Stats.h"

Stats::Stats(TempSensor* tempSensor) :
		systemTimer(), dayProbeIdx(0), lastProbeMs(0), tempSensor(tempSensor) {
	systemTimer.start();
}

void Stats::onEvent(BusEvent event, va_list ap) {
	if (!eb_inGroup(event, RELAY)) {
		return;
	}
	int relayId = va_arg(ap, int);

	if (event == RELAY_ON) {
		relayTimer[relayId].start();

	} else if (event == RELAY_OFF) {
		relayTimer[relayId].suspend();
	}
}

Time* Stats::getRelayTime(uint8_t relayId) {
	return relayTimer[relayId].getTime();
}

Time* Stats::getUpTime() {
	return systemTimer.getTime();
}

void Stats::onCycle() {

}

uint8_t Stats::deviceId() {
	return SERVICE_ID_STATS;
}

void Stats::probeTemp() {
	uint32_t currentMillis = util_millis();
	if (currentMillis - lastProbeMs < DAY_PROBE_MS) {
		return;
	}
	if (dayProbeIdx == DAY_PROBES_AMOUNT) {
		int8_t avg = util_avg_i8(dayProbes, DAY_PROBES_AMOUNT);
		int8_t min = util_min_i8(dayProbes, DAY_PROBES_AMOUNT);
		int8_t max = util_max_i8(dayProbes, DAY_PROBES_AMOUNT);
#if LOG
		log(F("Calculated Temp -> AVG: %d, Min: %d, Max :%d"), avg, min, max);
#endif
		dayProbeIdx = 0;
	} else {
		dayProbes[dayProbeIdx++] = tempSensor->getTemp();
	}
	lastProbeMs = currentMillis;
}
