// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "..\BSString.h"
#include "BSResourceEntryDB.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			namespace BSResource
			{
				void* Cache;

				BSHashDB* (*CreateHashDB)(BSHashDB* Hash, const char* Path);
				bool (*GetTextureDBTraits)(BSHashDB* Hash, BSTextureDB::DBTraits* Traits);
				bool (*GetTextureDBTraitsFromCache)(void* Cache, BSHashDB* Hash, BSTextureDB::DBTraits* Traits);
				bool (*CreateStringEntryDB)(BSFixedString* String);
				bool (*ReleaseStringEntryDB)(BSFixedString* String);

				void ReleaseHashDB(BSHashDB* Hash)
				{
					Hash->Hash = 0xDEADBEEF;
					Hash->Format = 0xDEADBEEF;
					Hash->Unk8 = 0xDEADBEEF;
				}

				bool BSTextureDB::Exists(const char* Path)
				{
					if (!Path || Path[0] == '\0')
						return false;

					// The function is taken from code snippets, 
					// I do not know for sure how it works, but it works.
					// In this case, the function searches the database for a texture with a similar hash, 
					// if it finds it fills in the structure, it returns the true I used this.

					bool Result = false;
					BSHashDB Hash;
					DBTraits Traits;
					BSString FilePath;

					memset(&Hash, 0, sizeof(BSHashDB));
					memset(&Traits, 0, sizeof(DBTraits));

					// The function does not accept Data\\, because we only need textures, I will compare it with the symbol D.
					FilePath.Assign(Path, tolower(Path[0]) == 'd' ? 5 : 0);
					// Creates an empty BSFixedString
					CreateStringEntryDB(&Traits.Path);

					{
						// Create hash
						CreateHashDB(&Hash, FilePath.c_str());
						// I have no idea what it is or what it's for.
						Traits.Unk0C[2] = 0x18;
						// Getting a texture by hash
						// Note: It looks like the function returns everything in the archives
						Result = GetTextureDBTraits(&Hash, &Traits);	
						// Note: Removed, does not work well, requires study
						//if (!Result && (*(uint8_t**)Cache))
						//	// If it is not found, try to find it in the cache.
						//	Result = (bool)GetTextureDBTraitsFromCache((*(uint8_t**)Cache) + 0x100, &Hash, &Traits);						
						// Relese hash
						ReleaseHashDB(&Hash);
					}

					// Release BSFixedString
					ReleaseStringEntryDB(&Traits.Path);

					if (!Result)
						// It's hopeless, trying to find something using Windows
						// Note: It finds loose files perfectly (maybe no MO2)
						Result = BSString::Utils::FileExists(BSString::Utils::GetDataPath() + FilePath);

					return Result;
				}
			}
		}
	}
}