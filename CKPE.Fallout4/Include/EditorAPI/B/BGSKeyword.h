// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/N/NiTypes.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSAttractionRule;

				class BGS_KEYWORD_DATA
				{
				public:
					enum : std::uint8_t
					{
						kNone = 0,
						kComponentTechLevel,
						kAttachPoint,
						kComponentProperty,
						kInstantiationFilter,
						kModAssociation,
						kSound,
						kAnimArchetype,
						kFunctionCall,
						kRecipeFilter,
						kAttractionType,
						kDialogueSubtype,
						kQuestTarget,
						kAnimFlavor,
						kAnimGender,
						kAnimFace,
						kQuestGroup,
						kAnimInjured,
						kDispelEffect,
						kTotal
					};
				};


				// 1.11.240.0: size 0x60, vtable RVA 0x3037CF8, 124 virtual functions
				class BGSKeyword :
					public TESForm,      // 000
					public TESFullName   // 028
				{
				public:
					virtual ~BGSKeyword() = default;

					constexpr static std::uint8_t TYPE_ID = ftKeyword;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSKeyword[0];

					[[nodiscard]] inline const char* GetDisplayName() const noexcept(true) { return GetFullName(); }
					[[nodiscard]] inline NiAPI::NiRGB GetColor() const noexcept(true) { return color; }
					[[nodiscard]] inline const char* GetDescription() const noexcept(true) { return description.c_str(); }
					[[nodiscard]] inline std::uint8_t GetType() const noexcept(true) { return (std::uint8_t)type; }

				public:
					NiAPI::NiRGB color; // 40
					std::uint8_t alpha; // 43 - fourth byte of the CNAM dword (0xFF on construction)
					std::uint32_t pad44; // 44
					BSFixedString description; // 48
					std::uint32_t type; // 50 - TNAM
					std::uint32_t pad54; // 54
					BGSAttractionRule* attractionRule; // 58 - DATA, only used when type is 10 (attraction rule)
				};
				static_assert(sizeof(BGSKeyword) == 0x60);
			}
		}
	}
}
