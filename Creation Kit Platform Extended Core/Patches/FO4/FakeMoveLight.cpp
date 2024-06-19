// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FakeMoveLight.h"
#include "Editor API/FO4/TESObjectREFR.h"
#include "Editor API/FO4/BSPointerHandleManager.h"
#include "Editor API/FO4/BGSRenderWindowReferenceEditModule.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			using namespace CreationKitPlatformExtended::EditorAPI::Fallout4;

			uintptr_t pointer_FakeMoveLightPatch_sub = 0;

			FakeMoveLightPatch::FakeMoveLightPatch() : Module(GlobalEnginePtr)
			{}

			bool FakeMoveLightPatch::HasOption() const
			{
				return false;
			}

			bool FakeMoveLightPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FakeMoveLightPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FakeMoveLightPatch::GetName() const
			{
				return "Fake move light";
			}

			bool FakeMoveLightPatch::HasDependencies() const
			{
				return true;
			}

			Array<String> FakeMoveLightPatch::GetDependencies() const
			{
				return { "Replace BSPointerHandle And Manager" };
			}

			bool FakeMoveLightPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_1_10_982_3;
			}

			bool FakeMoveLightPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// When lightbox and light nodes move together, and when lightbox is in the first queue, 
					// light gets a relative position from the origin, is it caused by the fact that coordinates are not stored 
					// for it in the cache ???
					//
					// Rewrite the function move objects
					
					if (!BSPointerHandleManagerCurrent::PointerHandleManagerCurrentId)
						lpRelocator->DetourJump(_RELDATA_RAV(0), &BGSRenderWindowReferenceEditModule::MoveSelectObjects);
					else
						lpRelocator->DetourJump(_RELDATA_RAV(0), &BGSRenderWindowReferenceEditModule::MoveSelectObjectsExtremly);

					*(uintptr_t*)&TESObjectREFR::SetPosition = _RELDATA_ADDR(1);
					*(uintptr_t*)&TESObjectREFR_Extremly::SetPosition = _RELDATA_ADDR(1);

					return true;
				}

				return false;
			}

			bool FakeMoveLightPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}