// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESLeveledList.h>
#include <EditorAPI/B/BGSModelMaterialSwap.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x108, vtable RVA 0x308D820, 154 virtual functions
				class TESLevCharacter :
					public TESBoundAnimObject,    // 000
					public TESLeveledList,        // 090
					public BGSModelMaterialSwap   // 0B8
				{
				public:
					virtual ~TESLevCharacter() = default;

					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 408 */ void UnClone3D(void* pRequester) noexcept(true) override;
					/* 480 */ void Clone3D(TESObjectREFR* pRequester, NiAPI::NiPointer<NiAPI::NiAVObject>* pObj3D) noexcept(true) override;
					/* 4A0 */ void CollectVoiceTypes(BSSimpleList<BGSVoiceType*>* VoiceTypes) const noexcept(true) override;
					
					constexpr static std::uint8_t TYPE_ID = ftLevelCharacter;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESLevCharacter[0];
				};
				static_assert(sizeof(TESLevCharacter) == 0x108);
			}
		}
	}
}
