// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Core/Module.h"
#include "Core/Relocator.h"
#include "Core/RelocationDatabase.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			using namespace CreationKitPlatformExtended::Core;

			class AddChangeRefPatch : public Module
			{
			public:
				AddChangeRefPatch();

				virtual bool HasOption() const;
				virtual bool HasCanRuntimeDisabled() const;
				virtual const char* GetOptionName() const;
				virtual const char* GetName() const;
				virtual bool HasDependencies() const;
				virtual Array<String> GetDependencies() const;
				
				static bool HKInsertMenuA(HMENU hMenu, uint32_t uPosition, uint32_t uFlags, 
					uintptr_t uIDNewItem, const char* lpNewItem);
				static bool HKDeleteMenu(HMENU hMenu, UINT uPosition, UINT uFlags);
			protected:
				virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
					const char* lpcstrPlatformRuntimeVersion) const;
				virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
				virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			private:
				AddChangeRefPatch(const AddChangeRefPatch&) = default;
				AddChangeRefPatch& operator=(const AddChangeRefPatch&) = default;
			};
		}
	}
}