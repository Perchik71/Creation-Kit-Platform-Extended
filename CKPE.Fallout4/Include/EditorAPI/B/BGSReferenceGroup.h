// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/B/BSString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x58, vtable RVA 0x30A62D8, 124 virtual functions
				class BGSReferenceGroup :
					public TESForm   // 000
				{
				public:
					virtual ~BGSReferenceGroup() = default;

					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2A8 */ [[nodiscard]] bool CanCheckIn(const CheckInFormList* Forms) const noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;

					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSReferenceGroup[0];
				public:
					BSString name; // 28 - NNAM
					std::uint32_t unk38; // 38
					std::uint32_t unk3C; // 3C
					TESForm* rnamForm; // 40 - RNAM (form id until InitItem resolves it)
					TESForm* pnamForm; // 48 - PNAM (told about the reference through TESForm::AddReference, cleared by RemoveReferencesToForm)
					std::uint32_t unk50; // 50
					std::uint32_t unk54; // 54
				};
				static_assert(sizeof(BGSReferenceGroup) == 0x58);
				static_assert(offsetof(BGSReferenceGroup, rnamForm) == 0x40);
				static_assert(offsetof(BGSReferenceGroup, pnamForm) == 0x48);
			}
		}
	}
}
