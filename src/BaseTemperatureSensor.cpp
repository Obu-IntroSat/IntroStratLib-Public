/*
 * TemperatureSensor.cpp
 *
 *  Created on: Mar 6, 2025
 *      Author: Aleksey <TeaCupMe> Gilenko
 */

#include "TemperatureSensor.h"

namespace IntroStratLib {

float TemperatureSensor::GetTemperature() {
	return temperature;
}

float TemperatureSensor::GetTemperatureC() {
	return GetTemperature();
}

float TemperatureSensor::GetTemperatureF() {
//	return GetTemperature() * 9 / 5 + 32; // TODO: Faster algorithm? Maybe optimize for LM75A using LM75A::GetTemperatureTimes8
	return GetTemperature() * 2 + 31;
}

}
