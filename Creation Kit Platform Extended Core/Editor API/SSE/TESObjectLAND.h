// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "TESForm.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			class TESChildObjectLAND
			{
			public:
				TESChildObjectLAND() = default;
				virtual ~TESChildObjectLAND() = default;
			private:
				uint64_t Unk08;
			};

			class TESObjectLAND : public TESForm, public TESChildObjectLAND
			{
			public:
				TESObjectLAND() = default;
				virtual ~TESObjectLAND() = default;

				TESForm* GetParentCell() const { return ParentCell; }
			private:
				TESForm* ParentCell;
				uint64_t Unk40;
				uint64_t NodeArrayPtrLand;				// maybe textures
			};
			static_assert(sizeof(TESObjectLAND) == 0x50);
		}
	}
}