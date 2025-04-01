/*
 * LIS3MDL.h
 *
 *  Created on: Mar 18, 2025
 *      Author: unflesh
 */

#ifndef LIS3MDL_H_
#define LIS3MDL_H_

#include "I2CDevice.h"
#include "Magnetometer.h"
#include <cmath>
#include <array>

namespace IntroStratLib {

class LIS3MDL : public Magnetometer {
private:
	static const uint8_t BASE_ADDRESS = 0x1C;
	static constexpr float _rawg = 27386.0f / 4.0f; // Gauss

	enum RegisterMap
	{
		CTRL_REG1 = 0x20,
		CTRL_REG2,
		CTRL_REG3,
		CTRL_REG4,
		CTRL_REG5,
		STATUS_REG = 0x27,
		OUT_X_L,
		OUT_X_H,
		OUT_Y_L,
		OUT_Y_H,
		OUT_Z_L,
		OUT_Z_H,
	};

	uint16_t _mx = 0;
	uint16_t _my = 0;
	uint16_t _mz = 0;

	uint8_t _sensitivity = 0;

protected:
	I2CDevice _i2c;

public:

	enum Scale
	{
		G4 = 1,
		G8,
		G12,
		G16
	};

#ifndef ARDUINO
	LIS3MDL(I2C_HandleTypeDef *hi2c, uint8_t address = BASE_ADDRESS);
#else
	LIS3MDL(TwoWire &hi2c, uint8_t address = BASE_ADDRESS);
	LIS3MDL(uint8_t address = BASE_ADDRESS);
#endif

	HAL_StatusTypeDef Init(Scale sensitivity = Scale::G16, uint8_t force = 1);

	void SetScale(Scale sensitivity);

	int16_t RawMX();
	int16_t RawMY();
	int16_t RawMZ();

	float MX();
	float MY();
	float MZ();

	~LIS3MDL();
};

} /* namespace IntroStratLib */

#endif /* LIS3MDL_H_ */
