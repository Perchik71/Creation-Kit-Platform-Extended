// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "PreVis.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			uintptr_t pointer_PreVisPatch_sub = 0;

			PreVisPatch::PreVisPatch() : Module(GlobalEnginePtr)
			{}

			bool PreVisPatch::HasOption() const
			{
				return false;
			}

			bool PreVisPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* PreVisPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* PreVisPatch::GetName() const
			{
				return "PreVis";
			}

			bool PreVisPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> PreVisPatch::GetDependencies() const
			{
				return {};
			}

			bool PreVisPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool PreVisPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// This function returns Form, for preprocessing visibility, however, 
					// sometimes this function returns a Form that is not a Cell. This is an error, 
					// because in the body it is further revealedand coordinates in the world space are obtained.
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);
					pointer_PreVisPatch_sub = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));

					return true;
				}

				return false;
			}

			bool PreVisPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			EditorAPI::Fallout4::TESForm* PreVisPatch::sub(void* a1)
			{
				EditorAPI::Fallout4::TESForm* Ret = fastCall<EditorAPI::Fallout4::TESForm*>
					(pointer_PreVisPatch_sub, a1);

				if (Ret && (Ret->Type != EditorAPI::Fallout4::TESForm::ftCell))
					return nullptr;

				return Ret;
			}
		}
	}
}