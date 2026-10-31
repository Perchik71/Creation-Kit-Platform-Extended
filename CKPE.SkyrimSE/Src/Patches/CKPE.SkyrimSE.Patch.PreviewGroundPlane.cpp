// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Utils.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.Common.Relocation.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.PreviewGroundPlane.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			namespace
			{
				constexpr std::uint32_t UI_PREVIEW_DIALOG_GROUNDPLANE_CHECKBOX = 1007;

				std::uintptr_t pointer_ShowGroundPlane = 0;
				std::uintptr_t pointer_HideGroundPlane = 0;
				std::uintptr_t pointer_GetPreviewControl = 0;
				Common::EditorUI::TDialogProc OldPreviewDlgProc = nullptr;
				Common::EditorUI::TDialogProc OldModelDataDlgProc = nullptr;
				// The CK keeps no visibility setting; new previews follow the last user toggle
				bool ShowGroundPlaneState = true;

				void* GetPreviewControl(void* Dialog, std::uint32_t ControlId) noexcept(true)
				{
					return fast_call<void*>(pointer_GetPreviewControl, Dialog, ControlId);
				}

				bool IsGroundPlaneShown(void* Preview) noexcept(true)
				{
					auto plane = *reinterpret_cast<const std::uintptr_t*>((std::uintptr_t)Preview + 0xC0);
					return plane && *reinterpret_cast<const std::uintptr_t*>(plane + 0x30);
				}

				void ShowGroundPlane(void* Preview) noexcept(true)
				{
					ShowGroundPlaneState = true;
					fast_call<void>(pointer_ShowGroundPlane, Preview);
				}

				void HideGroundPlane(void* Preview) noexcept(true)
				{
					ShowGroundPlaneState = false;
					fast_call<void>(pointer_HideGroundPlane, Preview);
				}

				void InitGroundPlane(void* Preview) noexcept(true)
				{
					if (ShowGroundPlaneState)
						fast_call<void>(pointer_ShowGroundPlane, Preview);
				}

				void SetGroundPlaneShown(void* Preview, bool Shown) noexcept(true)
				{
					if (Shown)
						ShowGroundPlane(Preview);
					else
						HideGroundPlane(Preview);
					CKPE::vtbl_call<void>(0x28, Preview, 0.0f, false);
				}

				constexpr Common::EditorUI::GroundPlane Plane{ GetPreviewControl, IsGroundPlaneShown, SetGroundPlaneShown };

				std::ptrdiff_t HKPreviewDlgProc(void* Dialog, std::uint32_t Message,
					std::uintptr_t wParam, std::ptrdiff_t lParam) noexcept(true)
				{
					return Common::EditorUI::GroundPlaneDialogProc(Dialog, Message, wParam, lParam, OldPreviewDlgProc,
						Plane, UI_PREVIEW_DIALOG_GROUNDPLANE_CHECKBOX);
				}

				std::ptrdiff_t HKModelDataDlgProc(void* Dialog, std::uint32_t Message,
					std::uintptr_t wParam, std::ptrdiff_t lParam) noexcept(true)
				{
					return Common::EditorUI::GroundPlaneDialogProc(Dialog, Message, wParam, lParam, OldModelDataDlgProc, Plane);
				}
			}

			PreviewGroundPlane::PreviewGroundPlane() : Common::Patch()
			{
				SetName("Preview Ground Plane");
			}

			bool PreviewGroundPlane::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* PreviewGroundPlane::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool PreviewGroundPlane::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> PreviewGroundPlane::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool PreviewGroundPlane::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool PreviewGroundPlane::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool PreviewGroundPlane::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				pointer_ShowGroundPlane = Relocation(ID(620770)).Address();
				pointer_HideGroundPlane = Relocation(ID(620773)).Address();
				pointer_GetPreviewControl = Relocation(ID(401812)).Address();

				const bool patched =
					Relocation(ID(273740), 0x5E5).RedirectBranch(pointer_ShowGroundPlane, &InitGroundPlane) &&
					Relocation(ID(97959), 0x7DC).RedirectBranch(pointer_ShowGroundPlane, &ShowGroundPlane) &&
					Relocation(ID(97959), 0x7EA).RedirectBranch(pointer_HideGroundPlane, &HideGroundPlane) &&
					Relocation(ID(203738), 0x98C).RedirectBranch(pointer_ShowGroundPlane, &ShowGroundPlane) &&
					Relocation(ID(203738), 0x99B).RedirectBranch(pointer_HideGroundPlane, &HideGroundPlane);
				if (!patched)
					return false;

				*(std::uintptr_t*)&OldPreviewDlgProc = Relocation(ID(97959)).WriteJump(&HKPreviewDlgProc);
				*(std::uintptr_t*)&OldModelDataDlgProc = Relocation(ID(274635)).WriteJump(&HKModelDataDlgProc);

				return true;
			}
		}
	}
}
