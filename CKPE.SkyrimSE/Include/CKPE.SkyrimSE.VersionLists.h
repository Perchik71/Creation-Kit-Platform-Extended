// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <string>
#include <cstdint>

namespace CKPE
{
	namespace SkyrimSE
	{
		class VersionLists
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
			};

			static void Verify();
			[[nodiscard]] static bool HasAllowedEditorVersion() noexcept(true);
			[[nodiscard]] static bool HasOutdatedEditorVersion() noexcept(true);
			[[nodiscard]] static std::wstring GetGameName() noexcept(true);
			[[nodiscard]] static std::wstring GetDatabaseFileName() noexcept(true);
			[[nodiscard]] static std::wstring GetEditorVersionByString() noexcept(true);
			[[nodiscard]] static std::wstring GetExternalResourcePackageFileName() noexcept(true);
			[[nodiscard]] static EDITOR_EXECUTABLE_TYPE GetEditorVersion() noexcept(true);
		};
	}
}