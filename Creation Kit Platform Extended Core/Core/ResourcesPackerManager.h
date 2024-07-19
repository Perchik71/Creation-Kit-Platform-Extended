// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Editor API/BSString.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		class ResourcesPackerManager
		{
		public:
			ResourcesPackerManager() = default;
			~ResourcesPackerManager() = default;

			static EditorAPI::BSString GetResourcesFileName() noexcept;
			static bool HasResources() noexcept;
			static bool UnpackResources() noexcept;

			static bool ExtractResourceByName(void* zip, const EditorAPI::BSString& fileName,
				bool overwrite = false) noexcept;
			static bool ExtractResourceByName(void* zip, const EditorAPI::BSString& fileName, 
				const EditorAPI::BSString& fileNewName, bool overwrite = false) noexcept;
		};
	}
}