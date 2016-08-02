#include "Stats.h"

Stats::Stats(TempSensor* tempSensor) :
		tempSensor(tempSensor), systemTimer(), dayProbeIdx(0), lastProbeMs(0), dayHistoryIdx(0), dit_idx(0), actualProbesIdx(
				0), actualProbesFull(false) {
	systemTimer.start();
	actualTemp.day = 0;
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

Temp* Stats::getActual() {
	return &actualTemp;
}

void Stats::onCycle() {
	uint32_t currentMillis = util_millis();
	if (currentMillis - lastProbeMs >= DAY_PROBE_MS) {
		probeDayTemp();
		probeActualTemp();

		lastProbeMs = currentMillis;
	}
}

uint8_t Stats::deviceId() {
	return SERVICE_ID_STATS;
}

void Stats::probeActualTemp() {
	if (actualProbesIdx == ACTUAL_PROBES_SIZE) {
		actualProbesIdx = 0;
		actualProbesFull = true;
	}
	int8_t actTemp = tempSensor->getTemp();
	actualProbes[actualProbesIdx++] = actTemp;

	uint8_t probesSize = actualProbesFull ? ACTUAL_PROBES_SIZE : actualProbesIdx;
	actualTemp.avg = util_avg_i8(actualProbes, probesSize);
	actualTemp.min = util_min_i8(actualProbes, probesSize);
	actualTemp.max = util_max_i8(actualProbes, probesSize);

#if LOG
	log(F("Actual(%d) -> Now: %d, Avg: %d, Min: %d, Max :%d"), actualProbesIdx, actTemp, actualTemp.avg, actualTemp.min,
			actualTemp.max);
#endif
}

void Stats::probeDayTemp() {
	if (dayProbeIdx == PROBES_PER_DAY) {
		if (dayHistoryIdx == DAY_HISTORY_SIZE) {
			dayHistoryIdx = 0;
		}
		Temp temp = dayHistory[dayHistoryIdx++];
		temp.avg = util_avg_i8(dayProbes, PROBES_PER_DAY);
		temp.min = util_min_i8(dayProbes, PROBES_PER_DAY);
		temp.max = util_max_i8(dayProbes, PROBES_PER_DAY);
		dayProbeIdx = 0;
#if LOG
		log(F("History -> AVG: %d, Min: %d, Max :%d"), temp.avg, temp.min, temp.max);
#endif
	} else {
		dayProbes[dayProbeIdx++] = tempSensor->getTemp();
	}
}

Temp* Stats::dit_next() {
	if (dit_idx == 0) {
		dit_idx = dayProbeIdx;
	}
	Temp* temp = &dayHistory[dit_idx--];
	updateTempDay(temp);
	return temp;
}

Temp* Stats::dit_prev() {
	if (dit_idx == DAY_HISTORY_SIZE) {
		dit_idx = 0;
	}
	Temp* temp = &dayHistory[dit_idx++];
	updateTempDay(temp);
	return temp;
}

inline void Stats::updateTempDay(Temp* temp) {
	temp->day = DAY_HISTORY_SIZE - dit_idx;
}

void Stats::dit_reset() {
	dit_idx = dayProbeIdx;
}

uint8_t Stats::dit_size() {
	return dayProbeIdx;
}
