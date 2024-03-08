// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "BSEntryString.h"

#pragma once

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			// size 0x8 
			// func 5
			// abstract class (don't use directly)
			class IBGSLocalizedString 
			{
			public:
				virtual ~IBGSLocalizedString() = default;
				virtual const char* c_str() const;
			};

			// 0x10 5 
			class BGSLocalizedString : public IBGSLocalizedString
			{
			private:
				// members
				/*08*/ BSEntryString* _Data;
			public:
				inline const char* Get() const { return _Data ? _Data->Get<char>(true) : nullptr; }
			};

			// 0x18 5 functions
			class BGSLocalizedStringDL : public IBGSLocalizedString
			{
			private:
				// members
				/*08*/ char pad08[0x8];
				/*10*/ BSEntryString* _Data;

			public:
				inline static uintptr_t OrigSet = 0;
				inline const char* Get() const { return _Data ? _Data->Get<char>(true) : nullptr; }
				inline void Set(const char* str)
				{
					((void(__fastcall*)(BGSLocalizedStringDL*, const char*))OrigSet)(this, str);
				}
				inline void Set(const BSEntryString* str)
				{
					((void(__fastcall*)(BGSLocalizedStringDL*, const char*))OrigSet)(this, str->Get<char>(true));
				}
			};

			// 0x10 5 functions
			class BGSLocalizedStringIL : public BGSLocalizedString 
			{};
		}
	}
}