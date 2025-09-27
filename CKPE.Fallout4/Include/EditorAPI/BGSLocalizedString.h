// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/BSFixedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// size 0x8 
			// func 5
			// abstract class (don't use directly)
			class IBGSLocalizedString
			{
			public:
				virtual void release(bool free = false) noexcept(true);
				virtual void unk08() const noexcept(true);					// tag ???
				virtual void unk10() const noexcept(true);					// ^
				virtual std::uint32_t length() const noexcept(true);
				virtual const char* c_str() const noexcept(true);
			};
			static_assert(sizeof(IBGSLocalizedString) == 0x8);

			// 0x10 5
			class BGSLocalizedString : public IBGSLocalizedString
			{
			public:
				[[nodiscard]] inline bool is_wide() const noexcept(true) { return _Str.is_wide(); }
				[[nodiscard]] inline char* data() noexcept(true) { return const_cast<char*>(_Str.c_str()); }
				[[nodiscard]] inline wchar_t* wdata() noexcept(true) { return const_cast<wchar_t*>(_Str.c_wstr()); }
			private:
				// members
				/*08*/ BSFixedString _Str;
			};
			static_assert(sizeof(BGSLocalizedString) == 0x10);

			// 0x18 5 functions
			class BGSLocalizedStringDL : public IBGSLocalizedString
			{
			public:
				[[nodiscard]] inline bool is_wide() const noexcept(true) { return _Str.is_wide(); }
				[[nodiscard]] inline char* data() noexcept(true) { return const_cast<char*>(_Str.c_str()); }
				[[nodiscard]] inline wchar_t* wdata() noexcept(true) { return const_cast<wchar_t*>(_Str.c_wstr()); }
			private:
				// members
				/*08*/ std::uint32_t id;
				/*10*/ BSFixedString _Str;
			};
			static_assert(sizeof(BGSLocalizedStringDL) == 0x18);

			// 0x10 5 functions
			class BGSLocalizedStringIL : public BGSLocalizedString
			{};
		}
	}
}