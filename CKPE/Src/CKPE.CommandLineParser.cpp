// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <windows.h>
#include <shellapi.h>
#include <CKPE.Exception.h>
#include <CKPE.CommandLineParser.h>
#include <algorithm>

namespace CKPE
{
	CommandLineParser::CommandLineParser() :
		_aArgs(new std::vector<std::wstring>)
	{
		if (!_aArgs)
			throw RuntimeError("Fatal create array args cmd line");

		std::int32_t argc = 0;
		auto lpstrCommandLine = GetCommandLineW();
		auto cmdLine = CommandLineToArgvW(lpstrCommandLine, &argc);

		for (std::int32_t i = 1; i < argc; i++)
			_aArgs->push_back(cmdLine[i]);
	}

	CommandLineParser::~CommandLineParser() noexcept(true)
	{
		if (_aArgs)
		{
			delete _aArgs;
			_aArgs = nullptr;
		}
	}

	std::int32_t CommandLineParser::IndexArgWithCommand() const noexcept(true)
	{
		if (!_aArgs || !_aArgs->size())
			return -1;

		for (size_t i = 0; i < std::min((size_t)2, _aArgs->size()); i++)
		{
			const std::wstring* cmd = &((*_aArgs)[i]);
			if (cmd->empty())
				return -1;

			if (cmd->c_str()[0] == L'-')
				return (std::int32_t)i;
		}

		return -1;
	}

	std::wstring CommandLineParser::GetCommand() const noexcept(true)
	{
		if (!_aArgs) return L"";
		auto Id = IndexArgWithCommand();
		return (Id != -1) ? At(Id) : L"";
	}
}