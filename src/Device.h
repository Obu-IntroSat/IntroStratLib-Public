/*
 * Device.h
 *
 *  Created on: Mar 6, 2025
 *      Author: Aleksey <TeaCupMe> Gilenko
 */

#ifndef DEVICE_H_
#define DEVICE_H_
#include "stdint.h"

#ifndef __MAIN_H

	#if ARDUINO >= 100
		#include "Arduino.h"
		#include "Wire.h"
	#else
		#if __has_include("stm32h7xx_hal.h")
			#include "stm32h7xx_hal.h"
			#include "stm32h7xx_hal_i2c.h"
			#include "stm32h7xx_hal_i2c_ex.h"
		#endif
	#endif
#else
	#if __has_include("main.h")
		#include "main.h"
	#endif

#endif

namespace IntroStratLib {

class Device {

public:
	/*virtual*/ HAL_StatusTypeDef Init();
	void Enable();
	};

} /* namespace IntroStratLib */

#endif /* DEVICE_H_ */
