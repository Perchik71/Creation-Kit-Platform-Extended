// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "BGSPerkRankArray.h"
#include "..\ConsolePatch.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_BGSPerkRankArray_sub1 = 0;
			uintptr_t pointer_BGSPerkRankArray_sub2 = 0;
			uintptr_t pointer_BGSPerkRankArray_sub3 = 0;
			uintptr_t pointer_BGSPerkRankArray_sub4 = 0;

			BGSPerkRankArrayPatch::BGSPerkRankArrayPatch() : Module(GlobalEnginePtr)
			{}

			bool BGSPerkRankArrayPatch::HasOption() const
			{
				return false;
			}

			bool BGSPerkRankArrayPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* BGSPerkRankArrayPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* BGSPerkRankArrayPatch::GetName() const
			{
				return "BGSPerkRankArray";
			}

			bool BGSPerkRankArrayPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				// In later versions, the patch causes crashes, despite the fact that at first glance the code has not changed
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_1_5_73;
			}

			bool BGSPerkRankArrayPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for crash on null BGSPerkRankArray form ids and perk ranks being reset to 1 on save 
					// (i.e DianaVampire2017Asherz.esp)
					//
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub1);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(1), (uintptr_t)&sub2);

					pointer_BGSPerkRankArray_sub1 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(2));
					pointer_BGSPerkRankArray_sub2 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(3));
					pointer_BGSPerkRankArray_sub3 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(4));
					pointer_BGSPerkRankArray_sub4 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(5));

					return true;
				}

				return false;
			}

			bool BGSPerkRankArrayPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			bool BGSPerkRankArrayPatch::sub1(PerkRankEntry* Entry, uint32_t* FormId, __int64 UnknownArray)
			{
				auto sub_1416B8A20 = (__int64(*)(__int64, __int64))pointer_BGSPerkRankArray_sub1;
				auto sub_1416C05D0 = (__int64(*)(uint32_t*, __int64))pointer_BGSPerkRankArray_sub2;
				auto sub_14168E790 = (__int64(*)(void*, uint32_t*))pointer_BGSPerkRankArray_sub3;

				AssertMsg(Entry->Rank == 0, "NPC perk loading code is still broken somewhere, rank shouldn't be set");

				// Bugfix: Always zero the form pointer union, even if the form id was null
				*FormId = Entry->FormId;
				Entry->FormIdOrPointer = 0;

				if (*FormId && UnknownArray)
				{
					sub_1416C05D0(FormId, sub_1416B8A20(UnknownArray, 0xFFFFFFFFi64));
					Entry->FormIdOrPointer = sub_14168E790(nullptr, FormId);

					if (Entry->FormIdOrPointer)
					{
						(*(void(__fastcall**)(__int64, __int64))(*(__int64*)Entry->FormIdOrPointer + 520i64))(Entry->FormIdOrPointer, 
							UnknownArray);
						return true;
					}

					return false;
				}

				return true;
			}

			void BGSPerkRankArrayPatch::sub2(__int64 ArrayHandle, PerkRankEntry*& Entry)
			{
				//
				// This function does two things:
				// - Correct the "rank" value which otherwise defaults to 1 and zero the remaining bits
				// - Prevent insertion of the perk in the array if it's null
				//
				AssertMsg(Entry->NewRank == 1, "Unexpected default for new rank member conversion");

				Entry->NewRank = Entry->Rank;
				Entry->FormIdOrPointer &= 0x00000000FFFFFFFF;

				if (Entry->FormId != 0)
					((void(__fastcall*)(__int64, PerkRankEntry*&))pointer_BGSPerkRankArray_sub4)(ArrayHandle, Entry);
				else
					ConsolePatch::LogWarning(13, "Null perk found while loading a PerkRankArray. Entry will be discarded.");
			}
		}
	}
}