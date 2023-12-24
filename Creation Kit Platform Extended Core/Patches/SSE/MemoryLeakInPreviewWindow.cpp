// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "MemoryLeakInPreviewWindow.h"
#include "Editor API/SSE/BSGraphicsRenderTargetManager.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			extern uintptr_t pointer_Renderer;
			extern uintptr_t pointer_D3D11Device;
		}
	}

	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			MemoryLeakInPreviewWindowPatch::MemoryLeakInPreviewWindowPatch() : Module(GlobalEnginePtr)
			{}

			bool MemoryLeakInPreviewWindowPatch::HasOption() const
			{
				return false;
			}

			bool MemoryLeakInPreviewWindowPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* MemoryLeakInPreviewWindowPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* MemoryLeakInPreviewWindowPatch::GetName() const
			{
				return "Memory Leak In Peview Window";
			}

			bool MemoryLeakInPreviewWindowPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> MemoryLeakInPreviewWindowPatch::GetDependencies() const
			{
				return {};
			}

			bool MemoryLeakInPreviewWindowPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				// In 1.6.1130 class changed and functions too
				// Bug exists.
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool MemoryLeakInPreviewWindowPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				//
				// Fix for memory leak when opening many preview windows or resizing them. 
				// D3D11 render targets are recreated each time, but the old ones were never released.
				//

				if (lpRelocationDatabaseItem->Version() == 1)
				{
					EditorAPI::SkyrimSpectialEdition::pointer_Renderer = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0));
					EditorAPI::SkyrimSpectialEdition::pointer_D3D11Device = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));

					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(2),
						&EditorAPI::SkyrimSpectialEdition::BSGraphicsRenderTargetManager_CK::CreateRenderTarget);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(3),
						&EditorAPI::SkyrimSpectialEdition::BSGraphicsRenderTargetManager_CK::CreateDepthStencil);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(4),
						&EditorAPI::SkyrimSpectialEdition::BSGraphicsRenderTargetManager_CK::CreateCubemapRenderTarget);

					return true;
				}
				else if (lpRelocationDatabaseItem->Version() == 2)
				{
					EditorAPI::SkyrimSpectialEdition::pointer_Renderer = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0));
					EditorAPI::SkyrimSpectialEdition::pointer_D3D11Device = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));

					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(2),
						&EditorAPI::SkyrimSpectialEdition::BSGraphicsRenderTargetManager_CK1130::CreateRenderTarget);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(3),
						&EditorAPI::SkyrimSpectialEdition::BSGraphicsRenderTargetManager_CK1130::CreateDepthStencil);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(4),
						&EditorAPI::SkyrimSpectialEdition::BSGraphicsRenderTargetManager_CK1130::CreateCubemapRenderTarget);

					return true;
				}

				return false;
			}

			bool MemoryLeakInPreviewWindowPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}