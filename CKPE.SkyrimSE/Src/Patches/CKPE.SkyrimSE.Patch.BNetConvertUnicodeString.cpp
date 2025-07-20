// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.Asserts.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.BNetConvertUnicodeString.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			BNetConvertUnicodeString::BNetConvertUnicodeString() : Common::Patch()
			{
				SetName("BNet Convert Unicode String");
			}

			bool BNetConvertUnicodeString::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* BNetConvertUnicodeString::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool BNetConvertUnicodeString::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> BNetConvertUnicodeString::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool BNetConvertUnicodeString::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool BNetConvertUnicodeString::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				Detours::DetourJump(__CKPE_OFFSET(0), (std::uintptr_t)&Convert);

				return true;
			}

			std::size_t BNetConvertUnicodeString::Convert(char* Destination, std::size_t DestSize,
				const wchar_t* Source, std::size_t SourceSize) noexcept(true)
			{
				if (Destination)
					memset(Destination, 0, DestSize);

				size_t outLen;
				errno_t result = wcsrtombs_s(&outLen, Destination, DestSize, &Source, SourceSize, nullptr);

				if (result != 0)
				{
					// Try a fall back instead of normally returning -1
					outLen = WideCharToMultiByte(CP_UTF8, 0, Source, (int)SourceSize, Destination,
						(int)DestSize, nullptr, nullptr);

					if (Destination && DestSize > 0)
						Destination[DestSize - 1] = '\0';

					CKPE_ASSERT_MSG(outLen != 0, "Unicode conversion failed");
				}

				return outLen - 1;
			}
		}
	}
}