// Copyright © 2025-2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <vector>
#include <array>
#include <unordered_map>
#include <windows.h>
#include <CKPE.Application.h>
#include <CKPE.Common.AddressLibrary.h>
#include <CKPE.Fallout4.VersionLists.h>

#include <atomic>
#include <array>

namespace CKPE
{
	namespace Fallout4
	{
		std::atomic<int8_t> global_RuntimeIndex{ -1 };

		struct Runtime
		{
			VersionLists::EDITOR_EXECUTABLE_TYPE editor;
			std::initializer_list<std::uint32_t> crc32AllowedEditorVersion;
			std::uint32_t offsetForChecks;
			std::string_view stringForChecks;
			CKPE::Version version;
			std::wstring_view versionStr;

			[[nodiscard]] inline bool Crc32Matches(std::uint32_t a_crc32) const noexcept(true)
			{
				return std::find(crc32AllowedEditorVersion.begin(), crc32AllowedEditorVersion.end(), a_crc32)
					!= crc32AllowedEditorVersion.end();
			}

			[[nodiscard]] bool CheckBinary() const noexcept(true)
			{
				__try
				{
					return !strcmp(reinterpret_cast<const char*>(
						reinterpret_cast<std::uintptr_t>(GetModuleHandleA(nullptr)) + offsetForChecks),
						stringForChecks.data());
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					return false;
				}
			}
		};

		constexpr Runtime Runtime_OG{
			VersionLists::EDITOR_FALLOUT_C4_1_10_162_0,
			{ 0xDF67F346ul },								// CRC32
			0x3896168ul,
			"1.10.162.0"sv,
			{ 1, 10, 162, 0 },
			L"Fallout 4 [v1.10.162.0]"sv,
		};

		constexpr Runtime Runtime_NG{
			VersionLists::EDITOR_FALLOUT_C4_1_10_982_3,
			{ 0x481CCE95ul, 0x55F7F580ul },					// CRC32
			0x2F8D298ul,
			"1.10.982.3"sv,
			{ 1, 10, 982, 3 },
			L"Fallout 4 [v1.10.982.3]"sv,
		};

		constexpr Runtime Runtime_AE{
			VersionLists::EDITOR_FALLOUT_C4_1_11_137_0,
			{ 0x49E45284ul },								// CRC32
			0x3017E68ul,
			"1.11.137.0"sv,
			{ 1, 11, 137, 0 },
			L"Fallout 4 [v1.11.137.0]"sv,
		};

		constexpr Runtime Runtime_AE2{
			VersionLists::EDITOR_FALLOUT_C4_1_11_240_0,
			{ 0x94FA23E9ul },								// CRC32
			0x30284D8ul,
			"1.11.240.0"sv,
			{ 1, 11, 240, 0 },
			L"Fallout 4 [v1.11.240.0]"sv,
		};

		constexpr Runtime Runtime_AE3{
			VersionLists::EDITOR_FALLOUT_C4_1_11_246_0,
			{ 0x3E8BBBE7ul },								// CRC32
			0x30294D8ul,
			"1.11.246.0"sv,
			{ 1, 11, 246, 0 },
			L"Fallout 4 [v1.11.246.0]"sv,
		};

		constexpr Runtime Runtime_Latest = Runtime_AE3;
			
		constexpr std::array<Runtime, 4> global_RuntimeList
		{ Runtime_OG, Runtime_NG, Runtime_AE, Runtime_AE3 };

		void VersionLists::Verify()
		{
			auto crc32 = Application::GetSingleton()->GetFileCRC32();
			for (std::int8_t runtimeIdx = 0; runtimeIdx < global_RuntimeList.size(); runtimeIdx++)
			{
				const auto& runtime = global_RuntimeList[runtimeIdx];
				if (runtime.Crc32Matches(crc32) || runtime.CheckBinary())
				{
					global_RuntimeIndex.store(runtimeIdx);
					break;
				}
			}
		}

		bool VersionLists::HasAllowedEditorVersion() noexcept(true)
		{
			if (HasOutdatedEditorVersion())
				return false;

			auto idx = GetRuntimeIndex();
			return idx != 0xFF;
		}

		bool VersionLists::HasOutdatedEditorVersion() noexcept(true)
		{
			auto idx = GetRuntimeIndex();
			auto ver = Application::GetSingleton()->GetFileVersion();
			return (idx == 0xFF && (ver.value_or<CKPE::Version>({}) <= Runtime_Latest.version));
		}

		std::wstring VersionLists::GetGameName() noexcept(true)
		{
			return L"FO4";
		}

		std::uint8_t VersionLists::GetRuntimeIndex() noexcept(true)
		{
			return static_cast<uint8_t>(global_RuntimeIndex.load());
		}

		CKPE::Version VersionLists::GetEditorVersionByNum() noexcept(true)
		{
			auto idx = GetRuntimeIndex();
			constexpr CKPE::Version unknown{};
			return idx != 0xFF ? global_RuntimeList[idx].version : unknown;
		}

		std::wstring VersionLists::GetEditorVersionByString() noexcept(true)
		{
			auto idx = GetRuntimeIndex();
			return idx != 0xFF ? global_RuntimeList[idx].versionStr.data() : L"Unknown version";
		}

		std::wstring VersionLists::GetExternalResourcePackageFileName() noexcept(true)
		{
			return L"CreationKitPlatformExtended_FO4_Resources.pak";
		}

		VersionLists::EDITOR_EXECUTABLE_TYPE VersionLists::GetEditorVersion() noexcept(true)
		{
			auto idx = GetRuntimeIndex();
			return idx != 0xFF ? global_RuntimeList[idx].editor : EDITOR_EXECUTABLE_TYPE::EDITOR_UNKNOWN;
		}
	}
}
