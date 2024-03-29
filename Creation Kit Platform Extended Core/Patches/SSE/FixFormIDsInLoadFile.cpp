﻿// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixFormIDsInLoadFile.h"
#include "Editor API/SSE/TESFile.h"
#include "Patches/ConsolePatch.h"
#include "Editor API/SSE/TESDataHandler.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			using namespace EditorAPI::SkyrimSpectialEdition;

			constexpr static uint32_t MASK_HIGH_INDEX = 0xFF000000;
			constexpr static uint32_t MASK_FORM_INDEX = 0x00FFFFFF;
			constexpr static uint32_t MAXF_LOOP_COUNT = 0x60;

			uintptr_t pointer_FixFormIDsInLoadFile_sub1 = 0;
			uintptr_t pointer_FixFormIDsInLoadFile_sub2 = 0;

			template<uint32_t MainOff, uint32_t StoreOff, uint32_t LimitMaster = MAXF_LOOP_COUNT>
			void FindLocFormIDs(uintptr_t Stack)
			{
				auto LocForm = *(TESForm**)(Stack + MainOff);
				auto FormIDs = (uint32_t*)(Stack + StoreOff);

				if (*FormIDs > 0)
				{
					auto SafeFormIDs = *FormIDs;
					bool Ret = false;
					auto HighIndexLoop = 0;

					if (LocForm)
					{
						auto HighIndex = LocForm->GetFormID() & MASK_HIGH_INDEX;
						*FormIDs = HighIndex | (*FormIDs & MASK_FORM_INDEX);
					}

				Loop:
					auto LocForm = fastCall<TESForm*>(pointer_FixFormIDsInLoadFile_sub1, *FormIDs);
					if (LocForm)
					{
						// Need a location form, if this is not the case, then a mistake
						Ret = LocForm->GetFormType() == TESForm::ftLocation;
						if (Ret)
							return;
						else
							goto NextLoop;
					}
					else
					{
					NextLoop:
						// If can't find it, try to search in the loop
						if (HighIndexLoop <= LimitMaster)	// Limit master
						{
							*FormIDs = (HighIndexLoop << 24) | (*FormIDs & MASK_FORM_INDEX);
							HighIndexLoop++;

							goto Loop;
						}
					}

					if (!Ret)
						*FormIDs = SafeFormIDs;
				}
			}

			FixFormIDsInLoadFilePatch::FixFormIDsInLoadFilePatch() : Module(GlobalEnginePtr)
			{}

			bool FixFormIDsInLoadFilePatch::HasOption() const
			{
				return false;
			}

			bool FixFormIDsInLoadFilePatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixFormIDsInLoadFilePatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixFormIDsInLoadFilePatch::GetName() const
			{
				return "Fix FormIDs In Load File";
			}

			bool FixFormIDsInLoadFilePatch::HasDependencies() const
			{
				return true;
			}

			Array<String> FixFormIDsInLoadFilePatch::GetDependencies() const
			{
				return { "Console" };
			}

			bool FixFormIDsInLoadFilePatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixFormIDsInLoadFilePatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				auto verPatch = lpRelocationDatabaseItem->Version();

				if ((verPatch == 1) || (verPatch == 2))
				{
					// Numerous changes in the load. Changing the order of indexes, fixing bugs, where forms not found.

					auto Rva = lpRelocationDatabaseItem->At(0);
					auto Rva2 = lpRelocationDatabaseItem->At(6);
					auto Rva3 = lpRelocationDatabaseItem->At(7);

					{
						ScopeRelocator text;

						// get stack for sub2 function
						lpRelocator->Patch(Rva, { 0x48, 0x8D, 0x0C, 0x24, 0x90, 0x90, 0x90, 0x90 });

						// 0000000141C370FE | 48:8D5424 30 | lea rdx,qword ptr ss:[rsp+30] |
						lpRelocator->Patch(Rva2, { 0x48, 0x8D, 0x54, 0x24, 0x30 });
						lpRelocator->PatchNop((uintptr_t)Rva2 + 0x12, 0xD);

						// get stack for sub4 function
						lpRelocator->Patch(Rva3, { 0x48, 0x8D, 0x0C, 0x24, 0x90 });
					}

					if (verPatch == 2)
						lpRelocator->DetourCall((uintptr_t)Rva + 8, (uintptr_t)&FindLocFormIDs<0x688, 0xE8>);
					else
						lpRelocator->DetourCall((uintptr_t)Rva + 8, (uintptr_t)&FindLocFormIDs<0x698, 0xF0>);

					lpRelocator->DetourCall((uintptr_t)Rva2 + 0xD, (uintptr_t)&sub2);
					lpRelocator->DetourCall((uintptr_t)Rva3 + 5, (uintptr_t)&FindLocFormIDs<0x150, 0x54>);

					pointer_FixFormIDsInLoadFile_sub1 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(2));
					pointer_FixFormIDsInLoadFile_sub2 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(3));

					// Let's edit the function so that it finds the form correctly
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(4), (uintptr_t)&sub);

					return true;
				}

				return false;
			}

			bool FixFormIDsInLoadFilePatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			bool FixFormIDsInLoadFilePatch::sub(TESForm* World, uint32_t* ParentFormIDs, uint32_t* FormIDs)
			{
				if (!World || !FormIDs || !ParentFormIDs)
					return false;

				// Sometimes the index is 0 for some reason
				if (!(*FormIDs) && !(*ParentFormIDs))
					return true;

				// I assume that the location has an high-index of the world itself.
				auto HighIndex = World->GetFormID() & MASK_HIGH_INDEX;

				if (*FormIDs > 0)
					*FormIDs = HighIndex | (*FormIDs & MASK_FORM_INDEX);

				if (*ParentFormIDs > 0)
					*ParentFormIDs = HighIndex | (*ParentFormIDs & MASK_FORM_INDEX);

				return fastCall<bool>(pointer_FixFormIDsInLoadFile_sub2, World, ParentFormIDs, FormIDs);
			}

			uint32_t FixFormIDsInLoadFilePatch::sub2(TESForm* World, uint32_t* FormIDs)
			{
				if (!World || !FormIDs || !(*FormIDs))
					return 0;

				// I assume that the location has an high-index of the world itself.
				auto HighIndex = World->GetFormID() & MASK_HIGH_INDEX;
				*FormIDs = HighIndex | (*FormIDs & MASK_FORM_INDEX);
				uint32_t HighIndexLoop = 0;
				bool HasFind = false;

			Loop:
				auto LocForm = fastCall<TESForm*>(pointer_FixFormIDsInLoadFile_sub1, *FormIDs);
				if (LocForm)
				{
					// Need a location form, if this is not the case, then a mistake
					auto HasFind = LocForm->GetFormType() == TESForm::ftLocation;
					if (HasFind)
						return *FormIDs;
					else
						goto NextLoop;
				}
				else
				{
				NextLoop:
					// If can't find it, try to search in the loop
					if (HighIndexLoop <= MAXF_LOOP_COUNT)	// Limit master
					{
						*FormIDs = (HighIndexLoop << 24) | (*FormIDs & MASK_FORM_INDEX);
						HighIndexLoop++;

						goto Loop;
					}
				}

				if (!HasFind)
					*FormIDs = 0;

				return *FormIDs;
			}
		}
	}
}