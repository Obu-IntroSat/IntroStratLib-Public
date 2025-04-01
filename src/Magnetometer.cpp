/*
 * Magnetometer.cpp
 *
 *  Created on: Mar 18, 2025
 *      Author: unflesh
 */
#include "Magnetometer.h"

namespace IntroStratLib {

	int16_t Magnetometer::RawMX()
	{
		return _mx;
	}
	int16_t Magnetometer::RawMY()
	{
		return _my;
	}
	int16_t Magnetometer::RawMZ()
	{
		return _mz;
	}

} /* namespace IntroSatLib */
