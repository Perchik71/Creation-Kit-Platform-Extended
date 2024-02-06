// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Core/Module.h"
#include "Core/Relocator.h"
#include "Core/RelocationDatabase.h"
#include "Editor API/SSE/TESForm.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			using namespace CreationKitPlatformExtended::Core;
			using namespace EditorAPI::SkyrimSpectialEdition;

			class FixFormIDsInLoadFilePatch : public Module
			{
			public:
				FixFormIDsInLoadFilePatch();

				virtual bool HasOption() const;
				virtual bool HasCanRuntimeDisabled() const;
				virtual const char* GetOptionName() const;
				virtual const char* GetName() const;
				virtual bool HasDependencies() const;
				virtual Array<String> GetDependencies() const;

				static bool sub(TESForm* World, uint32_t* ParentFormIDs, uint32_t* FormIDs);
				static uint32_t sub2(TESForm* World, uint32_t* FormIDs);
			protected:
				virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
					const char* lpcstrPlatformRuntimeVersion) const;
				virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
				virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			private:
				FixFormIDsInLoadFilePatch(const FixFormIDsInLoadFilePatch&) = default;
				FixFormIDsInLoadFilePatch& operator=(const FixFormIDsInLoadFilePatch&) = default;
			};
		}
	}
}