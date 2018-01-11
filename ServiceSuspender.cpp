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
#include "ServiceSuspender.h"

ServiceSuspender::ServiceSuspender() :
		suspendStart(0) {
}

void ServiceSuspender::onEvent(BusEvent event, va_list ap) {
	if (event == BusEvent::CYCLE) {
		cycle();
	} else if (eb_inGroup(event, BusEventGroup::BUTTON)) {

		if (suspendStart == 0) {
#if LOG
			log(F("SU Suspending"));
#endif
			eb_fire(BusEvent::SERVICE_SUSPEND);
		}

		suspendStart = util_ms();

	} else if (event == BusEvent::SERVICE_RESUME) {
#if LOG
		log(F("SU drop resume"));
#endif
		suspendStart = 0;
	}
}

uint8_t ServiceSuspender::listenerId() {
	return LISTENER_ID_SUSPENDER;
}

inline void ServiceSuspender::cycle() {
	if (suspendStart != 0 && util_ms() - suspendStart >= SUSPEND_SERVICE_MS) {
#if LOG
		log(F("SU Resuming"));
#endif
		eb_fire(BusEvent::SERVICE_RESUME);
		suspendStart = 0;
	}
}
