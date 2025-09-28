// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BGSTransform.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// size = 0x10
				// func = 21
				class BGSPreviewTransform
				{
				public:
					virtual ~BGSPreviewTransform() = default;
				public:
					[[nodiscard]] inline BGSTransform* GetTransform() const noexcept(true) { return _Transform; }
				public:
					CKPE_READ_PROPERTY(GetTransform) BGSTransform* Transform;
				private:
					BGSTransform* _Transform;
				};
				static_assert(sizeof(BGSPreviewTransform) == 0x10);
			}
		}
	}
}