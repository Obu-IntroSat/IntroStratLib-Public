/*
 * Logger.h
 *
 *  Created on: Mar 6, 2025
 *      Author: Goldfor
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdint.h>

#define LOGDATA 0

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

void __attribute__((weak)) LOGGER(const char *text, uint8_t len);

void logText(const char *text);
void logHEX(uint8_t val);
void logNumber(uint8_t val);
HAL_StatusTypeDef logStatus(HAL_StatusTypeDef status);


#endif /* LOGGER_H_ */
