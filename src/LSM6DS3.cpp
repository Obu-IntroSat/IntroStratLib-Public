/*
 * LSM6DS3.cpp
 *
 *  Created on: Mar 17, 2025
 *      Author: Goldfor
 */

#include "LSM6DS3.h"

namespace IntroStratLib {

#ifndef ARDUINO
LSM6DS3::LSM6DS3(I2C_HandleTypeDef *hi2c, uint8_t address): _i2c(*new I2CDevice(hi2c, address))
{
}
#else
LSM6DS3::LSM6DS3(TwoWire &hi2c, uint8_t address): _i2c(*new I2CDevice(hi2c, address))
{
}
#endif

uint8_t LSM6DS3::WhoAmI() {
	uint8_t whoAmI = _i2c.GetRegister(RegisterMap::WHO_AM_I_REG);
	return (whoAmI != 0x69);
}

uint8_t LSM6DS3::InitGyro(ScaleGyro sensitivityGyro, DataRateGyro dataRateGyro, uint8_t force)
{
	HAL_Delay(15); // wait for power up
	HAL_StatusTypeDef status = _i2c.isReady(force);
	if (status) {
		return status;
	}
	if (WhoAmI()) {
		return HAL_ERROR;
	}
	SetScaleGyro(sensitivityGyro);
	SetDataRateGyro(dataRateGyro);
	return _i2c.isReady(force);
}

uint8_t LSM6DS3::InitGyro(ScaleGyro sensitivityGyro, uint8_t force)
{
	return InitGyro(sensitivityGyro, DataRateGyro::DR_G_416_Hz, force);
}
uint8_t LSM6DS3::InitGyro(uint8_t force)
{
	return InitGyro(ScaleGyro::DPS0500, force);
}

uint8_t LSM6DS3::InitAccel(ScaleAccel sensitivityAccel, DataRateAccel dataRateAccel, uint8_t force)
{
	HAL_Delay(15); // wait for power up
	HAL_StatusTypeDef status = _i2c.isReady(force);
	if (status) {
		return status;
	}
	if (WhoAmI()) {
		return HAL_ERROR;
	}
	SetScaleAccel(sensitivityAccel);
	SetDataRateAccel(dataRateAccel);
	return _i2c.isReady(force);
}

uint8_t LSM6DS3::InitAccel(ScaleAccel sensitivityAccel, uint8_t force)
{
	return InitAccel(sensitivityAccel, DataRateAccel::DR_A_104_Hz, force);
}
uint8_t LSM6DS3::InitAccel(uint8_t force)
{
	return InitAccel(ScaleAccel::fourG, force);
}

void LSM6DS3::SetMinCutX(float x)
{
	_cutX = x;
}
void LSM6DS3::SetMinCutY(float y)
{
	_cutY = y;
}
void LSM6DS3::SetMinCutZ(float z)
{
	_cutZ = z;
}

float LSM6DS3::cutMin(float value, float cut)
{
	if (value > 0)
	{
		return (value > cut) ? value - cut : 0;
	}
	else
	{
		return ((-value) > cut) ? value + cut : 0;
	}
}

void LSM6DS3::SetScaleGyro(ScaleGyro sensitivityGyro)
{
	uint8_t reg = _i2c.GetRegister(RegisterMap::GYRO_CONFIG);
	reg &= 0xFF ^ (ScaleGyro::DPS2000 << 2);
	reg |= (sensitivityGyro << 2);
	_sensitivityGyro = sensitivityGyro;
	_i2c.SetRegister(RegisterMap::GYRO_CONFIG, reg);
}

void LSM6DS3::SetDataRateGyro(DataRateGyro dataRateGyro)
{
	uint8_t reg = _i2c.GetRegister(RegisterMap::GYRO_CONFIG);
	reg &= 0x0F;
	reg |= (dataRateGyro << 4);
	_dataRateGyro = dataRateGyro;
	_i2c.SetRegister(RegisterMap::GYRO_CONFIG, reg);
}

void LSM6DS3::SetScaleAccel(ScaleAccel sensitivityAccel) {
	uint8_t reg = _i2c.GetRegister(RegisterMap::CTRL1_XL);
	reg &= 0xFF ^ (ScaleAccel::eightG << 2);
	reg |= (sensitivityAccel << 2);
	switch (sensitivityAccel)
	{
		case ScaleAccel::twoG:
			_sensitivityAccel = 0;
			break;
	    case ScaleAccel::fourG:
	    	_sensitivityAccel = 1;
	    	break;
	    case ScaleAccel::eightG:
	    	_sensitivityAccel = 2;
	    	break;
	    case ScaleAccel::sixteenG:
	    	_sensitivityAccel = 3;
	    	break;
	  }
	_i2c.SetRegister(RegisterMap::CTRL1_XL, reg);
}


void LSM6DS3::SetDataRateAccel(DataRateAccel dataRateAccel) {
	uint8_t reg = _i2c.GetRegister(RegisterMap::CTRL1_XL);
	reg &= 0xFF ^ (0x0F<<4);
	reg |= (dataRateAccel<<4);
	_dataRateAccel = dataRateAccel;
	_i2c.SetRegister(RegisterMap::CTRL1_XL, reg);
}

int16_t LSM6DS3::RawGX()
{
	uint8_t buf[2];
	_i2c.read(RegisterMap::GYRO_XOUT_H, buf, 2);
	return buf[1] << 8 | buf[0];
}
int16_t LSM6DS3::RawGY()
{
	uint8_t buf[2];
	_i2c.read(RegisterMap::GYRO_YOUT_H, buf, 2);
	return buf[1] << 8 | buf[0];
}
int16_t LSM6DS3::RawGZ()
{
	uint8_t buf[2];
	_i2c.read(RegisterMap::GYRO_ZOUT_H, buf, 2);
	return buf[1] << 8 | buf[0];
}

float LSM6DS3::GX()
{
	float e = RawGX() * (1 << _sensitivityGyro);
	return cutMin(e * _rawdps, _cutX);
}

float LSM6DS3::GY()
{
	float e = RawGY() * (1 << _sensitivityGyro);
	return cutMin(e * _rawdps, _cutY);
}
float LSM6DS3::GZ()
{
	float e = RawGZ() * (1 << _sensitivityGyro);
	return cutMin(e * _rawdps, _cutZ);
}

int16_t LSM6DS3::RawAX()
{
	uint8_t buf[2];
	_i2c.read(RegisterMap::OUTX_L_XL, buf, 2);
	return buf[1] << 8 | buf[0];
}
int16_t LSM6DS3::RawAY()
{
	uint8_t buf[2];
	_i2c.read(RegisterMap::OUTY_L_XL, buf, 2);
	return buf[1] << 8 | buf[0];
}
int16_t LSM6DS3::RawAZ()
{
	uint8_t buf[2];
	_i2c.read(RegisterMap::OUTZ_L_XL, buf, 2);
	return buf[1] << 8 | buf[0];
}

float LSM6DS3::AX()
{
	float e = RawAX() * (1 << _sensitivityAccel) * _rawg;
	return e;
}
float LSM6DS3::AY()
{
	float e = RawAY() * (1 << _sensitivityAccel) * _rawg;
	return e;
}
float LSM6DS3::AZ()
{
	float e = RawAZ() * (1 << _sensitivityAccel) * _rawg;
	return e;
}

float LSM6DS3::Temp() {
	uint8_t buf[2];
	_i2c.read(RegisterMap::OUT_TEMP_L, buf, 2);
	return float((int16_t)(buf[1] << 8 | buf[0]) / 16.0 + 25);
}

LSM6DS3::~LSM6DS3() { }

} /* namespace IntroSatLib */
