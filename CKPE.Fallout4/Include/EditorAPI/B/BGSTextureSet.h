// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSShaderData.h>
#include <EditorAPI/B/BGSImpactData.h>
#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/B/BSTextureSet.h>
#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/T/TESTexture.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{

				// 1.11.240.0: size 0x3D8, vtable RVA 0x3064968, 154 virtual functions
				class BGSTextureSet :
					public TESBoundObject,  // 000
					public BSTextureSet     // 090
				{
				public:
					virtual ~BGSTextureSet() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 330 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 368 */ void CollectAdditionalResourceFiles(BSTArray<BSFixedString>& Files) noexcept(true) override;
					/* 400 */ void Clone3D(TESObjectREFR* pRequester, NiAPI::NiPointer<NiAPI::NiAVObject>* pObj3D, bool bBackground) noexcept(true) override;
					/* 408 */ void UnClone3D(void* pRequester) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftTextureSet;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSTextureSet[0];
				public:
					BSFixedString materialName; // A0
					TESTexture textures[8]; // A8
					DecalData* decalData; // 1E8 - 0x24 byte heap object freed by ClearData
					std::uint16_t flags; // 1F0
					std::uint8_t pad1F2[0x6]; // 1F2
					// The CK keeps the material data inline (the game's textureFileIDs array is absent); same 0x1E0 byte object as the game's.
					BSShaderData materialData; // 1F8
				};
				static_assert(sizeof(BGSTextureSet) == 0x3D8);
			}
		}
	}
}
