// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <cstddef>
#include <EditorAPI/B/BSTList.h>
#include <EditorAPI/B/BSString.h>
#include <EditorAPI/C/CheckInFormList.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESForm;
			}

			class TESRegionObject;
			class TESRegionGrassObject;

			// 1.11.246.0: size 0x10, vtable RVA 0x302FA80, 6 virtual functions. Base of the objects and grass placements of a region
			// (TESRegionDataObjects / TESRegionDataGrass lists).
			class TESRegionObjectBase
			{
			public:
				virtual ~TESRegionObjectBase() = default; // 00

			protected:
				virtual std::uint32_t sub_008() const = 0; // 01 - returns 1 in TESRegionGrassObject, 0 in TESRegionObject

			public:
				[[nodiscard]] virtual Forms::TESForm* GetObject() const = 0; // 02 - the placed form
				// Removes what refers to Form
				virtual void RemoveReferencesTo(Forms::TESForm* Form) = 0; // 03
				[[nodiscard]] virtual bool CanCheckIn(const Forms::CheckInFormList* Forms, Forms::TESForm* ReferencingForm) const = 0; // 04
				virtual void BuildCheckInDependencyMessage(const Forms::CheckInFormList* Forms, BSString& Message, Forms::TESForm* ReferencingForm) const = 0; // 05

			public:
				Forms::TESForm* object; // 08
			};
			static_assert(sizeof(TESRegionObjectBase) == 0x10);

			// 1.11.246.0: size 0x20, vtable RVA 0x302FD58 (a TESRegionGrassObject list; frees the entries when ownsObjects is set)
			class TESRegionGrassObjectList
			{
			public:
				virtual ~TESRegionGrassObjectList() = default; // 00

			public:
				BSSimpleList<TESRegionGrassObject*> objects; // 08
				std::uint8_t ownsObjects; // 18
				std::uint8_t pad19[0x3]; // 19
				std::uint32_t count; // 1C
			};
			static_assert(sizeof(TESRegionGrassObjectList) == 0x20);

			// 1.11.246.0: size 0x20, vtable RVA 0x302FF28 (frees the entries when ownsObjects is set)
			class TESRegionObjectList
			{
			public:
				virtual ~TESRegionObjectList() = default; // 00

			public:
				BSSimpleList<TESRegionObject*> objects; // 08
				std::uint8_t ownsObjects; // 18
				std::uint8_t pad19[0x3]; // 19
				std::uint32_t count; // 1C
			};
			static_assert(sizeof(TESRegionObjectList) == 0x20);

			// 1.11.246.0: size 0x18, vtable RVA 0x302FAC8, 10 virtual functions. A grass type (object) and an optional land texture
			class TESRegionGrassObject :
				public TESRegionObjectBase   // 00
			{
			public:
				virtual ~TESRegionGrassObject() = default; // 00

				[[nodiscard]] virtual Forms::TESForm* GetTexture() const; // 06
				virtual void SetTexture(Forms::TESForm* Texture); // 07
				// Returns Dest when it is not null, else a new object with the same object and texture
				virtual TESRegionGrassObject* Clone(TESRegionGrassObject* Dest) const; // 08
				// Samples the height/slope data of the cell at a position for this grass type (returns a float in xmm0)
				virtual float sub_048(const void* Position, std::int32_t Value, std::int8_t Flag) const; // 09

			public:
				Forms::TESForm* texture; // 10
			};
			static_assert(sizeof(TESRegionGrassObject) == 0x18);

			// 1.11.246.0: size 0x50, vtable RVA 0x302FDF0, 6 virtual functions. The RDOT record of an objects region data (the
			// names of the members follow the RDOT record fields, the slopes/radius clamps in the constructor confirm them)
			class TESRegionObject :
				public TESRegionObjectBase   // 00
			{
			public:
				virtual ~TESRegionObject() = default; // 00

			public:
				float density; // 10
				std::uint8_t clustering; // 14
				std::uint8_t minSlope; // 15 - raised to the parent's value
				std::uint8_t maxSlope; // 16 - lowered to the parent's value, default 90
				std::uint8_t flags; // 17
				std::uint16_t radiusWrtParent; // 18
				std::uint16_t radius; // 1A
				float minHeight; // 1C
				float maxHeight; // 20
				float sink; // 24
				float sinkVariance; // 28
				float sizeVariance; // 2C
				std::uint16_t angleVariance[3]; // 30 - x, y, z
				std::uint16_t pad36; // 36
				std::uint32_t vertexShading; // 38 - default 0xFFFFFF
				std::uint32_t pad3C; // 3C
				TESRegionObjectList* children; // 40 - objects placed relative to this one
				TESRegionObject* parent; // 48
			};
			static_assert(offsetof(TESRegionObject, density) == 0x10);
			static_assert(offsetof(TESRegionObject, vertexShading) == 0x38);
			static_assert(offsetof(TESRegionObject, children) == 0x40);
			static_assert(sizeof(TESRegionObject) == 0x50);

			// 1.11.246.0: size 0x10, vtable RVA 0x302FD98
			class TESRegionNoiseFunction
			{
			public:
				virtual ~TESRegionNoiseFunction() = default; // 00

			public:
				std::uint16_t unk08; // 08 - 1 after construction (the constructor reduces any value above 1 to 1, 0 to 1)
				std::uint16_t unk0A; // 0A - at most 4
				std::uint32_t pad0C; // 0C
			};
			static_assert(sizeof(TESRegionNoiseFunction) == 0x10);
		}
	}
}
