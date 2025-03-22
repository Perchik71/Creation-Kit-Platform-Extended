// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "BGSImpactDataSet.h"
#include "..\BSTArray.h"

#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			// size 0x38
			class BGSFootstep : public TESForm
			{
			public:
				constexpr static uint8_t TYPE_ID = ftFootstep;

				virtual ~BGSFootstep() = default;

				[[nodiscard]] inline const char* GetType() const noexcept(true) { return _Type; }
				[[nodiscard]] inline BGSImpactDataSet* GetImpactDataSet() const noexcept(true) { return _ImpactDataSet; }

				READ_PROPERTY(GetType) const char* Type;
				READ_PROPERTY(GetImpactDataSet) BGSImpactDataSet* ImpactDataSet;
			private:
				const char* _Type;
				BGSImpactDataSet* _ImpactDataSet;
			};
			static_assert(sizeof(BGSFootstep) == 0x38);

			// size 0xA8
			class BGSFootstepSet : public TESForm
			{
			public:
				constexpr static uint8_t TYPE_ID = ftFootstepSet;
			
				virtual ~BGSFootstepSet() = default;

				[[nodiscard]] inline uint32_t GetCurrentIndex() const noexcept(true) { return _CurrentIndex; }
				[[nodiscard]] inline BSTArray<BGSFootstep*>* GetArrayItemByIndex(uint32_t index) noexcept(true) { return _Item + (index * sizeof(BSTArray<BGSFootstep*>)); }
				[[nodiscard]] inline BSTArray<BGSFootstep*>* GetCurrentArrayItem() noexcept(true) { return _Item + (_CurrentIndex * sizeof(BSTArray<BGSFootstep*>)); }

				READ_PROPERTY(GetCurrentIndex) uint32_t CurrentIndex;
				READ_PROPERTY(GetCurrentArrayItem) BSTArray<BGSFootstep*>* CurrentArrayItem;
			private:
				uint32_t _CurrentIndex;
				char pad28[0x4];
				BSTArray<BGSFootstep*> _Item[4];
				char pad90[0x18];
			};
			static_assert(sizeof(BGSFootstepSet) == 0xA8);
		}
	}
}

#pragma pack(pop)