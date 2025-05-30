// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <detours/Detours.h>
#include <CKPE.Detours.h>

namespace CKPE
{
	std::uintptr_t Detours::DetourJump(std::uintptr_t target, std::uintptr_t destination) noexcept(true)
	{
		return ::Detours::X64::DetourFunction(target, destination, ::Detours::X64Option::USE_REL32_JUMP);
	}

	std::uintptr_t Detours::DetourCall(std::uintptr_t target, std::uintptr_t destination) noexcept(true)
	{
		return ::Detours::X64::DetourFunction(target, destination, ::Detours::X64Option::USE_REL32_CALL);
	}

	std::uintptr_t Detours::DetourVTable(std::uintptr_t target, std::uintptr_t detour, std::uint32_t index) noexcept(true)
	{
		return ::Detours::X64::DetourVTable(target, detour, index);
	}

	std::uintptr_t Detours::DetourIAT(std::uintptr_t module, const std::string_view& import_module,
		const std::string_view& api, std::uintptr_t detour) noexcept(true)
	{
		return ::Detours::IATHook(module, import_module.data(), api.data(), detour);
	}

	std::uintptr_t Detours::DetourIATDelayed(std::uintptr_t module, const std::string_view& import_module,
		const std::string_view& api, std::uintptr_t detour) noexcept(true)
	{
		return ::Detours::IATDelayedHook(module, import_module.data(), api.data(), detour);
	}
}