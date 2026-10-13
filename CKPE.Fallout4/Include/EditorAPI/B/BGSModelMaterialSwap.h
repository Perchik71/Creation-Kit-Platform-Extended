// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BGSMaterialSwap.h>
#include <EditorAPI/T/TESModel.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// size = 0x50
				// func = 27
				class BGSModelMaterialSwap :
					public TESModel   // 000
				{
				public:
					virtual ~BGSModelMaterialSwap() = default;
				public:
					[[nodiscard]] inline const char* GetModelPath() const noexcept(true) { return !modelName.empty() ? modelName.c_str() : nullptr; }
					[[nodiscard]] inline BGSMaterialSwap* GetMaterialSwap() const noexcept(true) { return _MaterialSwap; }
					[[nodiscard]] inline float GetColorRemapIndex() const noexcept(true) { return fColorRemapIndex; }
					[[nodiscard]] inline bool HasColorRemapIndex() const noexcept(true)
					{
						// 0x7F7FFFFF - magic hex number. It's not NaN nor infiniti, I don't understand what it is.
						return uColorRemapIndex == 0x7F7FFFFF;
					}
				public:
					CKPE_READ_PROPERTY(GetModelPath) const char* ModelPath;
					CKPE_READ_PROPERTY(GetMaterialSwap) BGSMaterialSwap* MaterialSwap;
					CKPE_READ_PROPERTY(GetColorRemapIndex) float ColorRemapIndex;
				private:
					// TESModel is 0x38 bytes (modelName at 0x08)
					char pad38[0x8];					// 38
					BGSMaterialSwap* _MaterialSwap;			// 0x40
					union
					{
						float fColorRemapIndex;
						std::uint32_t uColorRemapIndex;
					};
					char pad4C[0x4];					// 4C
				};
				static_assert(sizeof(BGSModelMaterialSwap) == 0x50);
			}
		}
	}
}
