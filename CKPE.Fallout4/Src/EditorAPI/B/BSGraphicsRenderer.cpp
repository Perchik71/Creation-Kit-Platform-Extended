// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <intrin.h>
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
				constexpr Common::Offset ThreadContextOffset{ 0x2FC8, 0x39A8 };

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

				void Renderer::UpdateViewport(RendererShadowState* state, std::uint32_t width,
					std::uint32_t height, bool unk) noexcept(true)
				{
					using Function = void(*)(Renderer*, RendererShadowState*, std::uint32_t,
						std::uint32_t, bool);
					static Common::Relocation<Function> function{
						EditorAPI::IDs::BSGraphics::Renderer::UpdateViewport };
					function(this, state, width, height, unk);
				}

				RendererThreadState* RendererThreadContext::GetState() const noexcept(true)
				{
					static Common::Relocation<RendererThreadState**> instance{
						EditorAPI::IDs::BSGraphics::Renderer::DefaultThreadState };
					return state ? state : *instance;
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

				RendererThreadContext* GetRendererThreadContext() noexcept(true)
				{
					static Common::Relocation<std::uint32_t*> index{
						EditorAPI::IDs::Functions::data_8017B70 };
					const auto tls = reinterpret_cast<std::uintptr_t*>(__readgsqword(0x58));
					if (!tls || !tls[*index])
						return nullptr;
					return &Common::RelocateMember<RendererThreadContext>(
						reinterpret_cast<void*>(tls[*index]), ThreadContextOffset);
				}
			}
		}
	}
}
