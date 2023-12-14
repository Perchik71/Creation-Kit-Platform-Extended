// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/SSE/TESForm.h"
#include "AlteredFormList.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			using namespace EditorAPI::SkyrimSpectialEdition;

			AlteredFormListPatch::AlteredFormListPatch() : Module(GlobalEnginePtr)
			{}

			bool AlteredFormListPatch::HasOption() const
			{
				return false;
			}

			bool AlteredFormListPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* AlteredFormListPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* AlteredFormListPatch::GetName() const
			{
				return "Altered Form List And Reference Map";
			}

			bool AlteredFormListPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool AlteredFormListPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					TESForm::OldGetFormByNumericID = (TESForm* (*)(uint32_t))(lpRelocator->Rav2Off(
						lpRelocationDatabaseItem->At(0)));
					TESForm::OldAlteredFormList_Create = (void* (*)(TESForm::Array*, uint32_t))(lpRelocator->Rav2Off(
						lpRelocationDatabaseItem->At(1)));
					TESForm::OldAlteredFormList_RemoveAllEntries = (void (*)(TESForm::Array*, bool))(lpRelocator->Rav2Off(
						lpRelocationDatabaseItem->At(2)));
					TESForm::OldAlteredFormList_Insert = (void (*)(TESForm::Array*, TESForm*&))(lpRelocator->Rav2Off(
						lpRelocationDatabaseItem->At(3)));
					TESForm::OldAlteredFormList_RemoveEntry = (void (*)(TESForm::Array*, uint32_t, uint32_t))(lpRelocator->Rav2Off(
						lpRelocationDatabaseItem->At(4)));
					TESForm::OldFormReferenceMap_RemoveEntry = (void (*)(TESForm::Array*, int))(lpRelocator->Rav2Off(
						lpRelocationDatabaseItem->At(5)));
					TESForm::OldFormReferenceMap_Find = (TESForm::Array* (*)(uint64_t Key))(lpRelocator->Rav2Off(
						lpRelocationDatabaseItem->At(6)));
					TESForm::OldFormReferenceMap_Create = (TESForm::Array* (*)(TESForm::Array*))(lpRelocator->Rav2Off(
						lpRelocationDatabaseItem->At(7)));

					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(8), (uintptr_t)&TESForm::FormReferenceMap_RemoveAllEntries);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(9), (uintptr_t)&TESForm::FormReferenceMap_FindOrCreate);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(10), (uintptr_t)&TESForm::FormReferenceMap_RemoveEntry);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(11), (uintptr_t)&TESForm::FormReferenceMap_Get);

					lpRelocator->Patch(lpRelocationDatabaseItem->At(12), { 0xCC });
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(13), (uintptr_t)&TESForm::AlteredFormList_Create);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(14), (uintptr_t)&TESForm::AlteredFormList_RemoveAllEntries);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(15), (uintptr_t)&TESForm::AlteredFormList_Insert);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(16), (uintptr_t)&TESForm::AlteredFormList_RemoveEntry);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(17), (uintptr_t)&TESForm::AlteredFormList_RemoveEntry);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(18), (uintptr_t)&TESForm::AlteredFormList_ElementExists);

					return true;
				}

				return false;
			}

			bool AlteredFormListPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}