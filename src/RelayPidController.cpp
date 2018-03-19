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
#include "RelayPidController.h"

RelayPidController::RelayPidController(TempSensor* ts, int8_t tempSetPoint) :
		RelayController(ts, tempSetPoint), iDerivationSum(0), dPrevDerivation(0) {
}

RelayPidController::~RelayPidController() {
}

float RelayPidController::calculateP(float derivation) {
	return RPC_AMP_P * derivation;
}

float RelayPidController::calculateI(float derivation) {
	iDerivationSum += derivation;
	return RPC_AMP_I * iDerivationSum;
}

float RelayPidController::calculateD(float derivation) {
	float d = RPC_AMP_D * (derivation - dPrevDerivation);
	dPrevDerivation = derivation;
	return d;
}

Relay::State RelayPidController::execute() {
	float derivation = tempSetPoint - tempSensor->getTemp();
	float valP = calculateP(derivation);
	float valI = calculateI(derivation);
	float valD = calculateD(derivation);

	float pid = valP + valI + valD;

	Relay::State state = pid < RPC_PID_SWITCH_THRESHOLD ? Relay::State::ON: Relay::State::OFF;

	return state;
}

