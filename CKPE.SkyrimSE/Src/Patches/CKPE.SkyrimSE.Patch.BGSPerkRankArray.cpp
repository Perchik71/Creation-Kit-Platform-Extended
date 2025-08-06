// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.Asserts.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.BGSPerkRankArray.h>
#include <Patches/CKPE.SkyrimSE.Patch.Console.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			typedef std::int64_t(*TBGSPerkRankArraySub1)(std::int64_t, std::int64_t);
			typedef std::int64_t(*TBGSPerkRankArraySub2)(std::uint32_t*, std::int64_t);
			typedef std::int64_t(*TBGSPerkRankArraySub3)(void*, std::uint32_t*);
			typedef std::int64_t(*TBGSPerkRankArraySub4)(std::int64_t, BGSPerkRankArray::Entry*&);

			static TBGSPerkRankArraySub1 BGSPerkRankArraySub1;
			static TBGSPerkRankArraySub2 BGSPerkRankArraySub2;
			static TBGSPerkRankArraySub3 BGSPerkRankArraySub3;
			static TBGSPerkRankArraySub4 BGSPerkRankArraySub4;

			BGSPerkRankArray::BGSPerkRankArray() : Common::Patch()
			{
				SetName("BGSPerkRankArray");
			}

			bool BGSPerkRankArray::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* BGSPerkRankArray::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool BGSPerkRankArray::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> BGSPerkRankArray::GetDependencies() const noexcept(true)
			{
				return { "Console" };
			}

			bool BGSPerkRankArray::DoQuery() const noexcept(true)
			{
				// In later versions, the patch causes crashes, despite the fact that at first glance the code has not changed
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_1_5_73;
			}

			bool BGSPerkRankArray::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix for crash on null BGSPerkRankArray form ids and perk ranks being reset to 1 on save 
				// (i.e DianaVampire2017Asherz.esp)
				//
				Detours::DetourJump(__CKPE_OFFSET(0), (std::uintptr_t)&sub1);
				Detours::DetourCall(__CKPE_OFFSET(1), (std::uintptr_t)&sub2);

				BGSPerkRankArraySub1 = (TBGSPerkRankArraySub1)__CKPE_OFFSET(2);
				BGSPerkRankArraySub2 = (TBGSPerkRankArraySub2)__CKPE_OFFSET(3);
				BGSPerkRankArraySub3 = (TBGSPerkRankArraySub3)__CKPE_OFFSET(4);
				BGSPerkRankArraySub4 = (TBGSPerkRankArraySub4)__CKPE_OFFSET(5);

				return true;
			}

			bool BGSPerkRankArray::sub1(Entry* Entry, uint32_t* FormId, std::int64_t UnknownArray) noexcept(true)
			{
				CKPE_ASSERT_MSG(Entry->Rank == 0, "NPC perk loading code is still broken somewhere, rank shouldn't be set");

				// Bugfix: Always zero the form pointer union, even if the form id was null
				*FormId = Entry->FormId;
				Entry->FormIdOrPointer = 0;

				if (*FormId && UnknownArray)
				{
					BGSPerkRankArraySub2(FormId, BGSPerkRankArraySub1(UnknownArray, 0xFFFFFFFFi64));
					Entry->FormIdOrPointer = BGSPerkRankArraySub3(nullptr, FormId);

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

			void BGSPerkRankArray::sub2(std::int64_t ArrayHandle, Entry*& Entry) noexcept(true)
			{
				//
				// This function does two things:
				// - Correct the "rank" value which otherwise defaults to 1 and zero the remaining bits
				// - Prevent insertion of the perk in the array if it's null
				//
				CKPE_ASSERT_MSG(Entry->NewRank == 1, "Unexpected default for new rank member conversion");

				Entry->NewRank = Entry->Rank;
				Entry->FormIdOrPointer &= 0x00000000FFFFFFFF;

				if (Entry->FormId != 0)
					BGSPerkRankArraySub4(ArrayHandle, Entry);
				else
					Console::LogWarning(Console::MASTERFILE, "Null perk found while loading a PerkRankArray. Entry will be discarded.");
			}
		}
	}
}