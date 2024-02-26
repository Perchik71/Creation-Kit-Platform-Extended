// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "..\BSSimpleLock.h"

#pragma once

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
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
				/*08*/ char* _Data;
			};

			// 0x18 5 functions RVA 0x3893220
			class BGSLocalizedStringDL : public IBGSLocalizedString
			{
			private:
				// members
				/*08*/ char pad08[0x8];
				/*10*/ char* _Data;
			};

			// 0x10 5 functions RVA 0x3893260
			class BGSLocalizedStringIL : public BGSLocalizedString 
			{};
		}
	}
}