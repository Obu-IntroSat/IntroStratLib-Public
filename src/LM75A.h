/*
 * LM75A.h
 *
 *  Created on: Mar 4, 2025
 *      Author: Aleksey <TeaCupMe> Gilenko
 */

#ifndef LM75A_H_
#define LM75A_H_
#include "TemperatureSensor.h"
#include "RadioTransceiver.h"
#include "I2CDevice.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"


namespace IntroStratLib {



class LM75A: public BaseTemperatureSensor {
private:
	enum RegisterMap: uint8_t {
			TEMPERATURE = 0x00,
			CONFIGURATION = 0x01,
			T_HYST = 0x02,
			T_OS = 0x03
		};
//	enum C: uint8_t {
//
//	};


	static const uint8_t BASE_ADDRESS = 0x48;
	void setRegisterPointer(RegisterMap reg);


protected:
	I2CDevice _i2c;

public:
#ifndef ARDUINO
	LM75A(I2C_HandleTypeDef *hi2c, uint8_t address = BASE_ADDRESS);
#else
	LM75A(TwoWire &hi2c, uint8_t address = BASE_ADDRESS);
#endif

	LM75A(const LM75A &other);
	LM75A(LM75A &&other);
	LM75A& operator=(const LM75A &other);
	LM75A& operator=(LM75A &&other);

	HAL_StatusTypeDef Init();

	uint16_t GetRawTemperature(); // TODO: make private
	uint16_t GetTemperatureTimes8();

	float GetTemperature() override;
//	float GetTemperatureC() override;
//	float GetTemperatureF() override;
	uint8_t GetConfig();
//	void Enable();
	void PowerDown(bool shutdown);
	void DumpData(UART_HandleTypeDef* _uart);
//	void DumpData(void (*transmitMethod)(uint8_t* str, uint16_t len));
	void DumpData(RadioTransceiver* radio);

};


}




#endif /* LM75A_H_ */
