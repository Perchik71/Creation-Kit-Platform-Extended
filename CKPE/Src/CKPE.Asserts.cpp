// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.ErrorHandler.h>
#include <CKPE.Asserts.h>
#include <format>

namespace CKPE
{
	void Asserts::Trigger(const char* source_file, int source_line, const char* formatted_message, ...) noexcept(true)
	{
		char buffer[1024];
		char message[1024];

		memset(buffer, 0, 1024);
		memset(message, 0, 1024);

		va_list ap;
		va_start(ap, formatted_message);
		vsprintf_s(buffer, _TRUNCATE, formatted_message, ap);
		sprintf_s(message, "%s(%d):\n\n%s", source_file, source_line, buffer);
		va_end(ap);

		ErrorHandler::Trigger(message);
	}
}