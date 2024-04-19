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
				_aArgs.push_back(Conversion::Utf16ToUtf8(cmdLine[i]));
		}

		CommandLineParser::~CommandLineParser()
		{
			_aArgs.clear();
		}

		int CommandLineParser::IndexArgWithCommand() const
		{
			if (!_aArgs.size())
				return -1;

			for (size_t i = 0; i < std::min((size_t)2, _aArgs.size()); i++)
			{
				const String* cmd = &(_aArgs[i]);
				if (cmd->empty())
					return -1;

				if (cmd->c_str()[0] == '-')
					return (int)i;
			}

			return -1;
		}

		String CommandLineParser::GetCommand() const
		{
			auto Id = IndexArgWithCommand();
			return (Id != -1) ? _aArgs[Id] : "";
		}
	}
}