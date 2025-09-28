// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/NiAPI/NiFlags.h>
#include <EditorAPI/NiAPI/NiTypes.h>
#include <EditorAPI/BSFixedString.h>
#include <EditorAPI/Forms/TESForm.h>
#include <EditorAPI/Forms/TESFullName.h>
#include <EditorAPI/Forms/TESCondition.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// It has been announced that the RAW color is no longer supported

				// size = 0x68
				// func = 124
				class BGSColorForm : public TESForm, public TESFullName
				{
				public:
					static constexpr std::uint32_t FLAG_PLAYABLE		= (std::uint32_t)1 << 0;
					static constexpr std::uint32_t FLAG_REMAPCOLOR		= (std::uint32_t)1 << 1;
					static constexpr std::uint32_t FLAG_EXTENDED_LUT	= (std::uint32_t)1 << 2;
				public:
					constexpr static std::uint8_t TYPE_ID = ftColorForm;

					virtual ~BGSColorForm() = default;

					inline bool HasPlayable() const noexcept(true) { return _Flags.Has(FLAG_PLAYABLE); }
					inline void SetPlayable(bool v) noexcept(true)
					{
						if (HasPlayable() == v)
							return;

						if (!v)
							_Flags.Set(FLAG_PLAYABLE);
						else
							_Flags.Unset(FLAG_PLAYABLE);
					}
					inline bool HasRemappingIndex() const noexcept(true) { return _Flags.Has(FLAG_REMAPCOLOR); }
					inline void SetRemappingIndex(bool v) noexcept(true)
					{
						if (HasRemappingIndex() == v)
							return;

						if (!v)
							_Flags.Set(FLAG_REMAPCOLOR);
						else
							_Flags.Unset(FLAG_REMAPCOLOR);
					}
					inline bool HasExtendedLUT() const noexcept(true) { return _Flags.Has(FLAG_EXTENDED_LUT); }
					inline void SetExtendedLUT(bool v) noexcept(true)
					{
						if (HasExtendedLUT() == v)
							return;

						if (!v)
							_Flags.Set(FLAG_EXTENDED_LUT);
						else
							_Flags.Unset(FLAG_EXTENDED_LUT);
					}
					inline float GetRemappingIndex() const noexcept(true) { return _RemappingIndex; }
					inline void SetRemappingIndex(float v) noexcept(true)
					{
						SetRemappingIndex(true);
						_RemappingIndex = v;
					}
					inline NiAPI::NiRGBA GetRawColor() const noexcept(true) { return _Raw; }
					inline void SetRawColor(const NiAPI::NiRGBA& Color) noexcept(true)
					{
						SetRemappingIndex(false);
						_RemappingIndex = 0.0f;
						_Raw = Color;
					}
				public:
					CKPE_READ_PROPERTY(GetName) const char* Name;
					CKPE_PROPERTY(GetRemappingIndex, SetRemappingIndex) float RemappingIndex;
					CKPE_PROPERTY(GetRawColor, SetRawColor) NiAPI::NiRGBA RawColor;
				private:
					union
					{
						float _RemappingIndex;
						NiAPI::NiRGBA _Raw;
					};
					char pad44[0x4];
					TESCondition _Conditions;
					char pad50[0x10];
					NiAPI::NiTFlags<std::uint32_t, BGSColorForm> _Flags;
				};
				static_assert(sizeof(BGSColorForm) == 0x68);
			}
		}
	}
}