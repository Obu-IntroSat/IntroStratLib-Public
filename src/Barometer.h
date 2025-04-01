/*
 * Barometer.h
 *
 *  Created on: Mar 7, 2025
 *      Author: unflesh
 */

#ifndef BAROMETER_H_
#define BAROMETER_H_

#include "Device.h"

namespace IntroStratLib {

class Barometer: public Device {

private:
	uint32_t _raw_pressure;
	float _pressure;
	uint32_t _raw_temperature;
	float _temperature;

public:
	virtual uint32_t GetRawPressure();
	virtual uint32_t GetRawTemperature();
	virtual float GetPressure();
	virtual float GetTemperature();
};

} /* namespace IntroStratLib */



#endif /* BAROMETER_H_ */
