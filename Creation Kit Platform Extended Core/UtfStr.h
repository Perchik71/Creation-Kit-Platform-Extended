// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace Conversion
	{
		bool LazUnicodePluginInit();
		bool IsUtf8Valid(const String& str);
		String Utf16ToUtf8(const WideString& str);
		WideString Utf8ToUtf16(const String& str);
		String Utf8ToAnsi(const String& str);
		String AnsiToUtf8(const String& str);
		String Utf16ToAnsi(const WideString& str);
		WideString AnsiToUtf16(const String& str);
		String WideToAnsi(const WideString& str);
		WideString AnsiToWide(const String& str);
	}
}
