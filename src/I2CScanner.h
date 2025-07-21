/*
 * I2CScanner.h
 *
 *  Created on: Mar 6, 2025
 *      Author: User
 */

#ifndef I2CSCANNER_H_
#define I2CSCANNER_H_

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0')

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

	static void printTextToUART(UART_HandleTypeDef*_uart, const char* text, uint16_t len) {
		HAL_UART_Transmit(_uart, (uint8_t*) text, len, 1000);
	}
	static void printTextToUART(UART_HandleTypeDef*_uart, const char* text) {
		printTextToUART(_uart, text, strlen(text));
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

	static void scanDeviceRegisters(I2C_HandleTypeDef *_i2c, uint8_t addr, uint8_t firstRegister, uint8_t nRegisters, UART_HandleTypeDef*_uart) {
		char str[100];
		sprintf(str, "Scanning registers of device at 0x%0X. From 0x%0X to 0x%0X\n\r", addr, firstRegister, firstRegister+nRegisters);
		printTextToUART(_uart, str);

		HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(_i2c, addr << 1, 3,
							200);
		if (status != HAL_OK) {
			printTextToUART(_uart, "Device not found!\n");
			return;
		}
		uint8_t val;
		printTextToUART(_uart, "\t\t\tHEX\t  Binary");
		printTextToUART(_uart, "\t\t\t\t7 | 6 | 5 | 4 | 3 | 2 | 1 | 0");
		for (int i = firstRegister; i < firstRegister + nRegisters; i++) {
			status = HAL_I2C_Mem_Read(_i2c, addr<<1, i, I2C_MEMADD_SIZE_8BIT, &val, 1, 1000);
			if (status) {
				sprintf(str, "\tReg 0x%02X: Error", i);
			} else {
				sprintf(str, "\t Reg 0x%02X: \t0x%02X \t[" BYTE_TO_BINARY_PATTERN "]\n", i, val, BYTE_TO_BINARY(val));
			}

			printTextToUART(_uart, str);
		}
		printTextToUART(_uart, "Register scan Finished!\n");
	}

	static void scanDeviceRegistersBatch(I2C_HandleTypeDef *_i2c, uint8_t addr, uint8_t firstRegister, uint8_t nRegisters, UART_HandleTypeDef*_uart) {
			char str[100];
			sprintf(str, "Batch scanning registers of device at 0x%02X. From 0x%2X to 0x%02X\n\r", addr, firstRegister, firstRegister+nRegisters);
			printTextToUART(_uart, str);

			HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(_i2c, addr << 1, 3,
								200);
			if (status != HAL_OK) {
				printTextToUART(_uart, "Device not found!\n");
				return;
			}
			uint8_t* val = new uint8_t[nRegisters];
			HAL_I2C_Mem_Read(_i2c, addr<<1, firstRegister, I2C_MEMADD_SIZE_8BIT, val, nRegisters, 1000);
			for (int i = 0; i < nRegisters; i++) {
				sprintf(str, "\t Reg 0x%02X: \t0x%02X \t[" BYTE_TO_BINARY_PATTERN "]\n", i+firstRegister, val[i], BYTE_TO_BINARY(val[i]));
				printTextToUART(_uart, str);
			}
			printTextToUART(_uart, "Register scan Finished!\n");
		}
};
}



#endif /* I2CSCANNER_H_ */
