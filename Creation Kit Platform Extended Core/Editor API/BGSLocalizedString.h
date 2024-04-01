// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		// size 0x8 
		// func 5
		// abstract class (don't use directly)
		class IBGSLocalizedString 
		{
		public:
			virtual void release(bool free = false);
			virtual void unk08() const;					// tag ???
			virtual void unk10() const;					// ^
			virtual uint32_t length() const;
			virtual const char* c_str() const;
		};

		// 0x10 5 
		template<typename _Ty>
		class BGSLocalizedString : public IBGSLocalizedString
		{
		public:
			inline _Ty* data() { return _Str; }
		private:
			// members
			/*08*/ _Ty* _Str;
		};

		// 0x18 5 functions
		template<typename _Ty>
		class BGSLocalizedStringDL : public IBGSLocalizedString
		{
		public:
			inline _Ty* data() { return _Str; }
		private:
			// members
			/*08*/ char pad08[0x8];
			/*10*/ _Ty* _Str;
		};

		// 0x10 5 functions
		template<typename _Ty>
		class BGSLocalizedStringIL : public BGSLocalizedString<_Ty>
		{};
	}
}