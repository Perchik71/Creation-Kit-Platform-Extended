// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BSFixedString.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace BSResource
			{
				struct BSHashDB
				{
					std::uint32_t Hash;		// 00
					std::uint32_t Format;	// 04
					std::uint32_t Unk8;		// 08
				};
				static_assert(sizeof(BSHashDB) == 0xC);

				class BSTextureDB
				{
				public:
					struct DBTraits
					{
						BSHashDB Hash;		// 00
						char Unk0C[0x6C];	// 0C
						BSFixedString Path;	// 78
					};

					static bool Exists(const char* Path) noexcept(true);
				};
				static_assert(sizeof(BSTextureDB::DBTraits) == 0x80);

				extern void* Cache;
				extern BSHashDB* (*CreateHashDB)(BSHashDB* Hash, const char* Path);
				extern bool (*GetTextureDBTraits)(BSHashDB* Hash, BSTextureDB::DBTraits* Traits);
				extern bool (*GetTextureDBTraitsFromCache)(void* Cache, BSHashDB* Hash, BSTextureDB::DBTraits* Traits);
				static void ReleaseHashDB(BSHashDB* Hash) noexcept(true);

				extern bool (*CreateStringEntryDB)(BSFixedString* String);
				extern bool (*ReleaseStringEntryDB)(BSFixedString* String);
			}
		}
	}
}