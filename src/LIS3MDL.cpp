/*
 * LIS3MDL.cpp
 *
 *  Created on: Mar 18, 2025
 *      Author: unflesh
 */

#include "LIS3MDL.h"

namespace IntroStratLib {

#ifndef ARDUINO
LIS3MDL::LIS3MDL(I2C_HandleTypeDef *hi2c, uint8_t address): _i2c(*new I2CDevice(hi2c, address))
{
}
#else
LIS3MDL::LIS3MDL(TwoWire &hi2c, uint8_t address): _i2c(*new I2CDevice(hi2c, address))
{
}
#endif

HAL_StatusTypeDef LIS3MDL::Init(Scale sensitivity, uint8_t force)
{
	HAL_StatusTypeDef status = _i2c.isReady(force);
	if (status) {
		return status;
	}
	_i2c.SetRegister(RegisterMap::CTRL_REG1, 0x7C);
	HAL_Delay(1);
	SetScale(sensitivity);
	HAL_Delay(1);
	_i2c.SetRegister(RegisterMap::CTRL_REG3, 0x00);
	HAL_Delay(1);
	_i2c.SetRegister(RegisterMap::CTRL_REG4, 0x0C);
	HAL_Delay(1);
	_i2c.SetRegister(RegisterMap::CTRL_REG5, 0x40);
	return _i2c.isReady(force);
}

void LIS3MDL::SetScale(Scale sensitivity)
{
	uint8_t bitSensitivity = 2 * (sensitivity - 1);
	uint8_t reg = bitSensitivity << 4;
	_sensitivity = sensitivity;
	_i2c.SetRegister(RegisterMap::CTRL_REG2, reg);
}

int16_t LIS3MDL::RawMX()
{
	if (_i2c.GetRegister(RegisterMap::STATUS_REG)&0x08) {
		uint8_t buf[2];
		_i2c.read(RegisterMap::OUT_X_L, buf, 2);
		_mx = buf[1] << 8 | buf[0];
	}
	return _mx;
}
int16_t LIS3MDL::RawMY()
{
	if (_i2c.GetRegister(RegisterMap::STATUS_REG)&0x08) {
		uint8_t buf[2];
		_i2c.read(RegisterMap::OUT_Y_L, buf, 2);
		_my = buf[1] << 8 | buf[0];
	}
	return _my;
}
int16_t LIS3MDL::RawMZ()
{
	if (_i2c.GetRegister(RegisterMap::STATUS_REG)&0x08) {
		uint8_t buf[2];
		_i2c.read(RegisterMap::OUT_Z_L, buf, 2);
		_mz = buf[1] << 8 | buf[0];
	}
	return _mz;
}

float LIS3MDL::MX()
{
	RawMX();
	float e = int16_t(_mx) * _sensitivity;
	return e / _rawg;
}
float LIS3MDL::MY()
{
	RawMY();
	float e = int16_t(_my) * _sensitivity;
	return e / _rawg;
}
float LIS3MDL::MZ()
{
	RawMZ();
	float e = int16_t(_mz) * _sensitivity;
	return e / _rawg;
}


LIS3MDL::~LIS3MDL() { }

} /* namespace IntroSatLib */



