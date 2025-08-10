// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/BSEffectShaderMaterial.h>
#include <EditorAPI/BSShaderProperty.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashTestRadius.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			std::uintptr_t pointer_CrashTestRadius_sub1 = 0;
			std::uintptr_t pointer_CrashTestRadius_sub2 = 0;

			CrashTestRadius::CrashTestRadius() : Common::Patch()
			{
				SetName("Crash Test Radius");
			}

			bool CrashTestRadius::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* CrashTestRadius::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool CrashTestRadius::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> CrashTestRadius::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool CrashTestRadius::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashTestRadius::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix for crash when trying to use "Test Radius" on a reference's "3D Data" dialog tab. 
				// This code wasn't correctly ported to BSGeometry from NiGeometry during the LE->SSE transition. 
				// Flags & materials need to be fixed as a result.
				//
				Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&sub);
				Detours::DetourCall(__CKPE_OFFSET(1), (std::uintptr_t)&sub);
				SafeWrite::Write(__CKPE_OFFSET(2), { 0x48, 0x8B, 0xC1, 0x90, 0x90 });
				SafeWrite::Write(__CKPE_OFFSET(3), { 0x48, 0x8B, 0xC1, 0x90, 0x90 });

				pointer_CrashTestRadius_sub1 = __CKPE_OFFSET(4);
				pointer_CrashTestRadius_sub2 = __CKPE_OFFSET(5);

				return true;
			}

			void CrashTestRadius::sub(std::int64_t a1, void* Property) noexcept(true)
			{
				if (Property)
				{
					auto p = (EditorAPI::BSShaderProperty*)Property;
					p->ulFlags |= (1ull << EditorAPI::BSShaderProperty::BSSP_FLAG_TWO_SIDED);		// Sphere is only 1 sided
					p->ulFlags &= ~(1ull << EditorAPI::BSShaderProperty::BSSP_FLAG_ZBUFFER_WRITE);	// Transparency is used

					// Fix material alpha. A copy must be made because it uses a global pointer by default.
					auto oldShaderMaterial = static_cast<EditorAPI::BSEffectShaderMaterial*>(p->pMaterial);
					auto newShaderMaterial = static_cast<EditorAPI::BSEffectShaderMaterial*>(oldShaderMaterial->CreateNew());

					newShaderMaterial->CopyMembers(oldShaderMaterial);
					newShaderMaterial->kBaseColor.a = 0.5f;

					((void(__fastcall*)(EditorAPI::BSShaderProperty*, EditorAPI::BSEffectShaderMaterial*, bool))pointer_CrashTestRadius_sub1)
						(p, newShaderMaterial, false);
				}

				((void(__fastcall*)(std::int64_t, void*))pointer_CrashTestRadius_sub2)(a1 + 0x128, Property);
			}
		}
	}
}