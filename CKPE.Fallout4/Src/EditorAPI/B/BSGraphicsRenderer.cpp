// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.Relocation.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/B/BSGraphicsRenderer.h>
#include <EditorAPI/IDs.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace BSGraphics
			{
				Renderer* Renderer::GetSingleton() noexcept(true)
				{
					static Common::Relocation<Renderer*> instance{ EditorAPI::IDs::BSGraphics::Renderer::Instance };
					return instance.Get();
				}

				const AdapterInfo* Renderer::GetAdapterInfo() const noexcept(true)
				{
					if (VersionLists::GetEditorVersion() <
						VersionLists::EDITOR_FALLOUT_C4_1_11_137_0)
						return nullptr;

					return reinterpret_cast<const AdapterInfo*>(
						reinterpret_cast<const std::byte*>(this) + 0x25E0);
				}

				RendererData* GetRendererData() noexcept(true)
				{
					static Common::Relocation<RendererData**> instance{
						EditorAPI::IDs::BSGraphics::Renderer::DataPointer };
					return *instance;
				}

				RendererWindow* GetCurrentRendererWindow() noexcept(true)
				{
					static Common::Relocation<RendererWindow**> instance{
						EditorAPI::IDs::BSGraphics::Renderer::CurrentWindowPointer };
					return *instance;
				}
			}
		}
	}
}
