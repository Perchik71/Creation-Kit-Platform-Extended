// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "BGSMaterialSwap.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			// size = 0x50
			// func = 27
			class BGSModelMaterialSwap
			{
			public:
				virtual ~BGSModelMaterialSwap() = default;
			public:
				inline const char* GetModelPath() const { return _ModelPath ? _ModelPath->Get<char>(true) : nullptr; }
				inline BGSMaterialSwap* GetMaterialSwap() const { return _MaterialSwap; }
				inline float GetColorRemapIndex() const { return fColorRemapIndex; }
				inline bool HasColorRemapIndex() const
				{
					// 0x7F7FFFFF - magic hex number. It's not NaN nor infiniti, I don't understand what it is.
					return uColorRemapIndex == 0x7F7FFFFF;
				}
			public:
				READ_PROPERTY(GetModelPath) const char* ModelPath;
				READ_PROPERTY(GetMaterialSwap) BGSMaterialSwap* MaterialSwap;
				READ_PROPERTY(GetColorRemapIndex) float ColorRemapIndex;
			private:
				BSEntryString* _ModelPath;
				char pad10[0x18];
				uint32_t u32Unk28;
				char pad2C[0x14];
				BGSMaterialSwap* _MaterialSwap;
				union
				{
					float fColorRemapIndex;
					uint32_t uColorRemapIndex;
				};
				char pad4[0x4];
			};
			static_assert(sizeof(BGSModelMaterialSwap) == 0x50);
		}
	}
}
