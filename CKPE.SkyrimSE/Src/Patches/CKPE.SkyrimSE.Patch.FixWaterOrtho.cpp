// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Asserts.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/N/NiCamera.h>
#include <EditorAPI/B/BGSRenderWindow.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixWaterOrtho.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			using TFixWaterOrthoApplyFrustum = void(EditorAPI::NiAPI::NiCamera*, const EditorAPI::NiAPI::NiFrustum*);

			static Common::Relocation<TFixWaterOrthoApplyFrustum*> FixWaterOrthoApplyFrustum;

			using TFixWaterOrthoSetViewMode = EditorAPI::BGSRenderWindowCamera*(EditorAPI::BGSRenderWindowCamera*, std::uint32_t);
			using TFixWaterOrthoResize = void(EditorAPI::BGSRenderWindow*, std::int32_t, std::int32_t);

			static Common::Relocation<TFixWaterOrthoSetViewMode*> FixWaterOrthoSetViewMode;

			static Common::Relocation<TFixWaterOrthoResize*> FixWaterOrthoResize;

			FixWaterOrtho::FixWaterOrtho() : Common::Patch()
			{
				SetName("Fix Water In Ortho mode");
			}

			bool FixWaterOrtho::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixWaterOrtho::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixWaterOrtho::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixWaterOrtho::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixWaterOrtho::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixWaterOrtho::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixWaterOrtho::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fix for water not rendering correctly while using the orthographic (top-down) camera view. 
				// SSE camera scaling changes cause weird behavior with water shaders.
				//
				Relocation(EditorAPI::IDs::BGSRenderWindowCamera::Ctor, 0x88).WriteCall(&InitOrthoFrustum);

				//
				// Leaving the orthographic view re-applies the saved perspective frustum. The cell load code derives the
				// camera's far/near ratio from the near plane of the live frustum, which in ortho mode is the one made
				// above, so the ratio is too small once the perspective frustum comes back and its near plane is pushed
				// out (far / ratio). Restore the ratio of the frustum that is being applied.
				//
				FixWaterOrthoApplyFrustum = Relocation(EditorAPI::IDs::BGSRenderWindowCamera::SetViewMode, 0xF3)
					.WriteCall(&RestorePerspective);

				//
				// Resizing the render window rebuilds the live camera frustum as a perspective one, but the camera still
				// reports the orthographic view, so the next toggle "leaves" ortho without any visible change and it takes
				// a second press to get it. Put the orthographic view back after the resize.
				//
				FixWaterOrthoResize = Relocation(EditorAPI::IDs::BGSRenderWindow::WndProc, 0x298)
					.WriteCall(&ResizeRenderWindow);

				//
				// The window title shows the view mode ("[Front view, perspective]"), the CK refreshes it when the camera
				// mode is cycled but not when the hotkey switches between the perspective and orthographic view.
				// Refresh it after both view mode switches of the hotkey handler.
				//
				FixWaterOrthoSetViewMode = Relocation(EditorAPI::IDs::BGSRenderWindow::HKFunc_ToggleOrthographicPerspective, 0x173)
					.WriteCall(&SetViewModeAndUpdateTitle);
				Relocation(EditorAPI::IDs::BGSRenderWindow::HKFunc_ToggleOrthographicPerspective, 0x239)
					.WriteCall(&SetViewModeAndUpdateTitle);

				return true;
			}

			// Replaces NiFrustum::NiFrustum(bool) for the orthographic frustum saved by the render window camera
			// (BGSRenderWindowCamera constructor, first of the two frustum initializations).
			void FixWaterOrtho::InitOrthoFrustum(EditorAPI::NiAPI::NiFrustum* a1, bool a2) noexcept(true)
			{
				*a1 = EditorAPI::NiAPI::NiFrustum{};

				a1->m_bOrtho = a2;
				// The near plane of the orthographic view
				a1->m_fNear = 1000.0f;
			}

			void FixWaterOrtho::RestorePerspective(EditorAPI::NiAPI::NiCamera* a1, const EditorAPI::NiAPI::NiFrustum* a2) noexcept(true)
			{
				// The camera limits a frustum it is given: near = max(near, far / m_fMaxFarNearRatio, m_fMinNearPlaneDist)
				if (a1 && a2 && !a2->m_bOrtho && (a2->m_fNear > 0.0f) && (a2->m_fFar > 0.0f))
				{
					auto need = a2->m_fFar / a2->m_fNear;
					if (a1->m_fMaxFarNearRatio < need)
						a1->m_fMaxFarNearRatio = need;
				}

				FixWaterOrthoApplyFrustum(a1, a2);
			}

			EditorAPI::BGSRenderWindowCamera* FixWaterOrtho::SetViewModeAndUpdateTitle(EditorAPI::BGSRenderWindowCamera* a1, std::uint32_t a2) noexcept(true)
			{
				auto result = FixWaterOrthoSetViewMode(a1, a2);

				auto window = a1 ? a1->GetRenderWindow() : nullptr;
				if (window)
					window->UpdateTitle();

				return result;
			}

			void FixWaterOrtho::ResizeRenderWindow(EditorAPI::BGSRenderWindow* a1, std::int32_t a2, std::int32_t a3) noexcept(true)
			{
				FixWaterOrthoResize(a1, a2, a3);

				auto camera = a1 ? a1->Camera : nullptr;
				auto ni_camera = camera ? camera->GetNiCamera() : nullptr;
				if (!ni_camera || (camera->ViewMode != EditorAPI::BGSRenderWindowCamera::Orthographic) ||
					ni_camera->m_kViewFrustum.m_bOrtho)
					return;

				// The live frustum is the perspective one, pretend to be in perspective and let the game enter ortho again,
				// it saves the live (new) perspective frustum and builds the orthographic one for the new window size.
				camera->SetViewModeRaw(EditorAPI::BGSRenderWindowCamera::Perspective);
				camera->SetViewMode(EditorAPI::BGSRenderWindowCamera::Orthographic);
			}
		}
	}
}