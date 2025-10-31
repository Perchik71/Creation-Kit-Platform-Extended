// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "../BGSLocalizedString.h"
#include "TESForm.h"
#include <EditorAPI/NiAPI/NiTypes.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
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
				private:
					BGSLocalizedString _DisplayName;	// 08
					NiAPI::NiRGB _Color;				// 10
					BSFixedString _Description;			// 18
					std::uint8_t _Type;
					char _unk[15];
				public:
					virtual ~BGS_KEYWORD_DATA() = default;

					[[nodiscard]] inline const char* GetDisplayName() const noexcept(true) { return _DisplayName.c_str(); }
					[[nodiscard]] inline NiAPI::NiRGB GetColor() const noexcept(true) { return _Color; }
					[[nodiscard]] inline const char* GetDescription() const noexcept(true) { return _Description.c_str(); }
					[[nodiscard]] inline std::uint8_t GetType() const noexcept(true) { return _Type; }
				};
				static_assert(sizeof(BGS_KEYWORD_DATA) == 0x38);

				class BGSKeyword : public TESForm, public BGS_KEYWORD_DATA
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftKeyword;

					virtual ~BGSKeyword() = default;
				};
				static_assert(sizeof(BGSKeyword) == 0x60);
			}
		}
	}
}
