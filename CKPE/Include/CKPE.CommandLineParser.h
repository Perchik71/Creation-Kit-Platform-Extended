// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <CKPE.Common.h>

namespace CKPE
{
	class CKPE_API CommandLineParser
	{
		std::vector<std::wstring>* _aArgs;

		CommandLineParser(const CommandLineParser&) = delete;
		CommandLineParser& operator=(const CommandLineParser&) = delete;
	public:
		CommandLineParser();
		virtual ~CommandLineParser() noexcept(true);
	
		[[nodiscard]] inline virtual constexpr std::wstring At(std::uint32_t nId) const noexcept(true) { return (*_aArgs)[nId]; }
		[[nodiscard]] inline virtual constexpr std::uint32_t Count() const noexcept(true) { return (std::uint32_t)_aArgs->size(); }
		[[nodiscard]] inline virtual constexpr std::wstring operator[](std::uint32_t nId) const noexcept(true) { return At(nId); }
		[[nodiscard]] virtual std::int32_t IndexArgWithCommand() const noexcept(true);
		[[nodiscard]] inline virtual constexpr bool HasCommandRun() const noexcept(true) { return IndexArgWithCommand() != -1; }
		[[nodiscard]] virtual std::wstring GetCommand() const noexcept(true);
	};
}