// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "CrashUploadBNetModWithArchive.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			CrashUploadBNetModWithArchivePatch::CrashUploadBNetModWithArchivePatch() : 
				Module(GlobalEnginePtr)
			{}

			bool CrashUploadBNetModWithArchivePatch::HasOption() const
			{
				return false;
			}

			bool CrashUploadBNetModWithArchivePatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* CrashUploadBNetModWithArchivePatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* CrashUploadBNetModWithArchivePatch::GetName() const
			{
				return "Crash Upload BNet Mod With Archive";
			}

			bool CrashUploadBNetModWithArchivePatch::HasDependencies() const
			{
				return false;
			}

			Array<String> CrashUploadBNetModWithArchivePatch::GetDependencies() const
			{
				return {};
			}

			bool CrashUploadBNetModWithArchivePatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashUploadBNetModWithArchivePatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix crash while trying to upload BNet mods with existing archives
					//
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(0), (uintptr_t)&IsBSAVersionCurrent);

					return true;
				}

				return false;
			}

			bool CrashUploadBNetModWithArchivePatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			bool CrashUploadBNetModWithArchivePatch::IsBSAVersionCurrent(void* File)
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