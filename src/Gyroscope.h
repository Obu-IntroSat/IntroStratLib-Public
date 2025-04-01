/*
 * Gyroscope.h
 *
 *  Created on: Mar 17, 2025
 *      Author: unflesh
 */

#ifndef GYROSCOPE_H_
#define GYROSCOPE_H_

#include "Device.h"

namespace IntroStratLib {

class Gyroscope: virtual public Device {

public:
	virtual int16_t RawGX();
	virtual int16_t RawGY();
	virtual int16_t RawGZ();

	virtual float GX();
	virtual float GY();
	virtual float GZ();
};

} /* namespace IntroStratLib */


#endif /* GYROSCOPE_H_ */
