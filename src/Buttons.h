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
#include "Initializable.h"

class Buttons: public BusListener, public Initializable {
public:
	Buttons();

private:
	void init();
	void onEvent(BusEvent event, va_list ap);
	uint8_t listenerId();
	void inline setupButton(uint8_t pin);
	void inline cycle();
};

#endif /* BUTTONS_H_ */
