// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BSString.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// Bethesda uses SettingT. 
			// This is a template class with type binding, I will not use it, as it greatly inflates the code.
			class Setting
			{
			public:
				enum Types : std::uint8_t
				{
					stBool = 0x0,			// 'b' size 1
					stChar = 0x1,			// 'c' size 1
					stUnsignedChar = 0x2,	// 'h' size 1
					stSignedInt = 0x3,		// 'i'/'n' size 4
					stUnsignedInt = 0x4,	// 'u' size 4
					stFloat = 0x5,			// 'f' size 4
					stString = 0x6,			// 'S'/'s' size indet.
					stRGB = 0x7,			// 'r' size 4, alpha byte set to 255
					stRGBA = 0x8,			// 'a' size 4
					stMAX = 0x9,
					stError
				};
				// union for various value types
				union Value
				{
					bool			b;
					char			c;
					std::uint8_t	h;
					std::int32_t	i;
					std::uint32_t	u;
					float			f;
					char* s;
					struct Color {
						// alpha is least sig. std::uint8_t, *opposite* standard windows order
						std::uint8_t alpha;
						std::uint8_t blue;
						std::uint8_t green;
						std::uint8_t red;
					} rgba;
				};

				Setting(const char* name, const Value& value);
				Setting(const Setting& s);
				virtual ~Setting();

				Types GetValueType() const;
				inline bool CheckValidValueType(Types t) const { return GetValueType() == t; }
				bool SetBool(bool value);
				bool SetChar(char value);
				bool SetUnsignedChar(std::uint8_t value);
				bool SetSignedInt(std::int32_t value);
				bool SetUnsignedInt(std::uint32_t value);
				bool SetFloat(float value);
				bool SetString(const char* value);
				inline bool SetString(const BSString& value) { SetString(value.c_str()); }
				inline bool SetString(const std::string& value) { SetString(value.c_str()); }
				bool SetRGB(std::uint8_t r, std::uint8_t g, std::uint8_t b);
				bool SetRGBA(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);

				inline const char* GetName() const { return _Name; }
				inline bool GetBool() const { return _Value.b; }
				inline char GetChar() const { return _Value.c; }
				inline std::uint8_t GetUnsignedChar() const { return _Value.h; }
				inline std::int32_t GetSignedInt() const { return _Value.i; }
				inline std::uint32_t GetUnsignedInt() const { return _Value.u; }
				inline float GetFloat() const { return _Value.f; }
				inline const char* GetString() const { return _Value.s; }
				inline Value::Color GetRGB() const { return _Value.rgba; }
				inline Value::Color GetRGBA() const { return _Value.rgba; }

				CKPE_READ_PROPERTY(GetName) const char* Name;
				CKPE_READ_PROPERTY(GetValueType) Types ValueType;

				void* operator new(std::size_t sz);
				void* operator new[](std::size_t sz);
				void operator delete(void* ptr) noexcept;

				static Setting* CreateInstance(const char* name, const Value& value);
				inline void DeleteInstance() { delete this; }
			private:
				/* 00 vtable */
				/* 08 */ Value _Value;
				/* 10 */ char* _Name;
			};
			static_assert(sizeof(Setting) == 0x18);
		}
	}
}
