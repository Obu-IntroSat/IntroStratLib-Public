/*
 * SD.h
 *
 *  Created on: Mar 30, 2025
 *      Author: alesha
 */

#ifndef SD_H_
#define SD_H_

#include "Device.h"

extern "C" {
	#include "SD/FATFS/App/fatfs.h"
}

namespace IntroStratLib {

class SD : public Device
{
	FATFS fatfs;
	FRESULT error;

  public:
	SD() noexcept;
	~SD();

	HAL_StatusTypeDef Init() noexcept;
	FRESULT GetError() const noexcept;

	class File {
		FIL fil;
		const char* const path;
		const uint8_t mode;
		FRESULT error;
	  public:
		File(const char* path, uint8_t mode); // For mode use FA_READ, FA_WRITE ... (SD/FATFS/FatFs/src/ff.h:325)
		~File();

		HAL_StatusTypeDef Init() noexcept;
		FRESULT GetError() const noexcept;

		int32_t Write(const void* data, size_t size) noexcept;

		int32_t Write(const char* string) noexcept;
		int32_t Write(char* string) noexcept;

		template <class T>
		inline int32_t Write(const T& data) noexcept;

		template <class T>
		int32_t Write(const T*) = delete;

		template <class T>
		int32_t Write(T*) = delete;

		FRESULT Close() noexcept;
	};
};

template <class T>
int32_t SD::File::Write(const T& data) noexcept {
	return Write(data, sizeof(T));
}

}

#endif /* SD_H_ */
