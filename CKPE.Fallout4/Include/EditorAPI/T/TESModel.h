// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BaseFormComponent.h>
#include <EditorAPI/B/BSFixedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiAVObject;
			}

			namespace BSResource
			{
				class ID;	// BSResource::ID, array element of textures/materials (12 bytes)
			}


			namespace Forms
			{
				class BGSModelMaterialSwap;
			}


			// 1.11.246.0: size 0x38, vtable RVA 0x30222A0, 27 virtual functions
			class TESModel :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~TESModel() = default;

				virtual const char* GetModel() const noexcept(true) { return nullptr; } // 21 - RVA 0x35B020
				virtual void SetModelFromDialogBox(HWND hDialogBox, std::uint32_t nCtrlID) noexcept(true) { return; } // 22 - RVA 0x8EAAB0
				virtual void SetModel(const char* lpModel) noexcept(true) { return; } // 23 - RVA 0x8E93E0
				virtual Forms::BGSModelMaterialSwap* GetAsModelMaterialSwap() noexcept(true) { return nullptr; } // 24 - RVA 0x8B33A0
				virtual bool UpdateResourceDependencies(NiAPI::NiAVObject* Object3D, Forms::TESForm* Owner) noexcept(true) { return false; } // 25
				// Likely a model lifecycle hook; all inspected bodies are no-ops.
				virtual void sub_0D0() { return; } // 26 - RVA 0x35B030

			public:
				BSFixedString modelName; // 08
				BSResource::ID* textures; // 10
				BSResource::ID* materials; // 18
				std::uint32_t* addons; // 20
				std::int8_t numTextures; // 28
				std::int8_t numTexturesSRGB; // 29
				std::int8_t numAddons; // 2A
				std::int8_t numMaterials; // 2B
				std::int8_t flags; // 2C
				std::uint8_t pad2D[0x3]; // 2D
				std::uint32_t modelNameControlID; // 30 - CK only, dialog control ID (IDC_MODEL_NAME, 2421 on construction)
				std::uint32_t modelDataControlID; // 34 - CK only, dialog control ID (IDC_MODEL_DATA, 2420 on construction)
			};
			static_assert(sizeof(TESModel) == 0x38);
		}
	}
}
