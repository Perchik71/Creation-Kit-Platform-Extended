// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSSoundDescriptorForm;

				// 1.11.240.0: size 0xA8, vtable RVA 0x307CB28, 154 virtual functions
				class TESSound :
					public TESBoundAnimObject   // 000
				{
				public:
					virtual ~TESSound() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2A8 */ [[nodiscard]] bool CanCheckIn(const CheckInFormList* Forms) const noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 408 */ void UnClone3D(void* pRequester) noexcept(true) override;
					/* 480 */ void Clone3D(TESObjectREFR* pRequester, NiAPI::NiPointer<NiAPI::NiAVObject>* pObj3D) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftSoundMarker;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESSound[0];
				public:
					BGSSoundDescriptorForm* descriptor; // 90
					std::uint64_t unk98; // 98
					std::uint8_t unkA0; // A0
					std::uint8_t unkA1[0x7]; // A1
				};
				static_assert(sizeof(TESSound) == 0xA8);
			}
		}
	}
}
