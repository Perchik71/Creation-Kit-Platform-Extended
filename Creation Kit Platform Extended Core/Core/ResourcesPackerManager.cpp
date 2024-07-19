// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Engine.h"
#include "ResourcesPackerManager.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		constexpr static char szResourceFileNameFormated[] = "CreationKitPlatformExtended_%s_Resources.pak";

		EditorAPI::BSString ResourcesPackerManager::GetResourcesFileName() noexcept
		{
			auto ShortNameGameIterator = allowedShortNameGame.find(GetShortExecutableTypeFromFull(GlobalEnginePtr->GetEditorVersion()));
			if (ShortNameGameIterator == allowedShortNameGame.end())
				return "";

			return EditorAPI::BSString::FormatString(szResourceFileNameFormated, ShortNameGameIterator->second.data());
		}

		bool ResourcesPackerManager::HasResources() noexcept
		{
			return EditorAPI::BSString::Utils::FileExists(GetResourcesFileName());
		}

		bool ResourcesPackerManager::UnpackResources() noexcept
		{
			auto ShortNameGame = GetShortExecutableTypeFromFull(GlobalEnginePtr->GetEditorVersion());
			// Everything is fine in Skyrim and it is not necessary.
			if (ShortNameGame == EDITOR_SHORT_SKYRIM_SE) return false;

			auto FileName = GetResourcesFileName();
			if (FileName.IsEmpty()) return false;

			if (!EditorAPI::BSString::Utils::FileExists(FileName))
			{
				_FATALERROR("RESOURCES: Archive \"%s\" no found", FileName.c_str());
				return false;
			}

			// open archive
			struct zip_t* zip = zip_open(FileName.c_str(), 0, 'r');
			if (!zip)
			{
				_FATALERROR("RESOURCES: Failed open archive \"%s\"", FileName.c_str());
				return false;
			}

			_MESSAGE("RESOURCES: Open archive \"%s\"", FileName.c_str());

			if (ShortNameGame == EDITOR_SHORT_FALLOUT_C4)
			{
				if (!ExtractResourceByName(zip, "CreationKit - Textures.ba2"))
					_FATALERROR("RESOURCES: File could not be unpacked \"CreationKit - Textures.ba2\"");

				EditorAPI::BSString sShaderName =
					(GlobalEnginePtr->GetEditorVersion() == EDITOR_FALLOUT_C4_1_10_162_0) ?
					"CreationKit - Shaders - OG.ba2" : "CreationKit - Shaders - NG.ba2";

				if (!ExtractResourceByName(zip, sShaderName, "CreationKit - Shaders.ba2", true))
					_FATALERROR("RESOURCES: File could not be unpacked \"%s\"", sShaderName.c_str());
			}

			zip_close(zip);
			return true;
		}

		bool ResourcesPackerManager::ExtractResourceByName(void* zip, const EditorAPI::BSString& fileName,
			bool overwrite) noexcept
		{
			return ExtractResourceByName(zip, fileName, fileName, overwrite);
		}

		bool ResourcesPackerManager::ExtractResourceByName(void* zip, const EditorAPI::BSString& fileName, 
			const EditorAPI::BSString& fileNewName, bool overwrite) noexcept
		{
			if (!zip || fileName.IsEmpty())
				return false;

			if (!overwrite && EditorAPI::BSString::Utils::FileExists(EditorAPI::BSString::Utils::GetDataPath() + fileName))
				return true;

			EditorAPI::BSString sName;
			ssize_t nCount = zip_entries_total((struct zip_t*)zip);

			for (ssize_t i = 0; i < nCount; ++i)
			{
				zip_entry_openbyindex((struct zip_t*)zip, i);
				
				// skip dirs
				if (zip_entry_isdir((struct zip_t*)zip) != 0)
				{
					zip_entry_close((struct zip_t*)zip);
					continue;
				}

				if (!fileName.Compare(zip_entry_name((struct zip_t*)zip)))
				{
					auto ret = zip_entry_fread((struct zip_t*)zip, (EditorAPI::BSString::Utils::GetDataPath() + fileNewName).c_str());
					zip_entry_close((struct zip_t*)zip);
					return !ret;
				}

				zip_entry_close((struct zip_t*)zip);
			}

			return false;
		}
	}
}