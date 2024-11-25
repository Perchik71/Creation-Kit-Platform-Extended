// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "CrashMHDTMoreThan70.h"
#include "Editor API/SSE/TESForm.h"
#include "Editor API/SSE/TESObjectLAND.h"
#include "Editor API/SSE/TESObjectCELL.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			using namespace EditorAPI::SkyrimSpectialEdition;

			uintptr_t pointer_CrashMHDTMoreThan70Patch_sub1 = 0;
			uintptr_t pointer_CrashMHDTMoreThan70Patch_sub2 = 0;

			CrashMHDTMoreThan70Patch::CrashMHDTMoreThan70Patch() : Module(GlobalEnginePtr)
			{}

			bool CrashMHDTMoreThan70Patch::HasOption() const
			{
				return false;
			}

			bool CrashMHDTMoreThan70Patch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* CrashMHDTMoreThan70Patch::GetOptionName() const
			{
				return nullptr;
			}

			const char* CrashMHDTMoreThan70Patch::GetName() const
			{
				return "Crash MHDT More Than 70";
			}

			bool CrashMHDTMoreThan70Patch::HasDependencies() const
			{
				return false;
			}

			Array<String> CrashMHDTMoreThan70Patch::GetDependencies() const
			{
				return {};
			}

			bool CrashMHDTMoreThan70Patch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_1_6_1130;
			}

			bool CrashMHDTMoreThan70Patch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// This patch adds additional information to the log, nothing more.

					pointer_CrashMHDTMoreThan70Patch_sub1 = _RELDATA_ADDR(1);
					pointer_CrashMHDTMoreThan70Patch_sub2 = _RELDATA_ADDR(3);

					lpRelocator->DetourCall(_RELDATA_RAV(0), (uintptr_t)&sub1);
					lpRelocator->DetourCall(_RELDATA_RAV(2), (uintptr_t)&sub2);

					return true;
				}

				return false;
			}

			bool CrashMHDTMoreThan70Patch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void* CrashMHDTMoreThan70Patch::sub1(void* world, int x, int y)
			{
				auto worldForm = (TESForm*)world;
				if (!worldForm || worldForm->Type != TESForm::ftWorldSpace)
					return nullptr;

				auto cellForm = fastCall<TESObjectCELL*>(pointer_CrashMHDTMoreThan70Patch_sub1, world, x, y);
				if (cellForm)
				{
					if (cellForm->GetEditorID_NoVTable() && (cellForm->GetEditorID_NoVTable()[0] != '\0'))
						_CONSOLE("Processing of the height map of the \"%s\" (0x%08X) cell according to the coordinates (%d, %d) of the \"%s\" world",
							cellForm->GetEditorID_NoVTable(), cellForm->FormID, x, y, worldForm->FullName);
					else
						_CONSOLE("Processing of the height map of the (0x%08X) cell according to the coordinates (%d, %d) of the \"%s\" world",
							cellForm->FormID, x, y, worldForm->FullName);
				}

				return (void*)cellForm;
			}

			void CrashMHDTMoreThan70Patch::sub2(void* shape, NiAPI::NiPoint3* vec, NiAPI::NiPoint2* point, void* unk1, int unk2)
			{
				// It is necessary to get the stack of the calling function.
				auto rsp = (uintptr_t)_AddressOfReturnAddress() + 8;
				
				float coord_x = *(float*)(rsp + 0x64);
				float coord_y = *(float*)(rsp + 0x60);

				__try
				{
					fastCall<void>(pointer_CrashMHDTMoreThan70Patch_sub2, shape, vec, point, unk1, unk2);
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					_CONSOLE("ASSERTION: Fatal calculating the height for the point (%.0f, %.0f) of (%.3f, %.3f)", coord_x, coord_y, point->x, point->y);
				}
			}
		}
	}
}