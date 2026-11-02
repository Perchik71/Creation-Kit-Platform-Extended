// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <cstddef>
#include <EditorAPI/N/NiRefObject.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// Render objects of a water form (shared/water/teswater.cpp). All three derive from NiRefObject and add no virtual functions.

			// 1.11.246.0: size 0x68, vtable RVA 0x310B878
			class TESWaterObject :
				public NiAPI::NiRefObject   // 00
			{
			public:
				constexpr static Common::ID RTTI = IDs::RTTI::TESWaterObject;
				constexpr static Common::ID VTABLE = IDs::VTABLE::TESWaterObject[0];

				std::uint8_t unk10[0x10]; // 10
				NiAPI::NiRefObject* unk20; // 20 - reference counted, set by the constructor
				NiAPI::NiRefObject* unk28; // 28 - reference counted
				void* unk30; // 30 - released in the destructor when its +0x260 member has +8 == 2
				NiAPI::NiRefObject* unk38; // 38 - reference counted
				NiAPI::NiRefObject* unk40; // 40 - reference counted
				std::uint8_t unk48[0x10]; // 48 - cleaned up by the destructor
				std::uint8_t unk58[0x8]; // 58
				std::uint8_t unk60; // 60 - cleared by the constructor
				std::uint8_t pad61[0x7]; // 61
			};
			static_assert(sizeof(TESWaterObject) == 0x68);

			// 1.11.246.0: size 0x88, vtable RVA 0x310B898
			class TESWaterReflections :
				public NiAPI::NiRefObject   // 00
			{
			public:
				constexpr static Common::ID RTTI = IDs::RTTI::TESWaterReflections;
				constexpr static Common::ID VTABLE = IDs::VTABLE::TESWaterReflections[0];

				std::uint8_t unk10; // 10
				std::uint8_t pad11; // 11
				std::uint16_t flags; // 12
				float plane[4]; // 14 - copied into the water material (offset 0x148)
				std::uint32_t pad24; // 24
				NiAPI::NiRefObject* unk28; // 28 - reference counted
				NiAPI::NiRefObject* unk30; // 30 - reference counted
				NiAPI::NiRefObject* unk38; // 38 - owned, destroyed through its virtual function 24
				void* waterMaterial; // 40 - smart pointer to the water material (reference counted at +8)
				std::uint8_t unk48[0x38]; // 48
				std::uint8_t unk80; // 80 - cleared by the constructor
				std::uint8_t pad81[0x7]; // 81
			};
			static_assert(offsetof(TESWaterReflections, plane) == 0x14);
			static_assert(sizeof(TESWaterReflections) == 0x88);

			// 1.11.246.0: size 0x48, vtable RVA 0x310B8B8
			class TESWaterNormals :
				public NiAPI::NiRefObject   // 00
			{
			public:
				constexpr static Common::ID RTTI = IDs::RTTI::TESWaterNormals;
				constexpr static Common::ID VTABLE = IDs::VTABLE::TESWaterNormals[0];

				std::uint8_t unk10; // 10
				std::uint8_t pad11[0x7]; // 11
				void* owner; // 18 - second constructor argument
				std::uint32_t unk20; // 20
				std::uint32_t pad24; // 24
				void* waterMaterial; // 28 - smart pointer to the water material (reference counted at +8)
				void* unk30; // 30 - released by the destructor
				void* unk38; // 38 - released by the destructor
				void* unk40; // 40 - released by the destructor
			};
			static_assert(sizeof(TESWaterNormals) == 0x48);
		}
	}
}
