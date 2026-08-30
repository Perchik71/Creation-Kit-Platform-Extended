// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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

			bool MemoryLeakInPreviewWindow::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool MemoryLeakInPreviewWindow::DoQuery() const noexcept(true)
			{
				// In 1.6.1130 class changed and functions too
				// Bug exists.
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool MemoryLeakInPreviewWindow::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				//
				// Fix for memory leak when opening many preview windows or resizing them. 
				// D3D11 render targets are recreated each time, but the old ones were never released.
				//

				using namespace Common;

				EditorAPI::pointer_Renderer = ID(549085).Address();
				EditorAPI::pointer_D3D11Device = ID(121032).Address();

				if (VersionLists::GetEditorVersion() < VersionLists::EDITOR_SKYRIM_SE_1_6_1130)
				{
					Relocation(ID(85000)).WriteJump(&EditorAPI::BSGraphicsRenderTargetManager_CK::CreateRenderTarget);
					Relocation(ID(556748)).WriteJump(&EditorAPI::BSGraphicsRenderTargetManager_CK::CreateDepthStencil);
					Relocation(ID(556750)).WriteJump(&EditorAPI::BSGraphicsRenderTargetManager_CK::CreateCubemapRenderTarget);
				}
				else
				{
					Relocation(ID(85000)).WriteJump(&EditorAPI::BSGraphicsRenderTargetManager_CK1130::CreateRenderTarget);
					Relocation(ID(556748)).WriteJump(&EditorAPI::BSGraphicsRenderTargetManager_CK1130::CreateDepthStencil);
					Relocation(ID(556750)).WriteJump(&EditorAPI::BSGraphicsRenderTargetManager_CK1130::CreateCubemapRenderTarget);
				}

				return true;
			}
		}
	}
}