/*
 * Accelerometer.h
 *
 *  Created on: Mar 18, 2025
 *      Author: unflesh
 */

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include "Device.h"

namespace IntroStratLib {

class Accelerometer: virtual public Device {

public:
	virtual int16_t RawAX();
	virtual int16_t RawAY();
	virtual int16_t RawAZ();

	virtual float AX();
	virtual float AY();
	virtual float AZ();
};

} /* namespace IntroStratLib */


#endif /* ACCELEROMETER_H_ */
