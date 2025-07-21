/*
 * LM75A.cpp
 *
 *  Created on: Mar 6, 2025
 *      Author: Aleksey <TeaCupMe> Gilenko
 */

#include "LM75A.h"
namespace IntroStratLib {

HAL_StatusTypeDef LM75A::Init() {
	return _i2c.isReady();
}

void LM75A::PowerDown(bool shutdown) {

}

#ifndef ARDUINO
LM75A::LM75A(I2C_HandleTypeDef *hi2c, uint8_t address) :
		_i2c(*new I2CDevice(hi2c, address)) {
}
#else
LM75A::LM75A(TwoWire &hi2c, uint8_t address) :
		_i2c(*new I2CDevice(hi2c, address)) {
}
#endif

LM75A::LM75A(const LM75A &other) :
		_i2c(other._i2c) {
}

LM75A::LM75A(LM75A &&other) :
		_i2c(other._i2c) {
}

LM75A& LM75A::operator =(const LM75A &other) {
	if (this != &other) {
		_i2c = other._i2c;
		//		delete &other._i2c;
	}
	return *this;
}

LM75A& LM75A::operator =(LM75A &&other) {
	if (this != &other) {
		_i2c = other._i2c;
		//		delete &other._i2c;
	}
	return *this;
}

float LM75A::GetTemperature() {
//	setRegisterPointer(RegisterMap::TEMPERATURE);

	uint8_t raw[2];
	_i2c.read((uint8_t*) &raw, 2);
	int16_t data = raw[0] << 8 | raw[1];
	return ((float) data) / 256.0;
}

uint16_t LM75A::GetTemperatureTimes8() {
	uint8_t raw[2];
	_i2c.read((uint8_t*) &raw, 2);
	int16_t data = raw[0] << 8 | raw[1];
	return data / 32;
}

uint8_t LM75A::GetConfig() {
	setRegisterPointer(RegisterMap::CONFIGURATION);
	uint8_t data = 0;
	_i2c.read((uint8_t*) &data, 1);
	setRegisterPointer(RegisterMap::TEMPERATURE);

	return data;
}

void LM75A::setRegisterPointer(RegisterMap reg) {
	uint8_t data = reg;
	_i2c.write(&data, 1);
}

uint16_t LM75A::GetRawTemperature() {
	uint16_t data = 0;
	_i2c.read((uint8_t*) &data, 2);
	return data;
}

void LM75A::DumpData(UART_HandleTypeDef* _uart) {
	char tx_buf[40] = {0};
	sprintf(tx_buf, "lm75ad init: %d\n\r", Init());
	HAL_UART_Transmit(_uart, (uint8_t *)tx_buf, strlen(tx_buf), 300);
	sprintf(tx_buf, "Raw temperature: %X\n\r", GetRawTemperature());
	HAL_UART_Transmit(_uart, (uint8_t *)tx_buf, strlen(tx_buf), 300);
	sprintf(tx_buf, "Temperature: %f\n\r",  GetTemperature());
	HAL_UART_Transmit(_uart, (uint8_t *)tx_buf, strlen(tx_buf), 300);
	sprintf(tx_buf, "Temperature in F: %f\n\r",  GetTemperatureF());
	HAL_UART_Transmit(_uart, (uint8_t *)tx_buf, strlen(tx_buf), 300);
	sprintf(tx_buf, "Temperature times 8: %d\n\r",  GetTemperatureTimes8());
	HAL_UART_Transmit(_uart, (uint8_t *)tx_buf, strlen(tx_buf), 300);
	sprintf(tx_buf, "Config: %X\n\r",  GetConfig());
	HAL_UART_Transmit(_uart, (uint8_t *)tx_buf, strlen(tx_buf), 300);
//	HAL_Delay(1000);
}

//void LM75A::DumpData(void (*transmitMethod)(uint8_t *str, uint16_t len)) {
//	char tx_buf[40] = {0};
//	sprintf(tx_buf, "lm75ad init: %d\n\r", Init());
//	transmitMethod((uint8_t *)tx_buf, strlen(tx_buf));
//	sprintf(tx_buf, "Raw temperature: %X\n\r", GetRawTemperature());
//	transmitMethod((uint8_t *)tx_buf, strlen(tx_buf));
//	sprintf(tx_buf, "Temperature: %f\n\r",  GetTemperature());
//	transmitMethod((uint8_t *)tx_buf, strlen(tx_buf));
//	sprintf(tx_buf, "Temperature in F: %f\n\r",  GetTemperatureF());
//	transmitMethod((uint8_t *)tx_buf, strlen(tx_buf));
//	sprintf(tx_buf, "Temperature times 8: %d\n\r",  GetTemperatureTimes8());
//	transmitMethod((uint8_t *)tx_buf, strlen(tx_buf));
//	sprintf(tx_buf, "Config: %X\n\r",  GetConfig());
//	transmitMethod((uint8_t *)tx_buf, strlen(tx_buf));
////	HAL_Delay(1000);
//}


}
