/*
 * I2CScanner.h
 *
 *  Created on: Mar 6, 2025
 *      Author: User
 */

#ifndef I2CSCANNER_H_
#define I2CSCANNER_H_

#include "stdio.h"
#include "stdint.h"
#include "string.h"

namespace IntroStratLib {
class I2CScanner {
private:
	static void helloMessage(UART_HandleTypeDef* _uart) {
		char tx_buf[30] = {0};
		sprintf(tx_buf, "\n\rI2C Scanner By TeaCupMe\n\r");
		HAL_UART_Transmit(_uart, (uint8_t*) tx_buf, strlen(tx_buf), 200);
	}

public:
	static uint8_t checkAddr7bit(I2C_HandleTypeDef* _i2c, uint8_t addr) {
		return HAL_I2C_IsDeviceReady(_i2c, addr<<1, 3, 200);
	}

	static uint8_t checkAddr8bit(I2C_HandleTypeDef *_i2c, uint8_t addr) {
		return HAL_I2C_IsDeviceReady(_i2c, addr, 3, 200);
	}

	static void scanI2CFull(I2C_HandleTypeDef *_i2c, UART_HandleTypeDef *_uart) {
		for (int i = 1; i<127; i++) {
			char tx_buf[30] = {0};
			sprintf(tx_buf, "Device at 0x%X: ", i);
			HAL_UART_Transmit(_uart, (uint8_t*) tx_buf, strlen(tx_buf), 200);
			HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(_i2c, i, 3, 200);
			switch (status) {
			case HAL_OK:
				sprintf(tx_buf, "HAL_OK\n\r");
				break;
			case HAL_ERROR:
				sprintf(tx_buf, "HAL_ERROR\n\r");
				break;
			case HAL_BUSY:
				sprintf(tx_buf, "HAL_BUSY\n\r");
				break;
			case HAL_TIMEOUT:
				sprintf(tx_buf, "HAL_TIMEOUT\n\r");
				break;
			default:
				sprintf(tx_buf, "def\n\r");
				break;
			}
			HAL_UART_Transmit(_uart, (uint8_t*) tx_buf, strlen(tx_buf), 200);

		}
	}
	static void scanI2C(I2C_HandleTypeDef *_i2c, UART_HandleTypeDef *_uart) {
		helloMessage(_uart);
		for (int i = 1; i < 127; i++) {
			char tx_buf[30] = { 0 };
			HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(_i2c, i << 1, 3,
					200);
			if (status == HAL_OK) {
				sprintf(tx_buf, "Device found at 0x%X\n\r", i);
				HAL_UART_Transmit(_uart, (uint8_t*) tx_buf, strlen(tx_buf), 200);
			}
		}
	}
};
}



#endif /* I2CSCANNER_H_ */
