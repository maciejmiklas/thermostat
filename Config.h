#ifndef CONFIG_H_
#define CONFIG_H_

const static uint8_t RELAYS_AMOUNT = 2;

/* Temperature threshold to enable first relay (RELAY_PIN_1) and start cooling. */
const static uint8_t THRESHOLD_RELAY_1 = 20;

/* Temperature threshold to enable second relay (RELAY_PIN_2) and start cooling. */
const static uint8_t THRESHOLD_RELAY_2 = 24;

const static uint16_t DELAY_AFTER_SWITCH_MS = 1000;

const static uint8_t PIN_RELAY_1 = 8;
const static uint8_t PIN_RELAY_2 = 9;
const static uint8_t PIN_BUTTON_NEXT = 10;
const static uint8_t PIN_BUTTON_PREV = 11;
const static uint8_t PIN_TEMP_SENSOR = 12;

const static uint8_t SERVICE_ID_TEMP_SENSOR = 1;
const static uint8_t SERVICE_ID_RELAY_DRIVER = 2;
const static uint8_t SERVICE_ID_STATS = 3;

#endif /* CONFIG_H_ */
