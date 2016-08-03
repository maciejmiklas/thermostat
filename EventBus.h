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
#ifndef EVENTBUS_H_
#define EVENTBUS_H_

#import "Arduino.h"
#import "ArdLog.h"

typedef enum {
	/** Parameters: 0 - relay ID */
	RELAY_ON = 10,

	/** Parameters: 0 - relay ID */
	RELAY_OFF = 11,

	/** Parameters: none */
	BUTTON_NEXT = 20,

	/** Parameters: none */
	BUTTON_PREV = 21,

	/** Parameters: none */
	SERVICE_SUSPEND = 30,

	/** Parameters: none */
	SERVICE_RESUME = 31,

	/**
	 * Event with this state will not be fired.
	 * It's just used by some methods passing events to indicate that there was no event.
	 */
	NO_EVENT = 255
} BusEvent;

typedef enum {
	RELAY, BUTTON, SERVICE
} BusEventGroup;

class BusListener {
public:
	virtual void onEvent(BusEvent event, va_list ap) = 0;
protected:
	virtual ~BusListener();
	BusListener();
};

boolean eb_inGroup(BusEvent event, BusEventGroup group);
void eb_register(BusListener* listener);
void eb_fire(BusEvent event, ...);

#endif /* EVENTBUS_H_ */
