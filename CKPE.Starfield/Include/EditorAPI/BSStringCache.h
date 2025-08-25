// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <cstdint>
#include <EditorAPI/BSSimpleLock.h>

#pragma once

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			class BSStringCache
			{
			public:
				struct Lock
				{
					SimpleLock lock;
				};

				struct Entry
				{
					Entry* next;		// 00
					union
					{
						struct
						{
							std::uint16_t refCount;	// invalid if 0x8000 is set
							std::uint16_t hash;
						};
						std::uint32_t refCountAndHash;
					} state;				// 08 - refcount, hash
					std::uint64_t length;	// 10
					char* data;				// 18

					inline const char* c_str() const noexcept(true) { return data; }
					inline operator const char*() const { return data ? data : ""; }
				};

				struct Ref
				{
					char* data{ nullptr };

					// For 1.6.378.1
					//260BC00 (ctor, Ref*, 0x00CEC5D0, const char* buf);
					//260BC70 (ctor_ref, Ref*, 0x00CEC680, const Ref& rhs);
					//260BD50 (Set, Ref*, 0x00CEC760, const char* buf);
					//260BDD0 (Set_ref, Ref*, 0x00CEC820, const Ref& rhs);
					//260E040 (Release, void, 0x00CED9A0);

					constexpr Ref() noexcept(true) = default;

					inline bool operator==(const Ref& lhs) const noexcept(true) { return data == lhs.data; }
					inline bool operator<(const Ref& lhs) const noexcept(true) { return data < lhs.data; }

					inline const char* c_str() const { return data; }
					inline operator const char*() const { return data ? data : ""; }
				};

				BSStringCache() = default;
				~BSStringCache() = default;

				inline Lock* GetLock(std::uint32_t crc16) { return &locks[crc16 & 0x1F]; }
			private:
				Entry* lut[0x10000];
				Lock locks[0x20];
				std::uint8_t isInit;
			};

			static_assert(sizeof(BSStringCache::Lock) == 0x8);
			static_assert(sizeof(BSStringCache::Ref) == 0x8);
			static_assert(sizeof(BSStringCache::Entry) == 0x20);
		}
	}
}