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

/** Keep history for last 14 days. */
const static uint8_t DAY_HISTORY_SIZE = 14;

const static uint8_t RELAYS_AMOUNT = 2;

/* Temperature threshold to enable first relay (RELAY_PIN_1) and start cooling. */
const static uint8_t THRESHOLD_RELAY_0 = 22;

/* Temperature threshold to enable second relay (RELAY_PIN_2) and start cooling. */
const static uint8_t THRESHOLD_RELAY_1 = 27;

const static uint8_t DIG_PIN_BUTTON_NEXT = 2;
const static uint8_t DIG_PIN_BUTTON_PREV = 3;

const static uint8_t DIG_PIN_LCD_D7 = 4;
const static uint8_t DIG_PIN_LCD_D6 = 5;
const static uint8_t DIG_PIN_LCD_D5 = 6;
const static uint8_t DIG_PIN_LCD_D4 = 7;
const static uint8_t DIG_PIN_LCD_ENABLE = 8;
const static uint8_t DIG_PIN_LCD_RS = 9;

const static uint8_t DIG_PIN_RELAY_0 = 10;
const static uint8_t DIG_PIN_RELAY_1 = 11;

const static uint8_t DIG_PIN_TEMP_SENSOR = 12;

const static uint8_t DIG_PIN_SYSTEM_STATUS_LED = 13;

const static uint8_t DEVICE_ID_TEMP_SENSOR = 1;
const static uint8_t DEVICE_ID_RELAY_DRIVER = 2;
const static uint8_t DEVICE_ID_STATS = 3;

const static uint8_t LISTENER_ID_BUTTONS = 200;
const static uint8_t LISTENER_ID_DISPLAY = 201;
const static uint8_t LISTENER_ID_SUSPENDER = 202;
const static uint8_t LISTENER_ID_STATUS = 203;

#define USE_FEHRENHEIT false

/* Minimum time to switch next relay. */
const static uint16_t RELAY_DELAY_AFTER_SWITCH_MS = 60.000;

/* Prevents frequent switches of the particular relay. */
const static uint32_t RELAY_MIN_SWITCH_MS = 600.000;

#endif /* CONFIG_H_ */
