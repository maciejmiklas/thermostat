#ifndef DOMAIN_H_
#define DOMAIN_H_

#include "Arduino.h"

void util_setup();
void util_cycle();

uint32_t util_millis();

inline void util_sort_u8(uint8_t arr[], uint8_t size) {
	uint8_t i, temp, j;
	for (i = 1; i < size; i++) {
		temp = arr[i];
		j = i - 1;
		while ((temp < arr[j]) && (j >= 0)) {
			arr[j + 1] = arr[j];
			j = j - 1;
		}
		arr[j + 1] = temp;
	}
}

inline int8_t util_avg_i8(int8_t arr[], uint8_t size) {
	int16_t tmp = 0;
	for (uint8_t i = 0; i < size; i++) {
		tmp += arr[i];
	}
	return tmp / size;
}

inline int8_t util_min_i8(int8_t arr[], uint8_t size) {
	int8_t tmp = arr[0];
	for (uint8_t i = 1; i < size; i++) {
		int8_t next = arr[i];
		if (next < tmp) {
			tmp = next;
		}
	}
	return tmp;
}

inline int8_t util_max_i8(int8_t arr[], uint8_t size) {
	int8_t tmp = arr[0];
	for (uint8_t i = 1; i < size; i++) {
		int8_t next = arr[i];
		if (next > tmp) {
			tmp = next;
		}
	}
	return tmp;
}

inline void util_sort_i8(int8_t arr[], uint8_t size) {
	int8_t i, temp, j;
	for (i = 1; i < size; i++) {
		temp = arr[i];
		j = i - 1;
		while ((temp < arr[j]) && (j >= 0)) {
			arr[j + 1] = arr[j];
			j = j - 1;
		}
		arr[j + 1] = temp;
	}
}

inline uint16_t util_freeRam() {
	extern int __heap_start, *__brkval;
	int v;
	return (uint16_t) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

inline uint16_t util_abs16(int16_t val) {
	return val > 0 ? val : val * -1;

}

inline uint8_t util_abs8(int8_t val) {
	return val > 0 ? val : val * -1;
}

#endif /* DOMAIN_H_ */
