// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include "vassert.h"

#ifdef _DEBUG

#if !defined(_CONSOLE) && (defined(_WIN32) || defined(_WIN64))
#	include <windows.h>
#elif defined(_CONSOLE)
#	include <string>
#	include <iostream>
#endif // defined(_WIN32) || defined(_WIN64)

#include <stdarg.h>
#include <stdio.h>

namespace voltek
{
	namespace core
	{
		namespace _internal
		{
			void kill_this_process()
			{
				*((void**)0) = 0;
			}

			void input_message(const char* msg)
			{
#if !defined(_CONSOLE) && (defined(_WIN32) || defined(_WIN64))
				MessageBoxA(0, msg, "ASSERT", MB_OK | MB_ICONERROR);
#elif defined(_CONSOLE)
				printf("ASSERT\n");
				printf("Message: %s\n", msg);
				std::string dummy;
				std::cout << "Press any key to continue...";
				std::getline(std::cin, dummy);
#endif // defined(_WIN32) || defined(_WIN64)
			}

			void assert(bool cond, const char* msg)
			{
				if (cond) return;
				input_message(msg);
				kill_this_process();
			}

			// https://stackoverflow.com/questions/4785381/replacement-for-ms-vscprintf-on-macos-linux
			int _vscprintf(const char* format, va_list pargs) {
				int retval;
				va_list argcopy;
				va_copy(argcopy, pargs);
				retval = vsnprintf(NULL, 0, format, argcopy);
				va_end(argcopy);
				return retval;
			}

			void assert_format(bool cond, const char* fmt, ...)
			{
				if (cond) return;

				va_list ap;
				va_start(ap, fmt);			
				int len = _vscprintf(fmt, ap);
				if (len > 0)
				{
					char* msg = new char[(size_t)len + 1];
					if (msg)
					{
						if (vsprintf(msg, fmt, ap) > 0)
							input_message(msg);
						delete[] msg;
					}
				}
				va_end(ap);
				kill_this_process();
			}
		}
	}
}

#endif // _DEBUG