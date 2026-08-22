// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <detours/Detours.h>
#include <CKPE.Detours.h>

namespace CKPE
{
	[[nodiscard]] static std::uintptr_t DetourCheckRel32AndReturnDestAddress(std::uintptr_t a_target) noexcept(true)
	{
		auto target = reinterpret_cast<const std::uint8_t*>(a_target);
		if ((target[0] == 0xE8) || (target[0] == 0xE9))
		{
			auto rel32 = reinterpret_cast<const std::int32_t*>(a_target + 1);
			return a_target + *rel32 + 5;
		}
		else if ((target[0] == 0xFF) && ((target[1] == 0x15) || (target[1] == 0x25)))
		{
			auto rel32 = reinterpret_cast<const std::int32_t*>(a_target + 2);
			return *reinterpret_cast<std::uintptr_t*>(a_target + *rel32 + 6);
		}
		return 0;
	}

	std::uintptr_t Detours::DetourJump(std::uintptr_t target, std::uintptr_t destination) noexcept(true)
	{
		if (!target) return 0;
		auto destAddress = DetourCheckRel32AndReturnDestAddress(target);
		auto detourAddress = ::Detours::X64::DetourFunction(target, destination, ::Detours::X64Option::USE_REL32_JUMP);
		return destAddress ? destAddress : detourAddress;
	}

	std::uintptr_t Detours::DetourCall(std::uintptr_t target, std::uintptr_t destination) noexcept(true)
	{
		if (!target) return 0;
		auto destAddress = DetourCheckRel32AndReturnDestAddress(target);
		auto detourAddress = ::Detours::X64::DetourFunction(target, destination, ::Detours::X64Option::USE_REL32_CALL);
		return destAddress ? destAddress : detourAddress;
	}

	std::uintptr_t Detours::DetourVTable(std::uintptr_t target, std::uintptr_t detour, std::uint32_t index) noexcept(true)
	{
		if (!target) return 0;
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