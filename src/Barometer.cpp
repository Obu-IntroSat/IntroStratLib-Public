/*
 * Barometer.cpp
 *
 *  Created on: Mar 7, 2025
 *      Author: unflesh
 */

#include "Barometer.h"

namespace IntroStratLib {

uint32_t Barometer::GetRawPressure() {
	return _raw_pressure;
}

float Barometer::GetPressure() {
	return _pressure;
}

uint32_t Barometer::GetRawTemperature() {
	return _raw_temperature;
}

float Barometer::GetTemperature() {
	return _temperature;
}

} /* namespace IntroStratLib */


