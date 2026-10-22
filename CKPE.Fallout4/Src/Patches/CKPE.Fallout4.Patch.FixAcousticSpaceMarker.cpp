// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <EditorAPI/IDs.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.Relocation.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.FixAcousticSpaceMarker.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			namespace
			{
				// Render window manager (global pointer), read from the handler itself.
				std::uintptr_t* g_renderWindowManager = nullptr;
				// void SetSoundMarkerNodeCulled(RenderWindowManager* mgr, bool culled), read from the handler itself.
				void (*g_setSoundMarkerNodeCulled)(void*, bool) = nullptr;
				// The call this patch replaces (the common tail of the "show markers" branch).
				std::uintptr_t (*g_original)(std::uintptr_t, std::uintptr_t, std::uintptr_t, std::uintptr_t) = nullptr;
			}

			FixAcousticSpaceMarker::FixAcousticSpaceMarker() : Common::Patch()
			{
				SetName("Fix Acoustic Space Marker");
			}

			bool FixAcousticSpaceMarker::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixAcousticSpaceMarker::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixAcousticSpaceMarker::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixAcousticSpaceMarker::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixAcousticSpaceMarker::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixAcousticSpaceMarker::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// The render window "Markers" handler (bShowMarkers) hides the cell's "SoundMarker Node" (where the
				// acoustic space and sound markers live) with SetSoundMarkerNodeCulled(mgr, true) when markers are turned
				// off, but nothing ever calls it with false: the "show" branch only restores the light, land and
				// category nodes. After hide -> show the node stays culled until the cells are reloaded.
				// The fix calls SetSoundMarkerNodeCulled(mgr, false) at the end of the "show" branch.
				//

				// void RenderWindow_OnShowMarkersChanged(...): 1.10.162.0, 1.10.982.3 and later
				const auto handler = EditorAPI::IDs::Functions::RenderWindow_OnShowMarkersChanged;
				// the last call of the "show" branch (the redraw)
				const auto tailCall = Relocation(handler, Offset{ 0xF0, 0x11B }).Address();
				// mov rcx, [rip+disp32] (the render window manager), right before the call of SetSoundMarkerNodeCulled
				const auto managerLoad = Relocation(handler, Offset{ 0x10A, 0x130 }).Address();

				// void TESObjectCELL_SetSoundMarkerNodeCulled(RenderWindowManager* mgr, bool culled)
				g_setSoundMarkerNodeCulled = reinterpret_cast<void(*)(void*, bool)>(EditorAPI::IDs::Functions::RenderWindowManager__SetSoundMarkerNodeCulled.Address());
				g_renderWindowManager = reinterpret_cast<std::uintptr_t*>(
					managerLoad + 7 + *reinterpret_cast<std::int32_t*>(managerLoad + 3));

				g_original = reinterpret_cast<decltype(g_original)>(Detours::DetourCall(tailCall, (std::uintptr_t)&sub));
				return g_original != nullptr;
			}

			std::uintptr_t FixAcousticSpaceMarker::sub(std::uintptr_t a1, std::uintptr_t a2, std::uintptr_t a3, std::uintptr_t a4) noexcept(true)
			{
				// Undo the hide of the "SoundMarker Node" before the original redraws the view.
				auto manager = *g_renderWindowManager;
				if (manager)
					g_setSoundMarkerNodeCulled(reinterpret_cast<void*>(manager), false);

				return g_original(a1, a2, a3, a4);
			}
		}
	}
}
