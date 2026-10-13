// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// Helper classes used as additional bases of Creation Kit form classes that do not inherit BaseFormComponent.
			// Only the layout (size, vtable slot count) was reverse engineered (CK 1.11.240.0).

			// size 0x8, 3 virtual functions
			class IKeywordFormBase
			{
			public:
				virtual ~IKeywordFormBase() = default;
				virtual void sub_008() { return; }
				virtual void sub_010() { return; }
			};
			static_assert(sizeof(IKeywordFormBase) == 0x8);

			// size 0x8, 11 virtual functions
			class ActorValueOwner
			{
			public:
				virtual ~ActorValueOwner() = default;
				virtual void sub_008() { return; }
				virtual void sub_010() { return; }
				virtual void sub_018() { return; }
				virtual void sub_020() { return; }
				virtual void sub_028() { return; }
				virtual void sub_030() { return; }
				virtual void sub_038() { return; }
				virtual void sub_040() { return; }
				virtual void sub_048() { return; }
				virtual void sub_050() { return; }
			};
			static_assert(sizeof(ActorValueOwner) == 0x8);

			// size 0x8, 5 virtual functions
			class BGSOpenCloseForm
			{
			public:
				virtual ~BGSOpenCloseForm() = default;
				virtual void sub_008() { return; }
				virtual void sub_010() { return; }
				virtual void sub_018() { return; }
				virtual void sub_020() { return; }
			};
			static_assert(sizeof(BGSOpenCloseForm) == 0x8);

			// size 0x8, 4 virtual functions
			class BSIAudioEffectChain
			{
			public:
				virtual ~BSIAudioEffectChain() = default;
				virtual void sub_008() { return; }
				virtual void sub_010() { return; }
				virtual void sub_018() { return; }
			};
			static_assert(sizeof(BSIAudioEffectChain) == 0x8);

			// size 0x10, 11 virtual functions
			class BSIMusicTrack
			{
			public:
				virtual ~BSIMusicTrack() = default;
				virtual void sub_008() { return; }
				virtual void sub_010() { return; }
				virtual void sub_018() { return; }
				virtual void sub_020() { return; }
				virtual void sub_028() { return; }
				virtual void sub_030() { return; }
				virtual void sub_038() { return; }
				virtual void sub_040() { return; }
				virtual void sub_048() { return; }
				virtual void sub_050() { return; }

				std::uint8_t unk08[0x8];
			};
			static_assert(sizeof(BSIMusicTrack) == 0x10);

			// size 0x8, 9 virtual functions
			class BSIMusicType
			{
			public:
				virtual ~BSIMusicType() = default;
				virtual void sub_008() { return; }
				virtual void sub_010() { return; }
				virtual void sub_018() { return; }
				virtual void sub_020() { return; }
				virtual void sub_028() { return; }
				virtual void sub_030() { return; }
				virtual void sub_038() { return; }
				virtual void sub_040() { return; }
			};
			static_assert(sizeof(BSIMusicType) == 0x8);

			// size 0x8, 11 virtual functions
			class BSIReverbType
			{
			public:
				virtual ~BSIReverbType() = default;
				virtual void sub_008() { return; }
				virtual void sub_010() { return; }
				virtual void sub_018() { return; }
				virtual void sub_020() { return; }
				virtual void sub_028() { return; }
				virtual void sub_030() { return; }
				virtual void sub_038() { return; }
				virtual void sub_040() { return; }
				virtual void sub_048() { return; }
				virtual void sub_050() { return; }
			};
			static_assert(sizeof(BSIReverbType) == 0x8);

			// size 0x8, 16 virtual functions
			class BSISoundCategory
			{
			public:
				virtual ~BSISoundCategory() = default;
				virtual void sub_008() { return; }
				virtual void sub_010() { return; }
				virtual void sub_018() { return; }
				virtual void sub_020() { return; }
				virtual void sub_028() { return; }
				virtual void sub_030() { return; }
				virtual void sub_038() { return; }
				virtual void sub_040() { return; }
				virtual void sub_048() { return; }
				virtual void sub_050() { return; }
				virtual void sub_058() { return; }
				virtual void sub_060() { return; }
				virtual void sub_068() { return; }
				virtual void sub_070() { return; }
				virtual void sub_078() { return; }
			};
			static_assert(sizeof(BSISoundCategory) == 0x8);

			// size 0x8, 5 virtual functions
			class BSISoundDescriptor
			{
			public:
				virtual ~BSISoundDescriptor() = default;
				virtual void sub_008() { return; }
				virtual void sub_010() { return; }
				virtual void sub_018() { return; }
				virtual void sub_020() { return; }
			};
			static_assert(sizeof(BSISoundDescriptor) == 0x8);

			// size 0x8, 16 virtual functions
			class BSISoundOutputModel
			{
			public:
				virtual ~BSISoundOutputModel() = default;
				virtual void sub_008() { return; }
				virtual void sub_010() { return; }
				virtual void sub_018() { return; }
				virtual void sub_020() { return; }
				virtual void sub_028() { return; }
				virtual void sub_030() { return; }
				virtual void sub_038() { return; }
				virtual void sub_040() { return; }
				virtual void sub_048() { return; }
				virtual void sub_050() { return; }
				virtual void sub_058() { return; }
				virtual void sub_060() { return; }
				virtual void sub_068() { return; }
				virtual void sub_070() { return; }
				virtual void sub_078() { return; }
			};
			static_assert(sizeof(BSISoundOutputModel) == 0x8);

			// size 0x50, 2 virtual functions
			class BSMaterialObject
			{
			public:
				virtual ~BSMaterialObject() = default;
				virtual void sub_008() { return; }

				std::uint8_t unk08[0x48];
			};
			static_assert(sizeof(BSMaterialObject) == 0x50);

			// size 0x228, 2 virtual functions
			class BSNavmesh
			{
			public:
				virtual ~BSNavmesh() = default;
				virtual void sub_008() { return; }

				std::uint8_t unk08[0x220];
			};
			static_assert(sizeof(BSNavmesh) == 0x228);

			// size 0x10, 6 virtual functions
			class BSNavmeshInfoMap
			{
			public:
				virtual ~BSNavmeshInfoMap() = default;
				virtual void sub_008() { return; }
				virtual void sub_010() { return; }
				virtual void sub_018() { return; }
				virtual void sub_020() { return; }
				virtual void sub_028() { return; }

				std::uint8_t unk08[0x8];
			};
			static_assert(sizeof(BSNavmeshInfoMap) == 0x10);

			// size 0x8, 26 virtual functions
			class IAnimationGraphManagerHolder
			{
			public:
				virtual ~IAnimationGraphManagerHolder() = default;
				virtual void sub_008() { return; }
				virtual void sub_010() { return; }
				virtual void sub_018() { return; }
				virtual void sub_020() { return; }
				virtual void sub_028() { return; }
				virtual void sub_030() { return; }
				virtual void sub_038() { return; }
				virtual void sub_040() { return; }
				virtual void sub_048() { return; }
				virtual void sub_050() { return; }
				virtual void sub_058() { return; }
				virtual void sub_060() { return; }
				virtual void sub_068() { return; }
				virtual void sub_070() { return; }
				virtual void sub_078() { return; }
				virtual void sub_080() { return; }
				virtual void sub_088() { return; }
				virtual void sub_090() { return; }
				virtual void sub_098() { return; }
				virtual void sub_0A0() { return; }
				virtual void sub_0A8() { return; }
				virtual void sub_0B0() { return; }
				virtual void sub_0B8() { return; }
				virtual void sub_0C0() { return; }
				virtual void sub_0C8() { return; }
			};
			static_assert(sizeof(IAnimationGraphManagerHolder) == 0x8);

			// Empty helper base (no data).
			class BGSNavmeshableObject {};

			// Empty helper base (no data).
			class TESMagicCasterForm {};

			// Empty helper base (no data).
			class TESMagicTargetForm {};

			// Empty helper base, singleton marker.
			template<class T>
			class BSTSingletonImplicit {};

		}
	}
}
