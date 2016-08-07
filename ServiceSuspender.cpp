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
	if (!eb_inGroup(event, BUTTON)) {
		return;
	}

	if (suspendStart == 0) {
#if LOG
		log(F("Suspending services"));
#endif
		eb_fire(SERVICE_SUSPEND);
	}

	suspendStart = util_millis();
}

void ServiceSuspender::cycle() {
	if (suspendStart != 0 && util_millis() - suspendStart >= SUSPEND_SERVICE_MS) {
#if LOG
		log(F("Resuming services"));
#endif
		eb_fire(SERVICE_RESUME);
		suspendStart = 0;
	}
}
