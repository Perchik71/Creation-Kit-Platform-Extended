// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashUploadBNetModWithArchive.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			CrashUploadBNetModWithArchive::CrashUploadBNetModWithArchive() : Common::Patch()
			{
				SetName("Crash Upload BNet Mod With Archive");
			}

			bool CrashUploadBNetModWithArchive::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* CrashUploadBNetModWithArchive::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool CrashUploadBNetModWithArchive::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> CrashUploadBNetModWithArchive::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool CrashUploadBNetModWithArchive::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashUploadBNetModWithArchive::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix crash while trying to upload BNet mods with existing archives
				//
				Detours::DetourJump(__CKPE_OFFSET(0), (std::uintptr_t)&IsBSAVersionCurrent);

				return true;
			}

			bool CrashUploadBNetModWithArchive::IsBSAVersionCurrent(void* File) noexcept(true)
			{
				char fullPath[MAX_PATH];
				GetCurrentDirectory(ARRAYSIZE(fullPath), fullPath);

				strcat_s(fullPath, "\\");
				strcat_s(fullPath, (const char*)((__int64)File + 0x64));

				HANDLE file = CreateFileA(fullPath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
					nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

				if (file != INVALID_HANDLE_VALUE)
				{
					struct
					{
						uint32_t Marker = 0;
						uint32_t Version = 0;
					} header;

					DWORD bytesRead;
					ReadFile(file, &header, sizeof(header), &bytesRead, nullptr);
					CloseHandle(file);

					// LE: Version 0x68
					// SSE: Version 0x69
					if (header.Marker != '\0ASB' || header.Version < 0x69)
						return false;

					return true;
				}

				return false;
			}
		}
	}
}