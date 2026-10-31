// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/F/FormInterfaces.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x60, vtable RVA 0x2FAC100, 124 virtual functions
				class BGSObjectVisibilityManager :
					public TESForm,                                           // 000
					public BSTSingletonImplicit<BGSObjectVisibilityManager>   // 028
				{
				public:
					virtual ~BGSObjectVisibilityManager() = default;

					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftObjectVisibilityManager;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSObjectVisibilityManager[0];
				public:
					std::uint8_t unk28[0xC]; // 28
					std::uint8_t unk34[0x8]; // 34
					std::uint32_t unk3C; // 3C
					std::uint64_t unk40; // 40
					std::uint32_t unk48; // 48
					std::uint8_t unk4C[0xC]; // 4C
					std::uint64_t unk58; // 58
				};
				static_assert(sizeof(BGSObjectVisibilityManager) == 0x60);
			}
		}
	}
}
