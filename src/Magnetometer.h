/*
 * Magnetometer.h
 *
 *  Created on: Mar 18, 2025
 *      Author: unflesh
 */

#ifndef MAGNETOMETER_H_
#define MAGNETOMETER_H_

#include "Device.h"
#include <cmath>
#include <array>

namespace IntroStratLib
{

	class Magnetometer : public Device
	{
		uint16_t _mx = 0;
		uint16_t _my = 0;
		uint16_t _mz = 0;

	public:

		void Read();

		int16_t RawMX();
		int16_t RawMY();
		int16_t RawMZ();

		float MX();
		float MY();
		float MZ();

	};

} /* namespace IntroSatLib */


#endif /* MAGNETOMETER_H_ */
