// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BGSLocalizedString.h>
#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/B/BSTHashMap.h>
#include <EditorAPI/N/NiPoint.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSTextureSet;
			}

			// Race face/body morph data of the Creation Kit (CommonLibF4 BGSCharacterMorph, with the CK's differences noted).
			namespace BGSCharacterMorph
			{
				// 1.11.246.0: size 0x24 (BSMS chunk, boneBaseScales map)
				class BoneBaseScales
				{
				public:
					NiAPI::NiPoint3 scales[3]; // 00
				};
				static_assert(sizeof(BoneBaseScales) == 0x24);

				// 1.11.246.0: size 0x10 (boneModifiers map)
				class BONE_MODIFIER_MIN_MAX
				{
				public:
					float minY; // 00
					float minZ; // 04
					float maxY; // 08
					float maxZ; // 0C
				};
				static_assert(sizeof(BONE_MODIFIER_MIN_MAX) == 0x10);

				class Transform
				{
				public:
					NiAPI::NiPoint3 position; // 00
					NiAPI::NiPoint3 rotation; // 0C
					NiAPI::NiPoint3 scale; // 18
				};
				static_assert(sizeof(Transform) == 0x24);

				class TransformMinMax
				{
				public:
					Transform minima; // 00
					Transform maxima; // 24
				};
				static_assert(sizeof(TransformMinMax) == 0x48);

				// 1.11.246.0: size 0x28 (the game's is 0x20: the name is a 0x10 byte localized string in the CK). Elements of Group::presets.
				class Preset
				{
				public:
					BGSLocalizedString name; // 00 - MPPN
					BSFixedString morph; // 10 - MPPM
					Forms::BGSTextureSet* textureSet; // 18 - MPPT (a form id until resolved)
					std::int8_t flags; // 20 - MPPF
					std::uint8_t pad21[3]; // 21
					std::uint32_t id; // 24 - MPPI
				};
				static_assert(sizeof(Preset) == 0x28);

				// 1.11.246.0: size 0x40, same layout as the game's (MPGN/MPGS/MPPK chunks)
				class Group
				{
				public:
					BSFixedString name; // 00 - MPGN
					BSTArray<Preset> presets; // 08
					BSTArray<std::uint32_t> sliders; // 20 - MPGS
					std::uint16_t presetMaskID; // 38 - MPPK
					std::uint8_t pad3A[6]; // 3A
				};
				static_assert(sizeof(Group) == 0x40);

				// 1.11.246.0: size 0xA0 (the game's is 0x90: the name is a localized string object and the hash map has the CK's 0x38 byte layout).
				// FMRI creates the region with its id, FMRN loads the name.
				class FacialBoneRegion
				{
				public:
					Transform defaultTransform; // 00
					std::uint8_t pad24[4]; // 24
					BGSLocalizedString name; // 28 - FMRN
					BSFixedString associatedMorphGroupName; // 38
					std::int32_t associatedTintSlot; // 40 - BGSCharacterTint::EntrySlot
					std::uint32_t associatedHeadPartType; // 44 - BGSHeadPart::HeadPartType
					BSTHashMap<BSFixedString, TransformMinMax*> boneMinMaxMap; // 48
					BSTArray<std::uint32_t> associatedMorphSliders; // 80
					std::uint32_t id; // 98 - FMRI
					std::uint32_t pad9C; // 9C
				};
				static_assert(offsetof(FacialBoneRegion, name) == 0x28);
				static_assert(offsetof(FacialBoneRegion, boneMinMaxMap) == 0x48);
				static_assert(offsetof(FacialBoneRegion, associatedMorphSliders) == 0x80);
				static_assert(sizeof(FacialBoneRegion) == 0xA0);
			}

			namespace BGSCharacterTint
			{
				namespace Template
				{
					// 1.11.246.0: size 0x38, the polymorphic base of the tint entries (the factory 0x14041DEA0 creates Mask for slots below 7,
					// Palette for 7-20 and TextureSet from 21). TTET creates the entry, TTEF loads the flags, CTDA the conditions.
					class Entry
					{
					public:
						virtual ~Entry() = default;

					public:
						BGSLocalizedString name; // 08
						std::uint8_t chargenConditions[0x18]; // 18 - TESCondition (0x18 bytes in the CK)
						std::int32_t slot; // 30 - BGSCharacterTint::EntrySlot
						std::uint16_t uniqueID; // 34
						std::int8_t flags; // 36 - TTEF chunk
						std::uint8_t pad37; // 37
					};
					static_assert(offsetof(Entry, name) == 0x8);
					static_assert(offsetof(Entry, chargenConditions) == 0x18);
					static_assert(offsetof(Entry, slot) == 0x30);
					static_assert(sizeof(Entry) == 0x38);

					// 1.11.246.0: size 0x48
					class Mask :
						public Entry   // 00
					{
					public:
						BSFixedString maskTexture; // 38
						std::int32_t blendOp; // 40 - 0 on construction
						std::uint32_t pad44; // 44
					};
					static_assert(sizeof(Mask) == 0x48);

					// 1.11.246.0: size 0x60
					class Palette :
						public Entry   // 00
					{
					public:
						BSFixedString maskTextureName; // 38
						std::int32_t defaultIndex; // 40 - -1 on construction
						std::uint32_t pad44; // 44
						BSTArray<void*> colorValues; // 48 - the game's ColorValue is 0x18 bytes {BGSColorForm*, value, blend op, swatch id}
					};
					static_assert(sizeof(Palette) == 0x60);

					// 1.11.246.0: size 0x58
					class TextureSet :
						public Entry   // 00
					{
					public:
						BSFixedString unk38; // 38
						BSFixedString unk40; // 40
						BSFixedString unk48; // 48
						std::uint64_t unk50; // 50 - 0 on construction
					};
					static_assert(sizeof(TextureSet) == 0x58);

					// 1.11.246.0: a tint group (TTGP / TTGR chunks); the CK object is bigger than the game's 0x28 because the name is a 0x10 byte
					// localized string object.
					class Group
					{
					public:
						BGSLocalizedString name; // 00
						std::uint32_t unk10; // 10
						std::uint32_t chargenIndex; // 14 - read at the end of the group loader
						BSTArray<Entry*> entries; // 18 - TTET chunks create the entries
					};
					static_assert(offsetof(Group, entries) == 0x18);

					// The array object behind a race's TTGP data
					class Groups
					{
					public:
						BSTArray<Group*> groups; // 00
					};
					static_assert(sizeof(Groups) == 0x18);
				}

				// 1.11.246.0: the 24 byte array object behind TESNPC::tintingData
				class Entries
				{
				public:
					BSTArray<Template::Entry*> entries; // 00
				};
				static_assert(sizeof(Entries) == 0x18);
			}
		}
	}
}
