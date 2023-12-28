// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Patches/ConsolePatch.h"
#include "FaceGen.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_FaceGen_sub1 = 0;
			uintptr_t pointer_FaceGen_sub2 = 0;
			uintptr_t pointer_FaceGen_sub3 = 0;
			uintptr_t pointer_FaceGen_sub4 = 0;
			uintptr_t pointer_FaceGen_sub5 = 0;
			uintptr_t pointer_FaceGen_data = 0;

			FaceGenPatch::FaceGenPatch() : Module(GlobalEnginePtr)
			{}

			bool FaceGenPatch::HasOption() const
			{
				return false;
			}

			bool FaceGenPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FaceGenPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FaceGenPatch::GetName() const
			{
				return "FaceGen";
			}

			bool FaceGenPatch::HasDependencies() const
			{
				return true;
			}

			Array<String> FaceGenPatch::GetDependencies() const
			{
				return { "Console" };
			}

			bool FaceGenPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FaceGenPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					{
						ScopeRelocator text; // fast patches

						//
						// FaceGen
						//
						// Disable automatic FaceGen on save
						if (_READ_OPTION_BOOL("FaceGen", "bDisableAutoFaceGen", false))
						{
							lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0xC3 });
							_MESSAGE("Disabling automatic calling FaceGen");
						}

						// Don't produce DDS files
						if (_READ_OPTION_BOOL("FaceGen", "bDisableExportDDS", false))
						{
							lpRelocator->PatchNop(lpRelocationDatabaseItem->At(1), 5);
							_MESSAGE("Disabling export FaceGen .DDS files");
						}

						// Don't produce TGA files
						if (_READ_OPTION_BOOL("FaceGen", "bDisableExportTGA", false))
						{
							lpRelocator->PatchNop(lpRelocationDatabaseItem->At(2), 5);
							_MESSAGE("Disabling export FaceGen .TGA files");
						}

						// Don't produce NIF files
						if (_READ_OPTION_BOOL("FaceGen", "bDisableExportNIF", false))
						{
							lpRelocator->Patch(lpRelocationDatabaseItem->At(3), { 0xC3 });
							_MESSAGE("Disabling export FaceGen .NIF files");
						}

						// Allow variable tint mask resolution
						uint32_t tintResolution = _READ_OPTION_UINT("FaceGen", "uTintMaskResolution", 512);
						lpRelocator->Patch(lpRelocationDatabaseItem->At(4), (uint8_t*)&tintResolution, sizeof(uint32_t));
						lpRelocator->Patch(lpRelocationDatabaseItem->At(5), (uint8_t*)&tintResolution, sizeof(uint32_t));
					}

					// Prevent internal filesystem reloads when exporting FaceGen for many NPCs
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(6), (uintptr_t)&sub);
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(7), 5);

					pointer_FaceGen_data = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(8));
					pointer_FaceGen_sub1 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(9));
					pointer_FaceGen_sub2 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(10));
					pointer_FaceGen_sub3 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(11));
					pointer_FaceGen_sub4 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(12));
					pointer_FaceGen_sub5 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(13));
					
					return true;
				}

				return false;
			}

			bool FaceGenPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void FaceGenPatch::sub(__int64 a1, __int64 a2)
			{
				auto sub_1418F5210 = (bool(*)())pointer_FaceGen_sub1;
				auto sub_1418F5320 = (void(*)())pointer_FaceGen_sub2;
				auto sub_1413BAAC0 = (__int64(*)(HWND, __int64))pointer_FaceGen_sub3;
				auto sub_1418F5260 = (bool(*)(__int64, __int64))pointer_FaceGen_sub4;
				auto sub_141617680 = (void(*)(__int64))pointer_FaceGen_sub5;

				// Display confirmation message box first
				if (!sub_1418F5210())
					return;

				HWND listHandle = *(HWND*)(a1 + 16);
				int itemIndex = ListView_GetNextItem(listHandle, -1, LVNI_SELECTED);
				int itemCount = 0;

				for (bool flag = true; itemIndex >= 0 && flag; itemCount++)
				{
					flag = sub_1418F5260(a2, sub_1413BAAC0(listHandle, itemIndex));

					if (flag)
					{
						int oldIndex = itemIndex;
						itemIndex = ListView_GetNextItem(listHandle, itemIndex, LVNI_SELECTED);

						if (itemIndex == oldIndex)
							itemIndex = -1;
					}
				}

				// Reload loose file paths manually since it's patched out
				ConsolePatch::Log("Exported FaceGen for %d NPCs. Reloading loose file paths...", itemCount);
				sub_141617680(*(__int64*)pointer_FaceGen_data);

				sub_1418F5320();
			}
		}
	}
}