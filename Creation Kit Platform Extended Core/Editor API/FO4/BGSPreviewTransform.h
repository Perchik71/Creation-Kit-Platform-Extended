// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once
#pragma pack(push, 1)

#include "BGSTransform.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			// size = 0x10
			// func = 21
			class BGSPreviewTransform
			{
			public:
				virtual ~BGSPreviewTransform() = default;
			public:
				inline BGSTransform* GetTransform() const { return _Transform; }
			public:
				READ_PROPERTY(GetTransform) BGSTransform* Transform;
			private:
				BGSTransform* _Transform;
			};
			static_assert(sizeof(BGSPreviewTransform) == 0x10);
		}
	}
}

#pragma pack(pop)