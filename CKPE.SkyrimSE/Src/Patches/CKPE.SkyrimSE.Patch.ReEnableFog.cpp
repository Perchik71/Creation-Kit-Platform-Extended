// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/NiAPI/NiMemoryManager.h>
#include <Patches/CKPE.SkyrimSE.Patch.ReEnableFog.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			uintptr_t pointer_ReEnableFog_sub1 = 0;
			uintptr_t pointer_ReEnableFog_data = 0;
			uintptr_t pointer_ReEnableFog_sub2 = 0;
			uintptr_t pointer_ReEnableFog_sub3 = 0;
			uintptr_t pointer_ReEnableFog_sub4 = 0;

			ReEnableFog::ReEnableFog() : Common::Patch()
			{
				SetName("Re-enable fog rendering");
			}

			bool ReEnableFog::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* ReEnableFog::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool ReEnableFog::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> ReEnableFog::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool ReEnableFog::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool ReEnableFog::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool ReEnableFog::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Re-enable fog rendering in the Render Window by forcing post-process effects (SAO/SAOComposite/SAOFog)
				//
				auto target1 = ID(532405);			
				pointer_ReEnableFog_sub1 = Relocation(target1, 0x1562).WriteCall(&sub);
				Relocation(target1, Offset{ 0x180F, 0x180F, 0x1842 }).WriteCall(&sub);
				Relocation(target1, Offset{ 0x1A94, 0x1A94, 0x1AC7 }).WriteCall(&sub);
				Relocation(target1, Offset{ 0x1C9D, 0x1CB2, 0x1CE5 }).WriteCall(&sub);
				pointer_ReEnableFog_data = Relocation(ID(149232), 0x8).Address();
				pointer_ReEnableFog_sub2 = ID(608732).Address();
				pointer_ReEnableFog_sub3 = ID(608733).Address();
				pointer_ReEnableFog_sub4 = ID{ 771949, 995239 }.Address();

				auto target2 = ID{ 771949, 995239 };
				Relocation(target2, Offset{ 0xFF, 0x11D }).WriteFill(NOP, 4);		// Pointer always null
				Relocation(target2, Offset{ 0x153, 0x171 }).WriteFill(NOP, 0x63);	// Pointer always null
				Relocation(target2, Offset{ 0x1FE, 0x216 }).WriteFill(NOP, 5);		// Pointer always null (second parameter)
				Relocation(target2, Offset{ 0x3C0, 0x3D6 }).WriteFill(NOP, 5);		// Pointer always null (second parameter)

				if (VersionLists::GetEditorVersion() >= VersionLists::EDITOR_SKYRIM_SE_1_6_1130)
				{
					Relocation(target2, 0x3DB).WriteFill(NOP, 0x5B);	// Assert always triggers and multiple null pointers in call
					Relocation(target2, 0x440).WriteFill(NOP, 0x213);	// Remove most of the useless stuff in the function
				}
				else
				{
					Relocation(target2, 0x3C5).WriteFill(NOP, 0x65);	// Assert always triggers and multiple null pointers in call
					Relocation(target2, 0x434).WriteFill(NOP, 0x222);	// Remove most of the useless stuff in the function
				}

				Relocation(ID(661367)).Write(RET);		// Pointer always null (BSGraphics::State::UpdateTemporalData)
				Relocation(ID(661365)).Write(RET);		// Pointer always null (BSGraphics::State::UpdateTemporalData)

				// Force DEPTH_STENCIL_POST_ZPREPASS_COPY RT to be copied every frame
				Relocation(ID(729089), Offset { 0x4E5, 0x4C5 }).WriteFill(NOP, 2);

				return true;
			}

			void ReEnableFog::sub(std::int64_t a1, std::int64_t a2, std::int64_t a3) noexcept(true)
			{
				// Draw objects in the render window normally
				((void(__fastcall*)(std::int64_t, std::int64_t, std::int64_t))pointer_ReEnableFog_sub1)(a1, a2, a3);

				// Then do post-process SAO (Fog) ("Draw WorldRoot")
				auto& byte_144F05728 = *(bool*)pointer_ReEnableFog_data;
				auto& qword_145A11B28 = *(std::uintptr_t*)pointer_ReEnableFog_sub2;
				auto& qword_145A11B38 = *(std::uintptr_t*)pointer_ReEnableFog_sub3;

				if (byte_144F05728)
				{
					if (!qword_145A11B28)
						// Fake BSFadeNode
						qword_145A11B28 = (std::uintptr_t)EditorAPI::NiAPI::NiMemoryManager::Alloc(nullptr, 4096, 8);

					if (!qword_145A11B38)
						// Fake SceneGraph
						qword_145A11B38 = (std::uintptr_t)EditorAPI::NiAPI::NiMemoryManager::Alloc(nullptr, 4096, 8);

					((void(__fastcall*)())pointer_ReEnableFog_sub4)();
				}
			}

			bool ReEnableFog::IsFogEnabled()
			{
				return *reinterpret_cast<bool*>(pointer_ReEnableFog_data);
			}
		}
	}
}