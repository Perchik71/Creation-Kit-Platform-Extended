// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.Relocation.h>
#include <CKPE.Fallout4.VersionLists.h>
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
				State* State::GetSingleton() noexcept(true)
				{
					if (VersionLists::GetEditorVersion() ==
						VersionLists::EDITOR_FALLOUT_C4_1_10_162_0)
						return nullptr;

					static Common::Relocation<State*> instance{ EditorAPI::IDs::BSGraphics::State::Instance };
					return instance.Get();
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
