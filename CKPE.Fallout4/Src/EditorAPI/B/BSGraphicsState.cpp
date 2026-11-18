// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.Relocation.h>
#include <EditorAPI/B/BSGraphicsState.h>
#include <EditorAPI/IDs.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace BSGraphics
			{
				constexpr Common::Offset TAAStateOffset{ 0xA8, 0xAC };
				constexpr Common::Offset TAADisableCounterOffset{ 0xAC, 0xB0 };

				void RendererShadowState::InvalidateCachedState() noexcept(true)
				{
					using Function = void(*)(RendererShadowState*);
					static Common::Relocation<Function> function{
						EditorAPI::IDs::BSGraphics::RendererShadowState::InvalidateCachedState };
					function(this);
				}

				State* State::GetSingleton() noexcept(true)
				{
					static Common::Relocation<State*> instance{ EditorAPI::IDs::BSGraphics::State::Instance };
					return instance.Get();
				}

				std::uint32_t State::GetTAAState() const noexcept(true)
				{
					return Common::RelocateMember<std::uint32_t>(this, TAAStateOffset);
				}

				std::uint32_t State::GetTAADisableCounter() const noexcept(true)
				{
					return Common::RelocateMember<std::uint32_t>(this, TAADisableCounterOffset);
				}

				void State::UpdateTemporalData() noexcept(true)
				{
					using Function = void(*)(State*);
					static Common::Relocation<Function> function{
						EditorAPI::IDs::BSGraphics::State::UpdateTemporalData };
					function(this);
				}

				void State::SetCamera(const NiAPI::NiCamera* camera, bool useJitter,
					float minDepth, float maxDepth) noexcept(true)
				{
					using Function = void(*)(State*, const NiAPI::NiCamera*, bool,
						float, float);
					static Common::Relocation<Function> function{
						EditorAPI::IDs::BSGraphics::State::SetCamera };
					function(this, camera, useJitter, minDepth, maxDepth);
				}
			}
		}
	}
}
