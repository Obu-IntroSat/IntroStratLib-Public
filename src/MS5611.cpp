/*
 * MS5611.cpp
 *
 *  Created on: Mar 10, 2025
 *      Author: unflesh
 */

#include "MS5611.h"

namespace IntroStratLib {

#ifndef ARDUINO
	MS5611::MS5611(I2C_HandleTypeDef *hi2c, uint8_t address):
				_i2c(*new I2CDevice(hi2c, address)) {
	}
#else
	MS5611::MS5611(TwoWire &hi2c, uint8_t address):
				_i2c(*new I2CDevice(hi2c, address)) {
	}
#endif

//MS5611& MS5611::operator =(const MS5611 &other) {
//	if (this != &other) {
//		_i2c = other._i2c;
//		//		delete &other._i2c;
//	}
//	return *this;
//}
//
//MS5611& MS5611::operator =(MS5611 &&other) {
//	if (this != &other) {
//		_i2c = other._i2c;
//		//		delete &other._i2c;
//	}
//	return *this;
//}

uint8_t MS5611::Init(OSR sensitivity, uint8_t force) {
	HAL_StatusTypeDef status = _i2c.isReady(force);
	if (status) {
		return status;
	}
	uint8_t tx_buf = CMD::RST;
	status = _i2c.write(&tx_buf, 1);
	if (status) {
		return status;
	}
	HAL_Delay(5);
	SetSensitivity(sensitivity);
	if (PROM_Read()) {
		return ERROR_TYPE::CRC_ERROR;
	}
	return ERROR_TYPE::OK;
}

uint8_t MS5611::PROM_Read() {
	HAL_StatusTypeDef status;
	for (uint8_t i = 0; i < 8; i++) {
		uint8_t buf[2];
		status = _i2c.read(CMD::REQUEST_PROM + i * 0x02, buf, 2);
		if (status) {
			return status;
		}
		_koeff_prom[i] = buf[0] << 8 | buf[1];
	}
	return CRC_Calculate() ^ (0x000F & _koeff_prom[CRC_4]);
}

uint8_t MS5611::CRC_Calculate() {

	int cnt; /* simple counter */
	unsigned int n_rem; /* crc remainder */
	unsigned int crc_read; /* original value of the crc */
	unsigned char n_bit;
	n_rem = 0x00;
	crc_read = _koeff_prom[CRC_4]; //save read CRC
	_koeff_prom[7] = (0xFF00 & (_koeff_prom[CRC_4])); /* CRC byte is replaced by 0 */
	for (cnt = 0; cnt < 16; cnt++) { /* operation is performed on bytes */
		/* choose LSB or MSB */
		if (cnt % 2 == 1)
			n_rem ^= (unsigned short) ((_koeff_prom[cnt >> 1]) & 0x00FF); /* XOR – CRC calculating */
		else
			n_rem ^= (unsigned short) (_koeff_prom[cnt >> 1] >> 8);
		for (n_bit = 8; n_bit > 0; n_bit--) {
			if (n_rem & (0x8000)) {
				n_rem = (n_rem << 1) ^ 0x3000;
			} else {
				n_rem = (n_rem << 1);
			}
		}
	}
	n_rem = (0x000F & (n_rem >> 12)); /* final 4-bit reminder is CRC code */
	_koeff_prom[CRC_4] = crc_read; /* restore the crc_read to its original place */
	return (n_rem);
}

#if defined(DEBUG) && !defined(ARDUINO)
void MS5611::PROM_Print(UART_HandleTypeDef* uart) {
	char tx_buf[64];
	for (uint8_t i = 0; i < 8; i++) {
		sprintf(tx_buf, "%d\t", _koeff_prom[i]);
		HAL_UART_Transmit(uart, (uint8_t*)tx_buf, strlen(tx_buf), 100);
		for (uint8_t k = 0; k < 64; k++) {
			tx_buf[k] = '\0';
		}
	}
	HAL_UART_Transmit(uart, (uint8_t*)"\n", 1, 100);
}
#endif

void MS5611::SetSensitivity(OSR sensitivity) {
	_sensitivity = sensitivity;
}

uint8_t MS5611::ADCRead() {
	uint8_t tx_buf = CMD::ADC_READ;
	HAL_StatusTypeDef status = _i2c.write(&tx_buf, 1);
	HAL_Delay(10);
	return status;
}

uint32_t MS5611::GetRawTemperature() {
	uint8_t tx_buf = CMD::REQUEST_TEMPERATURE + _sensitivity;
	HAL_StatusTypeDef status;
	status = _i2c.write(&tx_buf, 1);
	HAL_Delay(30);
	if (status) {
		return status;
	}
	else {
		if (ADCRead()) {
			return ERROR_TYPE::ADC_ERROR;
		}
		uint8_t buf[3];
		status = _i2c.read(buf, 3);
		if (status) {
			return status;
		}
		else {
			_raw_temperature = (buf[0] << 16) | (buf[1] << 8) | buf[2];
		}
	}
	return _raw_temperature;
}

float MS5611::GetTemperature() {
	float T2 = 0;
	GetRawTemperature();
	dT = (int32_t)(_raw_temperature) - (int32_t)((_koeff_prom[T_REF] << 8));
	TEMP = 2000 + (dT * (((int32_t)_koeff_prom[TEMPSENS]) / ((float)(0b1 << 23))));
	_temperature = ((float)TEMP) / 100.0f;
	if (_temperature < 20 && _temperature >= -15) {
		T2 = (uint64_t)(dT * dT) / ((float)(0b1 << 31));
	}
	_temperature -= - T2;
	return _temperature;
}

uint32_t MS5611::GetRawPressure() {
	uint8_t tx_buf = CMD::REQUEST_PRESSURE + _sensitivity;
	HAL_StatusTypeDef status;
	status = _i2c.write(&tx_buf, 1);
	HAL_Delay(30);
	if (status) {
		return status;
	}
	else {
		if (ADCRead()) {
			return ERROR_TYPE::ADC_ERROR;
		}
		uint8_t buf[3];
		status = _i2c.read(buf, 3);
		if (status) {
			return status;
		}
		else {
			_raw_pressure = (buf[0] << 16) | (buf[1] << 8) | buf[2];
		}
	}
	return _raw_pressure;
}

float MS5611::GetPressure() {
	float OFF2 = 0;
	float SENS2 = 0;
	GetTemperature();
	GetRawPressure();
	OFF = (((int64_t)_koeff_prom[OFF_T1]) << 16) + (dT * (((int64_t)_koeff_prom[TCO]) / ((float)(0b1 << 7))));
	SENS =  (((int64_t)_koeff_prom[SENS_T1]) << 15) + (dT * (((int64_t)_koeff_prom[TCS]) / ((float)( 0b1 << 8))));
	P = ((_raw_pressure * (((int64_t)SENS) / ((float)(0b1 << 21)))) - OFF) / ((float)(0b1 << 15));
	if (_temperature < 20 && _temperature >= -15) {
		OFF2 = 5 * (_temperature - 2000) * (_temperature - 2000) / 2.0f;
		SENS2 = 5 * (_temperature - 2000) * (_temperature - 2000) / 4.0f;
	}
	else if (_temperature < -15) {
		OFF2 += 7 * (_temperature + 1500)*(_temperature + 1500);
		SENS2 += 11 * (_temperature + 1500)*(_temperature + 1500) / 2.0f;
	}

	OFF -= OFF2;
	SENS -= SENS2;
	P = ((_raw_pressure * (((int64_t)SENS) / ((float)(0b1 << 21)))) - OFF) / ((float)(0b1 << 15));
	_pressure = ((float)P) / 100.0f;
	return _pressure;
}

float MS5611::GetHeight() {
	return 18400.0f * log(P_SEA_LEVEL / GetPressure());
}

MS5611::~MS5611() {
}

} /* namespace IntroStratLib */

