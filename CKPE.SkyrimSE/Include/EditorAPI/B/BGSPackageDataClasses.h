// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			// Package data (1.7.99.0). The constructors are template instances (BGSNamedPackageData, BGSPackageDataSaveableTemplate, BGSPackageDataPointerTemplate)
			// over the IPackageData / IAITarget interfaces; sizes come from the allocations, members are not resolved.

			struct IPackageData
			{
				void* vftable;	// 00
			};
			static_assert(sizeof(IPackageData) == 0x08);

			// vtable RVA 0x30441D8
			class BGSPackageDataBool : public IPackageData
			{
			public:
				std::uint8_t unk08[0x8];	// 08
			};
			static_assert(sizeof(BGSPackageDataBool) == 0x10);

			// vtable RVA 0x30443F0
			class BGSPackageDataFloat : public IPackageData
			{
			public:
				std::uint8_t unk08[0x10];	// 08
			};
			static_assert(sizeof(BGSPackageDataFloat) == 0x18);

			// vtable RVA 0x3044618
			class BGSPackageDataInt : public IPackageData
			{
			public:
				std::uint8_t unk08[0x10];	// 08
			};
			static_assert(sizeof(BGSPackageDataInt) == 0x18);

			// vtable RVA 0x3043558
			class BGSPackageDataList : public IPackageData
			{
			public:
				std::uint8_t unk08[0x18];	// 08
			};
			static_assert(sizeof(BGSPackageDataList) == 0x20);

			// vtable RVA 0x3044C08
			class BGSPackageDataLocation : public IPackageData
			{
			public:
				std::uint8_t unk08[0x18];	// 08
			};
			static_assert(sizeof(BGSPackageDataLocation) == 0x20);

			// vtable RVA 0x3045C38
			class BGSPackageDataObjectList : public IPackageData
			{
			public:
				std::uint8_t unk08[0x38];	// 08
			};
			static_assert(sizeof(BGSPackageDataObjectList) == 0x40);

			// vtable RVA 0x3046188
			class BGSPackageDataRef : public IPackageData
			{
			public:
				std::uint8_t unk08[0x18];	// 08
			};
			static_assert(sizeof(BGSPackageDataRef) == 0x20);

			// vtable RVA 0x3044DA8
			class BGSPackageDataTargetSelector : public IPackageData
			{
			public:
				std::uint8_t unk08[0x10];	// 08
			};
			static_assert(sizeof(BGSPackageDataTargetSelector) == 0x18);

			// vtable RVA 0x3046500
			class BGSPackageDataTopic : public IPackageData
			{
			public:
				std::uint8_t unk08[0x18];	// 08
			};
			static_assert(sizeof(BGSPackageDataTopic) == 0x20);
		}
	}
}