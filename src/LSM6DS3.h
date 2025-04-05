/*
 * LSM6DS3.h
 *
 *  Created on: Mar 17, 2025
 *      Author: Goldfor / vlafes / unflesh
 */

#ifndef LSM6DS3_H_
#define LSM6DS3_H_

#include "I2CDevice.h"
#include "Gyroscope.h"
#include "Accelerometer.h"

namespace IntroStratLib {

class LSM6DS3: public Gyroscope, public Accelerometer {
private:

	static const uint8_t BASE_ADDRESS = 0x6A;
	static constexpr float _rawrps = (8.75f / 1000.f) * 3.1415926f / 180.0f; // radians per second
	static constexpr float _rawdps = (8.75f / 1000.f); // degrees per second
	static constexpr float _rawg = (0.061f / 1000.f); // fraction of 'g'

	enum RegisterMap {
		GYRO_CONFIG = 0X11,
		GYRO_XOUT_H = 0X22,
		GYRO_XOUT_L,
		GYRO_YOUT_H,
		GYRO_YOUT_L,
		GYRO_ZOUT_H,
		GYRO_ZOUT_L,
		WHO_AM_I_REG = 0X0F,
		CTRL1_XL = 0X10,
		CTRL8_XL = 0X17,
		OUT_TEMP_L = 0X20,
		OUT_TEMP_H = 0X21,
		OUTX_L_XL = 0X28,
		OUTX_H_XL = 0X29,
		OUTY_L_XL = 0X2A,
		OUTY_H_XL = 0X2B,
		OUTZ_L_XL = 0X2C,
		OUTZ_H_XL = 0X2D,
	};

	static float cutMin(float value, float cut);

	uint8_t WhoAmI();

	uint8_t _sensitivityGyro = 0;
	uint8_t _sensitivityAccel = 0;
	uint8_t _dataRateGyro = 0;
	uint8_t _dataRateAccel = 0;

//	float _cutX = 0.0872665; // rps
//	float _cutY = 0.0872665;
//	float _cutZ = 0.0872665;

	float _cutX = 5.0f; // dps
	float _cutY = 5.0f;
	float _cutZ = 5.0f;

protected:
	I2CDevice _i2c;

public:
	enum DataRateGyro {
		OFF_G,
		DR_G_13_Hz,
		DR_G_26_Hz,
		DR_G_52_Hz,
		DR_G_104_Hz, // - reset value
		DR_G_208_Hz,
		DR_G_416_Hz,
		DR_G_833_Hz,
		DR_G_1660_Hz,
	};

	enum DataRateAccel {
		OFF_A,
		DR_A_12_5_Hz,      //12.5Hz
		DR_A_26_Hz,        //26Hz
		DR_A_52_Hz,        //52Hz
		DR_A_104_Hz,       //104Hz - reset value
		DR_A_208_Hz,       //208Hz
		DR_A_416_Hz,       //416Hz
		DR_A_833_Hz,       //833Hz
		DR_A_1660_Hz,     //1.66kHz
		DR_A_3330_Hz,     //3.33kHz
		DR_A_6660_Hz,    //6.66kHz
    };

	enum ScaleGyro {
		DPS0250 = 0,
		DPS0500, // - reset value
		DPS1000,
		DPS2000
	};

	enum ScaleAccel {
		twoG = 0,
		fourG = 2, // - reset value
		eightG = 3,
		sixteenG = 1
	};

#ifndef ARDUINO
	LSM6DS3(I2C_HandleTypeDef *hi2c, uint8_t address = BASE_ADDRESS);
#else
	LSM6DS3(TwoWire &hi2c, uint8_t address = BASE_ADDRESS);
	LSM6DS3(uint8_t address = BASE_ADDRESS);
#endif

	uint8_t InitGyro(uint8_t force = 1); // TODO: add filter to Init() of gyro&accel
	uint8_t InitGyro(ScaleGyro sensitivityGyro, uint8_t force = 1);
	uint8_t InitGyro(ScaleGyro sensitivityGyro, DataRateGyro dataRateGyro, uint8_t force = 1);
	uint8_t InitAccel(uint8_t force = 1);
	uint8_t InitAccel(ScaleAccel sensitivityAccel, uint8_t force = 1);
	uint8_t InitAccel(ScaleAccel sensitivityAccel, DataRateAccel dataRateAccel, uint8_t force = 1);

	void SetScaleGyro(ScaleGyro sensitivityGyro);
	void SetScaleAccel(ScaleAccel sensitivityAccel);
	void SetDataRateGyro(DataRateGyro dataRateGyro);
	void SetDataRateAccel(DataRateAccel dataRateAccel);

public:
	int16_t RawGX() override;
	int16_t RawGY() override;
	int16_t RawGZ() override;

	float GX() override;
	float GY() override;
	float GZ() override;

	int16_t RawAX() override;
	int16_t RawAY() override;
	int16_t RawAZ() override;

	float AX() override;
	float AY() override;
	float AZ() override;

	void SetMinCutX(float x);
	void SetMinCutY(float y);
	void SetMinCutZ(float z);

    float Temp();

	~LSM6DS3();
};

} /* namespace IntroSatLib */

#endif /* LSM6DS3_H_ */
