// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "CommandLineParser.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		CommandLineParser::CommandLineParser()
		{
			int argc = 0;
			auto lpstrCommandLine = GetCommandLineW();
			auto cmdLine = CommandLineToArgvW(lpstrCommandLine, &argc);

			for (int i = 1; i < argc; i++)
				_aArgs.push_back(CreationKitPlatformExtended::Utils::Wide2Utf8(cmdLine[i]));
		}

		CommandLineParser::~CommandLineParser()
		{
			_aArgs.clear();
		}
	}
}