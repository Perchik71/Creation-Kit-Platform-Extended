// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Utils.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/Forms/TESForm.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixFormIDsInLoadFile.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			constexpr static std::uint32_t MASK_HIGH_INDEX = 0xFF000000;
			constexpr static std::uint32_t MASK_FORM_INDEX = 0x00FFFFFF;
			constexpr static std::uint32_t MAXF_LOOP_COUNT = 0x60;

			std::uintptr_t pointer_FixFormIDsInLoadFile_sub1 = 0;
			std::uintptr_t pointer_FixFormIDsInLoadFile_sub2 = 0;

			template<std::uint32_t MainOff, std::uint32_t StoreOff, std::uint32_t LimitMaster = MAXF_LOOP_COUNT>
			void FindLocFormIDs(std::uintptr_t Stack)
			{
				auto LocForm = *(EditorAPI::Forms::TESForm**)(Stack + MainOff);
				auto FormIDs = (std::uint32_t*)(Stack + StoreOff);

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
					auto LocForm = fast_call<EditorAPI::Forms::TESForm*>(pointer_FixFormIDsInLoadFile_sub1, *FormIDs);
					if (LocForm)
					{
						// Need a location form, if this is not the case, then a mistake
						Ret = LocForm->GetFormType() == EditorAPI::Forms::TESForm::ftLocation;
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

			FixFormIDsInLoadFile::FixFormIDsInLoadFile() : Common::Patch()
			{
				SetName("Fix FormIDs In Load File");
			}

			bool FixFormIDsInLoadFile::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixFormIDsInLoadFile::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixFormIDsInLoadFile::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> FixFormIDsInLoadFile::GetDependencies() const noexcept(true)
			{
				return { "Console" };
			}

			bool FixFormIDsInLoadFile::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixFormIDsInLoadFile::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixFormIDsInLoadFile::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				// Numerous changes in the load. Changing the order of indexes, fixing bugs, where forms not found.

				auto Rva = Relocation(ID(554411), Offset{ 0x28F5, 0x2946 }).Address();
				auto Rva2 = Relocation(ID(235267), 0x39E).Address();
				auto Rva3 = Relocation(ID(236153), 0x673).Address();
				
				{
					// Cutting a lot is faster this way
					auto stext = Application::GetSingleton()->GetSegment(Segment::text);
					ScopeSafeWrite text(stext.GetAddress(), stext.GetSize());

					// get stack for sub2 function
					text.Write(Rva, { 0x48, 0x8D, 0x0C, 0x24, 0x90, 0x90, 0x90, 0x90 });

					// 0000000141C370FE | 48:8D5424 30 | lea rdx,qword ptr ss:[rsp+30] |
					text.Write(Rva2, { 0x48, 0x8D, 0x54, 0x24, 0x30 });
					text.WriteNop(Rva2 + 0x12, 0xD);

					// get stack for sub4 function
					text.Write(Rva3, { 0x48, 0x8D, 0x0C, 0x24, 0x90 });
				}

				if (VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_1_5_73)
					Detours::DetourCall(Rva + 8, (std::uintptr_t)&FindLocFormIDs<0x688, 0xE8>);
				else
					Detours::DetourCall(Rva + 8, (std::uintptr_t)&FindLocFormIDs<0x698, 0xF0>);

				Detours::DetourCall(Rva2 + 0xD, (std::uintptr_t)&sub2);
				Detours::DetourCall(Rva3 + 5, (std::uintptr_t)&FindLocFormIDs<0x150, 0x54>);

				pointer_FixFormIDsInLoadFile_sub1 = ID(76804).Address();
				pointer_FixFormIDsInLoadFile_sub2 = ID(33992).Address();

				// Let's edit the function so that it finds the form correctly
				Relocation(ID(235267), 0x4CF).WriteCall(&sub);
				
				return true;
			}

			bool FixFormIDsInLoadFile::sub(void* World, std::uint32_t* ParentFormIDs, std::uint32_t* FormIDs) noexcept(true)
			{
				if (!World || !FormIDs || !ParentFormIDs)
					return false;

				// Sometimes the index is 0 for some reason
				if (!(*FormIDs) && !(*ParentFormIDs))
					return true;

				auto w = (EditorAPI::Forms::TESForm*)World;

				// I assume that the location has an high-index of the world itself.
				auto HighIndex = w->GetFormID() & MASK_HIGH_INDEX;

				if (*FormIDs > 0)
					*FormIDs = HighIndex | (*FormIDs & MASK_FORM_INDEX);

				if (*ParentFormIDs > 0)
					*ParentFormIDs = HighIndex | (*ParentFormIDs & MASK_FORM_INDEX);

				return fast_call<bool>(pointer_FixFormIDsInLoadFile_sub2, World, ParentFormIDs, FormIDs);
			}

			std::uint32_t FixFormIDsInLoadFile::sub2(void* World, std::uint32_t* FormIDs) noexcept(true)
			{
				if (!World || !FormIDs || !(*FormIDs))
					return 0;

				auto w = (EditorAPI::Forms::TESForm*)World;

				// I assume that the location has an high-index of the world itself.
				auto HighIndex = w->GetFormID() & MASK_HIGH_INDEX;
				*FormIDs = HighIndex | (*FormIDs & MASK_FORM_INDEX);
				std::uint32_t HighIndexLoop = 0;
				bool HasFind = false;

			Loop:
				auto LocForm = fast_call<EditorAPI::Forms::TESForm*>(pointer_FixFormIDsInLoadFile_sub1, *FormIDs);
				if (LocForm)
				{
					// Need a location form, if this is not the case, then a mistake
					auto HasFind = LocForm->GetFormType() == EditorAPI::Forms::TESForm::ftLocation;
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