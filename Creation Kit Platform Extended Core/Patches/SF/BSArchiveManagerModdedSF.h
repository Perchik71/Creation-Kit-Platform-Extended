// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Core/Module.h"
#include "Core/Relocator.h"
#include "Core/RelocationDatabase.h"

#include "Editor API/SF/TESFileSF.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Starfield
		{
			using namespace CreationKitPlatformExtended::Core;
			using namespace CreationKitPlatformExtended::EditorAPI::Starfield;

			class BSArchiveManagerModdedPatch : public Module
			{
			public:
				BSArchiveManagerModdedPatch();

				virtual bool HasOption() const;
				virtual bool HasCanRuntimeDisabled() const;
				virtual const char* GetOptionName() const;
				virtual const char* GetName() const;
				virtual bool HasDependencies() const;
				virtual Array<String> GetDependencies() const;

				static void AttachBA2File(LPCSTR _filename);
				static void LoadTesFile(const TESFile* load_file, __int8 unknown);
				static void LoadTesFileFinal();
				static bool HasLoaded();
			protected:
				virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
					const char* lpcstrPlatformRuntimeVersion) const;
				virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
				virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			private:
				BSArchiveManagerModdedPatch(const BSArchiveManagerModdedPatch&) = default;
				BSArchiveManagerModdedPatch& operator=(const BSArchiveManagerModdedPatch&) = default;
			};
		}
	}
}