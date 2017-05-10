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
#ifndef RELAYHYSTERESISCONTROLLER_H_
#define RELAYHYSTERESISCONTROLLER_H_

#include "RelayController.h"
#include "TempSensor.h"

class RelayHysteresisController: public RelayController {
public:
	RelayHysteresisController(TempSensor* ts, int16_t tempSetPoint);
	virtual ~RelayHysteresisController();
	Relay::State execute();

private:
	uint32_t lastSwitchMs;
	boolean on;
};

#endif /* RELAYHYSTERESISCONTROLLER_H_ */
