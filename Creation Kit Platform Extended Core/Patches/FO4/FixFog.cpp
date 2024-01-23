// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixFog.h"
#include "Patches/Windows/FO4/MainWindowF4.h"
#include "Editor API/EditorUI.h"
#include "Editor API/FO4/TESF4.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			struct SkyPropertyTag
			{
				char pad00[0x18];
				EditorAPI::Fallout4::TES::SkyM* lpSky;
			};

			bool* globalFogEnabled = nullptr;
			bool* globalSkyEnabled = nullptr;
			static decltype(&FixFogPatch::sub) function_FixFogPatch_sub = nullptr;

			FixFogPatch::FixFogPatch() : Module(GlobalEnginePtr)
			{}

			bool FixFogPatch::HasOption() const
			{
				return false;
			}

			bool FixFogPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixFogPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixFogPatch::GetName() const
			{
				return "Fix Fog";
			}

			bool FixFogPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixFogPatch::GetDependencies() const
			{
				return {};
			}

			bool FixFogPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixFogPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					globalFogEnabled = (bool*)lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0));

					// Refresh state
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(1), (uintptr_t)&sub);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(2), (uintptr_t)&sub);

					{
						ScopeRelocator text;

						// Remove useless
						lpRelocator->PatchNop(lpRelocationDatabaseItem->At(3), 6);
						lpRelocator->PatchNop(lpRelocationDatabaseItem->At(4), 6);

						// No sky
						lpRelocator->Patch(lpRelocationDatabaseItem->At(5), 
							{ 0x45, 0x31, 0xC9, 0x90, 0x90, 0x90, 0x90, 0x90 });
						lpRelocator->Patch(lpRelocationDatabaseItem->At(6),
							{ 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
					}

					//
					// When the sky is turned on, there is an unpleasant effect in the form of a pink render window. 
					// This is fixed when you turn on the sky again, so turn it off by default.
					// Of course, the bug appeared due to the removal of some code, 
					// but this is how the fog was separated from the sky.
					//
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(6), (uintptr_t)&InitSkyDisabled);
					globalSkyEnabled = (bool*)lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(7));

					return true;
				}

				return false;
			}

			bool FixFogPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			uint32_t FixFogPatch::InitSkyDisabled()
			{
				*globalSkyEnabled = false;

				// Return 0 as unspecified and unchecked toolbutton
				return 0;
			}

			void FixFogPatch::sub()
			{
				EditorAPI::Fallout4::TESUnknown* Inst = EditorAPI::Fallout4::TESUnknown::Instance;

				if (!Inst && Inst->Main->IsEmpty())
					return;

				if (*globalFogEnabled)
					Inst->Main->Sky->Enable();
				else
					Inst->Main->Sky->Disable();
			}

			bool FixFogPatch::IsFogEnabled()
			{
				return *globalFogEnabled;
			}

			void FixFogPatch::ToggleFog()
			{
				EditorAPI::Fallout4::TESUnknown* Inst = EditorAPI::Fallout4::TESUnknown::Instance;
				if (!Inst && Inst->Main->IsEmpty())
					return;

				auto SkyProperty = Inst->Main->Sky;
				if (!SkyProperty)
					return;

				*globalFogEnabled = !(*globalFogEnabled);
				CheckMenuItem(GlobalMainWindowPtr->MainMenu.Handle,
					EditorAPI::EditorUI::UI_EDITOR_TOGGLEFOG,
					*globalFogEnabled ? MF_CHECKED : MF_UNCHECKED);

				sub();
			}
		}
	}
}