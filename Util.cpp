#include <Util.h>

static uint32_t cycleMs;



void util_setup() {
	util_cycle();
}

void util_cycle() {
	cycleMs = millis();
}

uint32_t util_millis() {
	return cycleMs;
}


