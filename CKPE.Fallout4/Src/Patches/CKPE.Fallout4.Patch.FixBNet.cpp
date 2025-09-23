// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Asserts.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.FixBNet.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			FixBNet::FixBNet() : Common::Patch()
			{
				SetName("Fix crash when Unicode string BNet");
			}

			bool FixBNet::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixBNet::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixBNet::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixBNet::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixBNet::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixBNet::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// Fix crash when Unicode string conversion fails with bethesda.net http responses
				Detours::DetourJump(__CKPE_OFFSET(0), (std::uintptr_t)&sub);

				return true;
			}

			std::size_t FixBNet::sub(char* Destination, std::size_t DestSize, const wchar_t* Source, 
				std::size_t SourceSize) noexcept(true)
			{
				if (Destination)
					memset(Destination, 0, DestSize);

				std::size_t outLen;
				errno_t result = wcsrtombs_s(&outLen, Destination, DestSize, &Source, SourceSize, nullptr);

				if (result != 0)
				{
					// Try a fall back instead of normally returning -1
					outLen = WideCharToMultiByte(CP_UTF8, 0, Source, (int)SourceSize, Destination, (int)DestSize, nullptr, nullptr);

					if (Destination && DestSize > 0)
						Destination[DestSize - 1] = '\0';

					CKPE_ASSERT_MSG(outLen != 0, "Unicode conversion failed");
				}

				return outLen - 1;
			}
		}
	}
}