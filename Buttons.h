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
#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "Arduino.h"
#include "ArdLog.h"
#include "EventBus.h"
#include "Config.h"
#include "Util.h"

class Buttons: public BusListener {
public:
	Buttons();
	void cycle();
private:
	const static uint8_t PRESS_MS = 200;
	uint32_t pressMs;
	void onEvent(BusEvent event, va_list ap);
	void inline setuButton(uint8_t pin);
};

#endif /* BUTTONS_H_ */
