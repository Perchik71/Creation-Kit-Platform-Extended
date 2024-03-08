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
			class BSStringCache {
			public:
				struct Lock {
					SimpleLock lock;
				};

				struct Entry {
					Entry* next;		// 00
					union
					{
						struct
						{
							uint16_t refCount;	// invalid if 0x8000 is set
							uint16_t hash;
						};
						uint32_t refCountAndHash;
					} state;			// 08 - refcount, hash
					uint64_t length;	// 10
					LPSTR data;			// 18

					inline LPCSTR c_str() const { return data; }
				};

				struct Ref {
					LPSTR data;

					Ref() : data(nullptr) {}

					inline bool operator==(const Ref& lhs) const { return data == lhs.data; }
					inline bool operator<(const Ref& lhs) const { return data < lhs.data; }

					inline LPCSTR c_str() const { return data; }
				};

				BSStringCache() = default;
				~BSStringCache() = default;

				inline Lock* GetLock(ULONG crc16) { return &locks[crc16 & 0x1F]; }
			private:
				Entry* lut[0x10000];
				Lock locks[0x20];
				BYTE isInit;
			};

			static_assert(sizeof(BSStringCache::Lock) == 0x8);
			static_assert(sizeof(BSStringCache::Ref) == 0x8);
			static_assert(sizeof(BSStringCache::Entry) == 0x20);
		}
	}
}