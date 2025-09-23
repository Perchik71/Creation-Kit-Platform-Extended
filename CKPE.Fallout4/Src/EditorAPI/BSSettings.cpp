// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Asserts.h>
#include <EditorAPI/NiAPI/NiMemoryManager.h>
#include <EditorAPI/BSSettings.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			Setting::Setting(const char* name, const Value& value)
			{
				auto len = strlen(name);
				CKPE_ASSERT(len);

				_Name = (char*)NiAPI::NiMemoryManager::Alloc(nullptr, (std::size_t)(len + 1));
				CKPE_ASSERT(_Name);
				memcpy(_Name, name, len);
				_Name[len] = '\0';

				_Value = value;
			}

			Setting::Setting(const Setting& s)
			{
				auto len = strlen(s._Name);
				CKPE_ASSERT(len);

				_Name = (char*)NiAPI::NiMemoryManager::Alloc(nullptr, (std::size_t)(len + 1));
				CKPE_ASSERT(_Name);
				memcpy(_Name, s._Name, len);
				_Name[len] = '\0';

				if (CheckValidValueType(stString))
				{
					auto len = strlen(s._Value.s);
					_Value.s = (char*)NiAPI::NiMemoryManager::Alloc(nullptr, (std::size_t)(len + 1));
					CKPE_ASSERT(_Value.s);
					memcpy(_Value.s, s._Value.s, len);
					_Value.s[len] = '\0';
				}
				else
					_Value = s._Value;
			}

			Setting::~Setting()
			{
				if (CheckValidValueType(stString))
				{
					if (_Value.s)
					{
						NiAPI::NiMemoryManager::Free(nullptr, _Value.s);
						_Value.s = nullptr;
					}
				}
			}

			Setting::Types Setting::GetValueType() const
			{
				if (!_Name) return stError;
				switch (tolower(_Name[0]))
				{
				case 'b':
					return stBool;
				case 'c':
					return stChar;
				case 'h':
					return stUnsignedChar;
				case 'i':
				case 'n':
					return stSignedInt;
				case 'u':
					return stUnsignedInt;
				case 'f':
					return stFloat;
				case 's':
					return stString;
				case 'r':
					return stRGB;
				case 'a':
					return stRGBA;
				default:
					return stError;
				}
			}

			bool Setting::SetBool(bool value)
			{
				bool r = CheckValidValueType(stBool);
				if (r) _Value.b = value;
				return r;
			}

			bool Setting::SetChar(char value)
			{
				bool r = CheckValidValueType(stChar);
				if (r) _Value.c = value;
				return r;
			}

			bool Setting::SetUnsignedChar(std::uint8_t value)
			{
				bool r = CheckValidValueType(stUnsignedChar);
				if (r) _Value.h = value;
				return r;
			}

			bool Setting::SetSignedInt(std::int32_t value)
			{
				bool r = CheckValidValueType(stSignedInt);
				if (r) _Value.i = value;
				return r;
			}

			bool Setting::SetUnsignedInt(std::uint32_t value)
			{
				bool r = CheckValidValueType(stUnsignedInt);
				if (r) _Value.u = value;
				return r;
			}

			bool Setting::SetFloat(float value)
			{
				bool r = CheckValidValueType(stFloat);
				if (r) _Value.f = value;
				return r;
			}

			bool Setting::SetString(const char* value)
			{
				bool r = CheckValidValueType(stString);
				if (r)
				{
					std::uint16_t len = (std::uint16_t)strlen(value);
					auto newStr = (char*)NiAPI::NiMemoryManager::Alloc(nullptr, (std::size_t)len + 1);
					if (!newStr) r = false;
					if (r)
					{
						memcpy(newStr, value, len);
						newStr[len] = '\0';
						if (_Value.s) NiAPI::NiMemoryManager::Free(nullptr, _Value.s);
						_Value.s = newStr;
					}
				}
				return r;
			}

			bool Setting::SetRGB(std::uint8_t r, std::uint8_t g, std::uint8_t b)
			{
				bool res = CheckValidValueType(stRGB);
				if (res)
				{
					_Value.rgba.red = r;
					_Value.rgba.green = g;
					_Value.rgba.blue = b;
					_Value.rgba.alpha = 0xFF;
				}
				return res;
			}

			bool Setting::SetRGBA(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
			{
				bool res = CheckValidValueType(stRGBA);
				if (res)
				{
					_Value.rgba.red = r;
					_Value.rgba.green = g;
					_Value.rgba.blue = b;
					_Value.rgba.alpha = a;
				}
				return res;
			}

			void* Setting::operator new(std::size_t sz)
			{
				if (sz == 0) ++sz;

				if (void* ptr = NiAPI::NiMemoryManager::Alloc(nullptr, (std::size_t)sz))
					return ptr;

				throw std::bad_alloc{};
			}

			void* Setting::operator new[](std::size_t sz)
			{
				if (sz == 0) ++sz;

				if (void* ptr = NiAPI::NiMemoryManager::Alloc(nullptr, (std::size_t)sz))
					return ptr;

				throw std::bad_alloc{};
			}

			void Setting::operator delete(void* ptr) noexcept
			{
				NiAPI::NiMemoryManager::Free(nullptr, ptr);
			}

			Setting* Setting::CreateInstance(const char* name, const Value& value)
			{
				auto len = strlen(name);
				if (!len)
					return nullptr;

				return new Setting(name, value);
			}
		}
	}
}