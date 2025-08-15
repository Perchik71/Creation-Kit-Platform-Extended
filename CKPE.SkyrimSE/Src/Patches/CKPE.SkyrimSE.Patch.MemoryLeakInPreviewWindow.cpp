// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.Asserts.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/BSGraphicsRenderTargetManager.h>
#include <Patches/CKPE.SkyrimSE.Patch.MemoryLeakInPreviewWindow.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			MemoryLeakInPreviewWindow::MemoryLeakInPreviewWindow() : Common::Patch()
			{
				SetName("Memory Leak In Peview Window");
			}

			bool MemoryLeakInPreviewWindow::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* MemoryLeakInPreviewWindow::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool MemoryLeakInPreviewWindow::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> MemoryLeakInPreviewWindow::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool MemoryLeakInPreviewWindow::DoQuery() const noexcept(true)
			{
				// In 1.6.1130 class changed and functions too
				// Bug exists.
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool MemoryLeakInPreviewWindow::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				//
				// Fix for memory leak when opening many preview windows or resizing them. 
				// D3D11 render targets are recreated each time, but the old ones were never released.
				//

				auto verPatch = db->GetVersion();
				auto _interface = CKPE::Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				if (verPatch == 1)
				{
					EditorAPI::pointer_Renderer = __CKPE_OFFSET(0);
					EditorAPI::pointer_D3D11Device = __CKPE_OFFSET(1);

					Detours::DetourClassJump(__CKPE_OFFSET(2),
						&EditorAPI::BSGraphicsRenderTargetManager_CK::CreateRenderTarget);
					Detours::DetourClassJump(__CKPE_OFFSET(3),
						&EditorAPI::BSGraphicsRenderTargetManager_CK::CreateDepthStencil);
					Detours::DetourClassJump(__CKPE_OFFSET(4),
						&EditorAPI::BSGraphicsRenderTargetManager_CK::CreateCubemapRenderTarget);

					return true;
				}
				else if (verPatch == 2)
				{
					EditorAPI::pointer_Renderer = __CKPE_OFFSET(0);
					EditorAPI::pointer_D3D11Device = __CKPE_OFFSET(1);

					Detours::DetourClassJump(__CKPE_OFFSET(2),
						&EditorAPI::BSGraphicsRenderTargetManager_CK1130::CreateRenderTarget);
					Detours::DetourClassJump(__CKPE_OFFSET(3),
						&EditorAPI::BSGraphicsRenderTargetManager_CK1130::CreateDepthStencil);
					Detours::DetourClassJump(__CKPE_OFFSET(4),
						&EditorAPI::BSGraphicsRenderTargetManager_CK1130::CreateCubemapRenderTarget);

					return true;
				}

				return false;
			}
		}
	}
}