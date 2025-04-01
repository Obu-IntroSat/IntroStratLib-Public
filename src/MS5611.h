/*
 * MS5611.h
 *
 *  Created on: Mar 6, 2025
 *      Author: unflesh
 */

#ifndef MS5611_H_
#define MS5611_H_

#include "Barometer.h"
#include "I2CDevice.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"

namespace IntroStratLib {

class MS5611 : public Barometer {

public:

	/* Add OSR to the command REQUEST_PRESSURE or REQUEST_TEMPERATURE */
	typedef enum {
		OSR_256 = 0x00,
		OSR_512 = 0x02,
		OSR_1024 = 0x04,
		OSR_2048 = 0x06,
		OSR_4096 = 0x08
	} OSR;

private:
	typedef enum {
		OK = HAL_OK,
		ERROR = HAL_ERROR,
		BUSY = HAL_BUSY,
		TIMEOUT = HAL_TIMEOUT,
		CRC_ERROR = 0x05,
		ADC_ERROR = 0x06
	} ERROR_TYPE;

	enum CMD {
		RST = 0x1E,
		REQUEST_PROM = 0xA0,
		REQUEST_PRESSURE = 0x40,
		REQUEST_TEMPERATURE = 0x50,
		ADC_READ = 0x00
	};

	/* Use PROM_N enum as an index for _koeff_prom array */
	enum PROM_N {
		RSRV = 0, /* reserved */
		SENS_T1, /* pressure sensitivity */
		OFF_T1, /* pressure offset */
		TCS, /* temperature coefficient of pressure sensitivity */
		TCO, /* temperature coefficient of pressure offset */
		T_REF, /* reference temperature */
		TEMPSENS, /* temperature coefficient of the temperature */
		CRC_4 /* CRC-4 */
	};

	static const uint8_t BASE_ADDRESS = 0x77; /* another available address is 0x76 */

	uint8_t _sensitivity = 0;

	uint16_t _koeff_prom[8]; /* PROM content array */
	uint8_t _crc;

	int32_t dT;
	int32_t TEMP;
	int64_t OFF;
	int64_t SENS;
	int64_t P;
	uint32_t _raw_pressure;
	uint32_t _raw_temperature;
	float _pressure;
	float _temperature;

protected:
	I2CDevice _i2c;

public:

#ifndef ARDUINO
	MS5611(I2C_HandleTypeDef *hi2c, uint8_t address = BASE_ADDRESS);
#else
	MS5611(TwoWire &hi2c, uint8_t address = BASE_ADDRESS);
#endif

#if defined(DEBUG) && !defined(ARDUINO)
	void PROM_Print(UART_HandleTypeDef* uart);
#endif

//	MS5611& operator=(const MS5611 &other);
//	MS5611& operator=(MS5611 &&other);

	uint8_t Init(OSR sensitivity = OSR::OSR_1024, uint8_t force = 1);

	uint8_t CRC_Calculate();
	uint8_t PROM_Read();

	void SetSensitivity(OSR sensitivity);

	uint8_t ADCRead();

	uint32_t GetRawPressure();
	uint32_t GetRawTemperature();

	float GetPressure(); // TODO: if error return NaN
	float GetTemperature();

	~MS5611();
};

} /* namespace IntroStratLib */

#endif /* MS5611_H_ */
