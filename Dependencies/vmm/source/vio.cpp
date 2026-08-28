// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#ifndef VMMDLL_EXPORTS

#if defined(_MSC_VER)
// C4996 'vsprintf': This function or variable may be unsafe. Consider using vsprintf_s instead.To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
#pragma warning(disable : 4996)
#endif

#include "vio.h"
#include <stdio.h>

namespace voltek
{
	namespace core
	{
		namespace _internal
		{
			bool memory_to_file(const char* filename, void* memory, size_t size, size_t blocksize)
			{
				if (!filename || !memory) 
					return false;

				FILE* f = fopen(filename, "wb");
				if (!f) 
					return false;

				bool ret = true;

				// Записываем в файл по "blocksize" байт за итерацию

				size_t alloc_size = size;
				size_t wsize = blocksize > alloc_size ? alloc_size : blocksize;
				size_t write = 0;

				while (write < alloc_size)
				{
					if (fwrite((char*)memory + write, 1, wsize, f) != wsize)
					{
						ret = false;
						break;
					}
					write += wsize;
					wsize = blocksize > (alloc_size - write) ? alloc_size - write : blocksize;
				}

				// Закрытие файла
				fclose(f);
				return ret;
			}

			// Открытие файла. Возвращает дискриптор файла.
			void* open_file(const char* filename, const char* mode)
			{
				if (!filename || !mode)
					return nullptr;

				return (void*)fopen(filename, mode);
			}
			// Запись в файл.
			size_t write_file(void* handle, const void* memory, size_t offset, size_t size)
			{
				return fwrite((const char*)memory + offset, 1, size, (FILE*)handle);
			}
			// Закрытие файла и дискриптора.
			void close_file(void* handle)
			{
				fclose((FILE*)handle);
			}
		}
	}
}

#if defined(_MSC_VER)
#pragma warning(default : 4996)
#endif

#endif !VMMDLL_EXPORTS