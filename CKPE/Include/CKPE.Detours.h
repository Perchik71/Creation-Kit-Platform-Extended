// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <string_view>
#include <cstdint>
#include <CKPE.Common.h>

namespace CKPE
{
	class CKPE_API Detours
	{
		constexpr Detours() noexcept(true) = default;
		Detours(const Detours&) = delete;
		Detours& operator=(const Detours&) = delete;
	public:
		static std::uintptr_t DetourJump(std::uintptr_t target, std::uintptr_t destination) noexcept(true);
		static std::uintptr_t DetourCall(std::uintptr_t target, std::uintptr_t destination) noexcept(true);
		static std::uintptr_t DetourVTable(std::uintptr_t target, std::uintptr_t detour, std::uint32_t index) noexcept(true);

		template<typename T>
		inline static std::uintptr_t DetourClassJump(std::uintptr_t target, T detour) noexcept(true)
		{
			return DetourJump(target, *(std::uintptr_t*)&detour);
		}

		template<typename T>
		inline static std::uintptr_t DetourClassCall(std::uintptr_t target, T detour) noexcept(true)
		{
			return DetourCall(target, *(std::uintptr_t*)&detour);
		}

		template<typename T>
		inline static std::uintptr_t DetourClassVTable(std::uintptr_t target, T detour, std::uint32_t index) noexcept(true)
		{
			return DetourVTable(target, *(std::uintptr_t*)&detour, index);
		}

		static std::uintptr_t DetourIAT(std::uintptr_t module, const std::string_view& import_module,
			const std::string_view& api, std::uintptr_t detour) noexcept(true);
		static std::uintptr_t DetourIATDelayed(std::uintptr_t module, const std::string_view& import_module,
			const std::string_view& api, std::uintptr_t detour) noexcept(true);

		template<typename T>
		inline static std::uintptr_t DetourClassIAT(std::uintptr_t module, const std::string_view& import_module,
			const std::string_view& api, T detour) noexcept(true)
		{
			return DetourIAT(module, import_module, api, *(std::uintptr_t*)&detour);
		}

		template<typename T>
		inline static std::uintptr_t DetourClassIATDelayed(std::uintptr_t module, const std::string_view& import_module,
			const std::string_view& api, T detour) noexcept(true)
		{
			return DetourIATDelayed(module, import_module, api, *(std::uintptr_t*)&detour);
		}
	};
}