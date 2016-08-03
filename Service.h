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
#ifndef SERVICE_H_
#define SERVICE_H_

#include "Arduino.h"
#include "EventBus.h"

class Service {

public:
	Service();
	virtual ~Service();
	void cycle();

protected:
	virtual void onCycle() = 0;
	virtual uint8_t deviceId() = 0;

private:
	class ServiceBusListener: public BusListener {
	public:
		ServiceBusListener(Service* service);
	protected:
		void onEvent(BusEvent event, va_list ap);
	private:
		Service* service;
	};

	boolean enabled;
	ServiceBusListener serviceBusListener;
};

#endif /* SERVICE_H_ */
