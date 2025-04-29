// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once
#pragma pack(push, 1)

#include "BSFixedString.h"
#include "..\BGSLocalizedString.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Starfield
		{
			class BaseFormComponent
			{
			public:
				virtual ~BaseFormComponent() = default;

				virtual void Unk08();
				virtual void Unk10();
				virtual void Unk18();
				virtual void Unk20();
				virtual void Unk28();
				virtual BSFixedString GetComponentName() const;
				virtual void Unk38();
				virtual void Unk40();
				virtual void Unk48();
				virtual void Unk50();
				virtual void Unk58();
				virtual void Unk60();
				virtual void Unk68();
				virtual void Unk70();
				virtual void Unk78();
				virtual void Unk80();
				virtual void Unk88();
				virtual void Unk90();
				virtual void Unk98();
				virtual void UnkA0();
				virtual void UnkA8();
				virtual void UnkB0();
				virtual void UnkB8();
				virtual void UnkC0();
				virtual void UnkC8();
				virtual void UnkD0();
				virtual void UnkD8();
				virtual void UnkE0();
				virtual void UnkE8();
			};
			static_assert(sizeof(BaseFormComponent) == 0x08);
		}


	}
}

#pragma pack(pop)
