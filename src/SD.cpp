/*
 * SD.cpp
 *
 *  Created on: Mar 31, 2025
 *      Author: alesha
 */

#ifndef ARDUINO

#include "SD.h"
#include <cstring>

using namespace IntroStratLib;

SD::SD()
: error(FR_OK)
{
	MX_FATFS_Init();
}

SD::~SD() {
	f_mount(NULL, "", 0);
}

HAL_StatusTypeDef SD::Init() {
	return (error = f_mount(&fatfs, "", 1)) ? HAL_ERROR : HAL_OK;
}

FRESULT SD::GetError() const noexcept {
	return error;
}

SD::File::File(const char* path, uint8_t mode)
: path(path), mode(mode)
{}

FRESULT SD::File::Close() noexcept {
	return f_close(&fil);
}

HAL_StatusTypeDef SD::File::Init() {
	return (error = f_open(&fil, path, mode)) ? HAL_ERROR : HAL_OK;
}

FRESULT SD::File::GetError() const noexcept {
	return error;
}

int32_t SD::File::Write(const void* data, size_t size) noexcept {
	size_t writed;
	FRESULT rez = f_write(&fil, data, size, &writed);
	return rez ? -rez : writed;
}

int32_t SD::File::Write(const char* string) noexcept{
	return Write(string, strlen(string));
}

int32_t SD::File::Write(char* string) noexcept {
	return Write(const_cast<const char*>(string));
}

#endif
