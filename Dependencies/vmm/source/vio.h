// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#ifndef VMMDLL_EXPORTS

#include <stddef.h>

namespace voltek
{
	namespace core
	{
		namespace _internal
		{
			// Запись в файл память.
			bool memory_to_file(const char* filename, void* memory, size_t size, size_t blocksize = 4096);
			// Открытие файла. Возвращает дискриптор файла.
			void* open_file(const char* filename, const char* mode);
			// Запись в файл.
			size_t write_file(void* handle, const void* memory, size_t offset, size_t size);
			// Закрытие файла и дискриптора.
			void close_file(void* handle);
		}
	}
}

#endif !VMMDLL_EXPORTS