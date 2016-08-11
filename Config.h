/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef CONFIG_H_
#define CONFIG_H_

/** Keep history for last 30 days. */
const static uint8_t DAY_HISTORY_SIZE = 60;

const static uint8_t RELAYS_AMOUNT = 2;

/* Temperature threshold to enable first relay (RELAY_PIN_1) and start cooling. */
const static uint8_t THRESHOLD_RELAY_1 = 22;

/* Temperature threshold to enable second relay (RELAY_PIN_2) and start cooling. */
const static uint8_t THRESHOLD_RELAY_2 = 27;

// TODO replace delay with no-op and rename to RELAY_SWITCH__MS
const static uint16_t DELAY_AFTER_SWITCH_MS = 1000;

const static uint8_t BUTTON_NEXT_IRQ = 0;
const static uint8_t DIG_PIN_BUTTON_NEXT = 2;
const static uint8_t DIG_PIN_BUTTON_PREV = 3;

const static uint8_t DIG_PIN_LCD_D3 = 4;
const static uint8_t DIG_PIN_LCD_D2 = 5;
const static uint8_t DIG_PIN_LCD_D1 = 6;
const static uint8_t DIG_PIN_LCD_D0 = 7;
const static uint8_t DIG_PIN_LCD_ENABLE = 8;
const static uint8_t DIG_PIN_LCD_RS = 9;

const static uint8_t DIG_PIN_RELAY_1 = 10;
const static uint8_t DIG_PIN_IN_RELAY_2 = 11;

const static uint8_t DIG_PIN_TEMP_SENSOR = 12;

const static uint8_t DIG_PIN_SYSTEM_STATUS_LED = 13;

const static uint8_t SERVICE_ID_TEMP_SENSOR = 1;
const static uint8_t SERVICE_ID_RELAY_DRIVER = 2;
const static uint8_t SERVICE_ID_STATS = 3;

#endif /* CONFIG_H_ */
