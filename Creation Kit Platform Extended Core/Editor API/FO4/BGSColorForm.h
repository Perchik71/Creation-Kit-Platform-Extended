// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "TESFormF4.h"
#include "BSEntryString.h"
#include "TESFormComponents.h"

#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			// It has been announced that the RAW color is no longer supported

			// size = 0x80
			// func = 124
			class BGSColorForm : public TESForm
			{
			public:
				static constexpr uint32_t FLAG_PLAYABLE		= 1;
				static constexpr uint32_t FLAG_REMAPCOLOR	= 2;
				static constexpr uint32_t FLAG_EXTENDED_LUT = 4;

				struct RawColorT
				{
					uint8_t Red, Green, Blue;
				};
			public:
				virtual ~BGSColorForm() = default;
			
				inline const char* GetName() const { return _Name.c_str(); }
				inline bool HasPlayable() const { return (_Flags & FLAG_PLAYABLE) == FLAG_PLAYABLE; }
				inline void SetPlayable(bool v) 
				{ 
					if (HasPlayable() == v)
						return;

					if (!v)
						_Flags |= FLAG_PLAYABLE;
					else
						_Flags &= ~FLAG_PLAYABLE;
				}
				inline bool HasRemappingIndex() const { return (_Flags & FLAG_REMAPCOLOR) == FLAG_REMAPCOLOR; }
				inline void SetRemappingIndex(bool v)
				{
					if (HasRemappingIndex() == v)
						return;

					if (!v)
						_Flags |= FLAG_REMAPCOLOR;
					else
						_Flags &= ~FLAG_REMAPCOLOR;
				}
				inline bool HasExtendedLUT() const { return (_Flags & FLAG_EXTENDED_LUT) == FLAG_EXTENDED_LUT; }
				inline void SetExtendedLUT(bool v)
				{
					if (HasExtendedLUT() == v)
						return;

					if (!v)
						_Flags |= FLAG_EXTENDED_LUT;
					else
						_Flags &= ~FLAG_EXTENDED_LUT;
				}
				inline float GetRemappingIndex() const { return _RemappingIndex; }
				inline void SetRemappingIndex(float v) 
				{
					SetRemappingIndex(true);
					_RemappingIndex = v; 
				}
				inline RawColorT GetRawColor() const { return _Raw; }
				inline void SetRawColor(const RawColorT& Color)
				{
					SetRemappingIndex(false);
					_RemappingIndex = 0.0f;
					_Raw = Color;
				}
			public:
				READ_PROPERTY(GetName) const char* Name;
				PROPERTY(GetRemappingIndex, SetRemappingIndex) float RemappingIndex;
				PROPERTY(GetRawColor, SetRawColor) RawColorT RawColor;
			private:
				char pad28[0x8];
				BGSLocalizedString<BSEntryString> _Name;
				union
				{
					float _RemappingIndex;
					RawColorT _Raw;
				};
				char pad44[0x4];
				TESCondition _Conditions;
				char pad58[0x8];
				uint32_t _Flags;
				char pad64[0x1C];
			};
			static_assert(sizeof(BGSColorForm) == 0x80);
		}
	}
}

#pragma pack(pop)