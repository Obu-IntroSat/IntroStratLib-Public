/*
 * I2CDevice.h
 *
 *  Created on: Mar 5, 2025
 *      Author: Goldfor
 */

#ifndef I2CDEVICE_H_
#define I2CDEVICE_H_

#include <stdint.h>
#include "I2C_Err.h"

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

enum I2CSpeed
{
	Standard = 0,
	Fast = 1
};

class I2CDevice {

private:
	I2CSpeed _speed = I2CSpeed::Standard;
	uint8_t _address = 0;
	I2C_HandleTypeDef *_hi2c = 0;
	HAL_StatusTypeDef innerIsReady();
public:
#if not (ARDUINO >= 100)
	I2CDevice(I2C_HandleTypeDef *hi2c, uint8_t address);
	I2CDevice(I2C_HandleTypeDef *hi2c, uint8_t address, I2CSpeed speed);
#else
	I2CDevice(TwoWire &hi2c, uint8_t address);
	I2CDevice(TwoWire &hi2c, uint8_t address, I2CSpeed speed);
#endif
	I2CDevice(const I2CDevice& other);
	I2CDevice(I2CDevice&& other);
	I2CDevice& operator=(const I2CDevice& other);
	I2CDevice& operator=(I2CDevice&& other);
	HAL_StatusTypeDef isReady(uint8_t force = 0);
	HAL_StatusTypeDef read(uint8_t* Data, uint8_t Nbytes);
	HAL_StatusTypeDef read(uint8_t Register, uint8_t* Data, uint8_t Nbytes);
	HAL_StatusTypeDef write(uint8_t* Data, uint8_t Nbytes);
	HAL_StatusTypeDef write(uint8_t Register, uint8_t* Data, uint8_t Nbytes);
	uint8_t GetRegister(uint8_t reg);
	void SetRegister(uint8_t reg, uint8_t value);
	void SetBitRegister(uint8_t reg, uint8_t bit);
	void ResetBitRegister(uint8_t reg, uint8_t bit);
	void BitRegister(uint8_t reg, uint8_t bit, uint8_t value);
	~I2CDevice();
};


} /* namespace IntroStratLib */


#endif /* I2CDEVICE_H_ */
