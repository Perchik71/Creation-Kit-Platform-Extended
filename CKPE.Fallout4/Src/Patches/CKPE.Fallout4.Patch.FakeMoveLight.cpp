// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/BGSRenderWindow.h>
#include <Patches/CKPE.Fallout4.Patch.FakeMoveLight.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			EditorAPI::NiAPI::NiPoint3 data_FakeMoveLight_coord;

			FakeMoveLight::FakeMoveLight() : Common::Patch()
			{
				SetName("Fake move light");
			}

			bool FakeMoveLight::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FakeMoveLight::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FakeMoveLight::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> FakeMoveLight::GetDependencies() const noexcept(true)
			{
				return { "Render Window" };
			}

			bool FakeMoveLight::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() != VersionLists::EDITOR_FALLOUT_C4_1_10_943_1;
			}

			bool FakeMoveLight::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// When lightbox and light nodes move together, and when lightbox is in the first queue, 
				// light gets a relative position from the origin, is it caused by the fact that coordinates are not stored 
				// for it in the cache ???

				*(std::uintptr_t*)&EditorAPI::Forms::TESObjectREFR::SetPosition =
					Detours::DetourJump(__CKPE_OFFSET(1), (std::uintptr_t)&SetPosition);

				auto offset = __CKPE_OFFSET(0);

				if (VersionLists::GetEditorVersion() == VersionLists::EDITOR_FALLOUT_C4_1_10_162_0)
				{
					SafeWrite::WriteNop(offset + 0x37A, 0x2D);
					SafeWrite::Write(offset + 0x37A, { 0x48, 0x8D, 0x55, 0x9F, 0x48, 0x89, 0xD9, 0x4C, 0x8D, 0x46, 0x70 });
					Detours::DetourCall(offset + 0x385, (std::uintptr_t)&SetPosition);
				}
				else
				{
					SafeWrite::WriteNop(offset + 0x422, 0x2D);
					SafeWrite::Write(offset + 0x422, { 0x48, 0x8D, 0x55, 0xB7, 0x4C, 0x89, 0xE1, 0x4D, 0x8D, 0x46, 0x70 });
					Detours::DetourCall(offset + 0x42D, (std::uintptr_t)&SetPosition);
				}

				return true;
			}

			void FakeMoveLight::SetPosition(EditorAPI::Forms::TESObjectREFR* refr,
				EditorAPI::NiAPI::NiPoint3* pos, EditorAPI::NiAPI::NiPoint3* dir) noexcept(true)
			{
				if (!refr || !pos) return;

				if (dir)
				{
					if (*dir == EditorAPI::NiAPI::ZERO_P3)
						data_FakeMoveLight_coord = EditorAPI::NiAPI::ZERO_P3;

					auto pick = EditorAPI::BGSRenderWindow::Singleton->GetPickHandler();
					if (pick->Count > 1)
					{
						if (refr->GetParent()->GetFormType() == EditorAPI::Forms::TESForm::ftLight)
						{
							auto pos_refr = refr->GetPosition();

							pos_refr += *pos - data_FakeMoveLight_coord;
							data_FakeMoveLight_coord = *pos;

							EditorAPI::Forms::TESObjectREFR::SetPosition(refr, &pos_refr);

							return;
						}
					}
				}
				
				EditorAPI::Forms::TESObjectREFR::SetPosition(refr, pos);
			}
		}
	}
}