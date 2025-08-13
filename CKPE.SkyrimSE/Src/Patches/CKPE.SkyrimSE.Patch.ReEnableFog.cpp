// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
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

			bool ReEnableFog::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool ReEnableFog::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto verPatch = db->GetVersion();

				if ((verPatch != 1) && (verPatch != 2))
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Re-enable fog rendering in the Render Window by forcing post-process effects (SAO/SAOComposite/SAOFog)
				//
				Detours::DetourCall(__CKPE_OFFSET(9), (std::uintptr_t)&sub);
				Detours::DetourCall(__CKPE_OFFSET(10), (std::uintptr_t)&sub);
				Detours::DetourCall(__CKPE_OFFSET(11), (std::uintptr_t)&sub);
				Detours::DetourCall(__CKPE_OFFSET(12), (std::uintptr_t)&sub);

				pointer_ReEnableFog_sub1 = __CKPE_OFFSET(13);
				pointer_ReEnableFog_data = __CKPE_OFFSET(14);
				pointer_ReEnableFog_sub2 = __CKPE_OFFSET(15);
				pointer_ReEnableFog_sub3 = __CKPE_OFFSET(16);
				pointer_ReEnableFog_sub4 = __CKPE_OFFSET(17);

				//
				// And send this code to the abyss of hell
				//
				// To speed up, a lot of patches
				auto stext = interface->GetApplication()->GetSegment(Segment::text);
				ScopeSafeWrite text(stext.GetAddress(), stext.GetSize());											

				text.WriteNop(__CKPE_OFFSET(0), 4);			// Pointer always null
				text.WriteNop(__CKPE_OFFSET(1), 0x63);		// Pointer always null
				text.WriteNop(__CKPE_OFFSET(2), 5);			// Pointer always null (second parameter)
				text.WriteNop(__CKPE_OFFSET(3), 5);			// Pointer always null (second parameter)

				if (verPatch == 2)
				{
					text.WriteNop(__CKPE_OFFSET(4), 0x5B);	// Assert always triggers and multiple null pointers in call
					text.WriteNop(__CKPE_OFFSET(5), 0x213);	// Remove most of the useless stuff in the function
				}
				else
				{
					text.WriteNop(__CKPE_OFFSET(4), 0x65);	// Assert always triggers and multiple null pointers in call
					text.WriteNop(__CKPE_OFFSET(5), 0x222);	// Remove most of the useless stuff in the function
				}

				text.Write(__CKPE_OFFSET(6), { 0xC3 });		// Pointer always null (BSGraphics::State::UpdateTemporalData)
				text.Write(__CKPE_OFFSET(7), { 0xC3 });		// Pointer always null (BSGraphics::State::UpdateTemporalData)

				text.WriteNop(__CKPE_OFFSET(8), 2);			// Force DEPTH_STENCIL_POST_ZPREPASS_COPY RT to be copied every frame

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