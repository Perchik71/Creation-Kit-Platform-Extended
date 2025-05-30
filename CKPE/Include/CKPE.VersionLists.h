// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <string>
#include <cstdint>
#include <CKPE.Common.h>

namespace CKPE
{
	class CKPE_API VersionLists
	{
		constexpr VersionLists() noexcept(true) = default;
		VersionLists(const VersionLists&) = delete;
		VersionLists& operator=(const VersionLists&) = delete;
	public:
		enum EDITOR_EXECUTABLE_TYPE : std::uint32_t 
		{
			EDITOR_UNKNOWN,
			EDITOR_SKYRIM_SE_1_5_3,
			EDITOR_SKYRIM_SE_1_5_73,
			EDITOR_SKYRIM_SE_1_6_438,
			EDITOR_SKYRIM_SE_1_6_1130,
			EDITOR_SKYRIM_SE_1_6_1378_1,
			EDITOR_SKYRIM_SE_LAST = EDITOR_SKYRIM_SE_1_6_1378_1,
			EDITOR_FALLOUT_C4_1_10_162_0,
			EDITOR_FALLOUT_C4_1_10_943_1,
			EDITOR_FALLOUT_C4_1_10_982_3,
			EDITOR_FALLOUT_C4_LAST = EDITOR_FALLOUT_C4_1_10_982_3,
			EDITOR_STARFIELD_1_13_61_0,
			EDITOR_STARFIELD_1_14_70_0,
			EDITOR_STARFIELD_1_14_74_0,
			EDITOR_STARFIELD_1_14_78_0,
			EDITOR_STARFIELD_1_15_216_0,
			EDITOR_STARFIELD_LAST = EDITOR_STARFIELD_1_15_216_0,
		};

		enum EDITOR_EXECUTABLE_SHORT_TYPE : std::uint32_t
		{
			EDITOR_SHORT_UNKNOWN,
			EDITOR_SHORT_SKYRIM_SE,
			EDITOR_SHORT_FALLOUT_C4,
			EDITOR_SHORT_STARFIELD,
		};

		static void Verify();
		[[nodiscard]] static bool HasAllowedEditorVersion() noexcept(true);
		[[nodiscard]] static bool HasOutdatedEditorVersion() noexcept(true);
		[[nodiscard]] static EDITOR_EXECUTABLE_SHORT_TYPE GetGameEditor() noexcept(true);
		[[nodiscard]] static std::wstring GetGameShortName() noexcept(true);
		[[nodiscard]] static std::wstring GetDatabaseFileName() noexcept(true);
		[[nodiscard]] static std::wstring GetEditorVersionByString() noexcept(true);
		[[nodiscard]] static std::wstring GetExternalResourcePackageFileName() noexcept(true);
	};
}