// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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
			using TCrashTestRadius_sub1 = void(EditorAPI::BSShaderProperty*, EditorAPI::BSEffectShaderMaterial*, bool);
			using TCrashTestRadius_sub2 = void(std::int64_t, void*);

			static std::function<TCrashTestRadius_sub1> CrashTestRadius_sub1;
			static std::function<TCrashTestRadius_sub2> CrashTestRadius_sub2;

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

			bool CrashTestRadius::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool CrashTestRadius::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashTestRadius::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fix for crash when trying to use "Test Radius" on a reference's "3D Data" dialog tab. 
				// This code wasn't correctly ported to BSGeometry from NiGeometry during the LE->SSE transition. 
				// Flags & materials need to be fixed as a result.
				//
				auto target = ID(294671);
				Relocation(target, 0x251).WriteCall(&sub);
				Relocation(target, 0x2D2).WriteCall(&sub);
				Relocation(target, 0x244).Write({ 0x48, 0x8B, 0xC1, 0x90, 0x90 });
				Relocation(target, 0x2C5).Write({ 0x48, 0x8B, 0xC1, 0x90, 0x90 });
				CrashTestRadius_sub1 = Relocation<TCrashTestRadius_sub1>(ID(654399)).Get();
				CrashTestRadius_sub2 = Relocation<TCrashTestRadius_sub2>(ID(397691)).Get();

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

					CrashTestRadius_sub1(p, newShaderMaterial, false);
				}

				CrashTestRadius_sub2(a1 + 0x128, Property);
			}
		}
	}
}