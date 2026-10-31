// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/BGSModelMaterialSwap.h>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/F/FormInterfaces.h>
#include <EditorAPI/B/BGSKeywordForm.h>
#include <EditorAPI/B/BGSNativeTerminalForm.h>
#include <EditorAPI/B/BGSLocalizedString.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/B/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSSoundDescriptorForm;

				// 1.11.240.0: size 0x1B0, vtable RVA 0x3098BB8, 154 virtual functions
				class TESObjectDOOR :
					public TESBoundAnimObject,         // 000
					public TESFullName,                // 090
					public BGSModelMaterialSwap,       // 0A8
					public BGSDestructibleObjectForm,  // 0F8
					public BGSOpenCloseForm,           // 108
					public BGSKeywordForm,             // 110
					public BGSNativeTerminalForm,      // 130
					public TESMagicCasterForm,         // 140
					public TESMagicTargetForm          // 141
				{
				public:
					virtual ~TESObjectDOOR() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 2B8 */ void GetReferencedForms(BSTBTree<TESForm*>* OutForms) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 338 */ void OnDialogFormSaved(HWND hDialogBox, TESForm* SavedForm) noexcept(true) override;
					/* 480 */ void Clone3D(TESObjectREFR* pRequester, NiAPI::NiPointer<NiAPI::NiAVObject>* pObj3D) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftDoor;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESObjectDOOR[0];
				public:
					std::uint8_t unk141[0x7]; // 141
					BGSLocalizedString altOpenText; // 148 - by position (same member order and sizes as the game after the last base)
					BGSLocalizedString altCloseText; // 158 - by position (same member order and sizes as the game after the last base)
					BGSSoundDescriptorForm* openSound; // 168 - by position (same member order and sizes as the game after the last base)
					BGSSoundDescriptorForm* closeSound; // 170 - by position (same member order and sizes as the game after the last base)
					BGSSoundDescriptorForm* loopSound; // 178 - by position (same member order and sizes as the game after the last base)
					std::int8_t flags; // 180 - by position (same member order and sizes as the game after the last base)
					std::uint8_t unk181[0x7]; // 181
					std::uint8_t unk188[0x10]; // 188
					BSTArray<void*> randomTeleports; // 198 - BSTArray<TESForm*>, by position (same member order and sizes as the game after the last base)
				};
				static_assert(sizeof(TESObjectDOOR) == 0x1B0);
			}
		}
	}
}
