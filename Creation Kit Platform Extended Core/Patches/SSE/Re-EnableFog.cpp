// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/NiMemoryManager.h"
#include "Re-EnableFog.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			using namespace EditorAPI;

			uintptr_t pointer_ReEnableFog_sub1 = 0;
			uintptr_t pointer_ReEnableFog_data = 0;
			uintptr_t pointer_ReEnableFog_sub2 = 0;
			uintptr_t pointer_ReEnableFog_sub3 = 0;
			uintptr_t pointer_ReEnableFog_sub4 = 0;

			ReEnableFogPatch::ReEnableFogPatch() : Module(GlobalEnginePtr)
			{}

			bool ReEnableFogPatch::HasOption() const
			{
				return false;
			}

			bool ReEnableFogPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* ReEnableFogPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* ReEnableFogPatch::GetName() const
			{
				return "Re-enable fog rendering";
			}

			bool ReEnableFogPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> ReEnableFogPatch::GetDependencies() const
			{
				return {};
			}

			bool ReEnableFogPatch::IsFogEnabled()
			{
				return *reinterpret_cast<bool*>(pointer_ReEnableFog_data);
			}

			bool ReEnableFogPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool ReEnableFogPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				auto verPatch = lpRelocationDatabaseItem->Version();
				if ((verPatch == 1) || (verPatch == 2))
				{
					//
					// Re-enable fog rendering in the Render Window by forcing post-process effects (SAO/SAOComposite/SAOFog)
					//
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(9),  (uintptr_t)&sub);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(10), (uintptr_t)&sub);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(11), (uintptr_t)&sub);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(12), (uintptr_t)&sub);
					
					pointer_ReEnableFog_sub1 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(13));
					pointer_ReEnableFog_data = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(14));
					pointer_ReEnableFog_sub2 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(15));
					pointer_ReEnableFog_sub3 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(16));
					pointer_ReEnableFog_sub4 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(17));

					//
					// And send this code to the abyss of hell
					//
					ScopeRelocator text;												// To speed up, a lot of patches

					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(0), 4);			// Pointer always null
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(1), 0x63);		// Pointer always null
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(2), 5);			// Pointer always null (second parameter)
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(3), 5);			// Pointer always null (second parameter)

					if (verPatch == 2)
					{
						lpRelocator->PatchNop(lpRelocationDatabaseItem->At(4), 0x5B);	// Assert always triggers and multiple null pointers in call
						lpRelocator->PatchNop(lpRelocationDatabaseItem->At(5), 0x213);	// Remove most of the useless stuff in the function
					}
					else
					{
						lpRelocator->PatchNop(lpRelocationDatabaseItem->At(4), 0x65);	// Assert always triggers and multiple null pointers in call
						lpRelocator->PatchNop(lpRelocationDatabaseItem->At(5), 0x222);	// Remove most of the useless stuff in the function
					}

					lpRelocator->Patch(lpRelocationDatabaseItem->At(6), { 0xC3 });		// Pointer always null (BSGraphics::State::UpdateTemporalData)
					lpRelocator->Patch(lpRelocationDatabaseItem->At(7), { 0xC3 });		// Pointer always null (BSGraphics::State::UpdateTemporalData)

					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(8), 2);			// Force DEPTH_STENCIL_POST_ZPREPASS_COPY RT to be copied every frame

					return true;
				}

				return false;
			}

			bool ReEnableFogPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void ReEnableFogPatch::sub(__int64 a1, __int64 a2, __int64 a3)
			{
				// Draw objects in the render window normally
				((void(__fastcall*)(__int64, __int64, __int64))pointer_ReEnableFog_sub1)(a1, a2, a3);

				// Then do post-process SAO (Fog) ("Draw WorldRoot")
				auto& byte_144F05728 = *(bool*)pointer_ReEnableFog_data;
				auto& qword_145A11B28 = *(uintptr_t*)pointer_ReEnableFog_sub2;
				auto& qword_145A11B38 = *(uintptr_t*)pointer_ReEnableFog_sub3;

				if (byte_144F05728)
				{
					if (!qword_145A11B28)
						qword_145A11B28 = (uintptr_t)NiMemoryManager::Alloc(nullptr, 4096, 8); // Fake BSFadeNode

					if (!qword_145A11B38)
						qword_145A11B38 = (uintptr_t)NiMemoryManager::Alloc(nullptr, 4096, 8); // Fake SceneGraph

					((void(__fastcall*)())pointer_ReEnableFog_sub4)();
				}
			}
		}
	}
}
