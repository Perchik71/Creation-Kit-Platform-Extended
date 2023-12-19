// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "CrashUsingMore16NPCForFaceGen.h"
#include "..\ConsolePatch.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_CrashUsingMore16NPCForFaceGen_sub = 0;

			CrashUsingMore16NPCForFaceGenPatch::CrashUsingMore16NPCForFaceGenPatch() : Module(GlobalEnginePtr)
			{}

			bool CrashUsingMore16NPCForFaceGenPatch::HasOption() const
			{
				return false;
			}

			bool CrashUsingMore16NPCForFaceGenPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* CrashUsingMore16NPCForFaceGenPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* CrashUsingMore16NPCForFaceGenPatch::GetName() const
			{
				return "Crash Using More 16 NPC For FaceGen";
			}

			bool CrashUsingMore16NPCForFaceGenPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashUsingMore16NPCForFaceGenPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix crash when using more than 16 NPC face tint masks during FaceGen
					//
					pointer_CrashUsingMore16NPCForFaceGen_sub = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0));

					auto addr = lpRelocationDatabaseItem->At(1);
					lpRelocator->Patch(addr, { 0x48, 0x8B, 0x4C, 0x24, 0x68, 0xE8, 0xCB, 0xFF,
						0xFF, 0xFF, 0xE9, 0x7D, 0x01, 0x00, 0x00 });
					lpRelocator->DetourCall((uintptr_t)addr + 5, (uintptr_t)&sub);

					return true;
				}

				return false;
			}

			bool CrashUsingMore16NPCForFaceGenPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void CrashUsingMore16NPCForFaceGenPatch::sub(__int64 Texture)
			{
				const char* texName = ((const char* (__fastcall*)(__int64))pointer_CrashUsingMore16NPCForFaceGen_sub)(*(__int64*)Texture);

				ConsolePatch::LogWarning(23, "Exceeded limit of 16 tint masks. Skipping texture: %s", texName);
			}
		}
	}
}