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
#ifndef MAINCONTROLLER_H_
#define MAINCONTROLLER_H_

#import "RelayDriver.h"
#import "Display.h"
#import "TempSensor.h"
#import "Util.h"
#import "EventBus.h"

class ServiceSuspender: public BusListener {
public:
	ServiceSuspender();

	void cycle();

private:
	void onEvent(BusEvent event, va_list ap);

	/** After button has been pressed suspend all services but display to provide fluent user feedback in GUI */
	const static uint16_t SUSPEND_SERVICE_MS = 60000;
	uint32_t suspendStart;
};

#endif /* MAINCONTROLLER_H_ */
