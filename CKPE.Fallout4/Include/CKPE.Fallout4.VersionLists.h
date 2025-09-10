// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <string>
#include <cstdint>

namespace CKPE
{
	namespace Fallout4
	{
		class VersionLists
		{
			constexpr VersionLists() noexcept(true) = default;
			VersionLists(const VersionLists&) = delete;
			VersionLists& operator=(const VersionLists&) = delete;
		public:
			enum EDITOR_EXECUTABLE_TYPE : std::uint32_t
			{
				EDITOR_UNKNOWN = 0,
				EDITOR_FALLOUT_C4_1_10_162_0,
				EDITOR_FALLOUT_C4_1_10_943_1,
				EDITOR_FALLOUT_C4_1_10_982_3,
				EDITOR_FALLOUT_C4_LAST = EDITOR_FALLOUT_C4_1_10_982_3,
			};

			static void Verify();
			[[nodiscard]] static bool HasAllowedEditorVersion() noexcept(true);
			[[nodiscard]] static bool HasOutdatedEditorVersion() noexcept(true);
			[[nodiscard]] static std::wstring GetGameName() noexcept(true);
			[[nodiscard]] static std::wstring GetDatabaseFileName() noexcept(true);
			[[nodiscard]] static std::uint64_t GetEditorVersionByNum() noexcept(true);
			[[nodiscard]] static std::wstring GetEditorVersionByString() noexcept(true);
			[[nodiscard]] static std::wstring GetExternalResourcePackageFileName() noexcept(true);
			[[nodiscard]] static EDITOR_EXECUTABLE_TYPE GetEditorVersion() noexcept(true);
		};
	}
}