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
			using namespace CreationKitPlatformExtended::EditorAPI::SkyrimSpectialEdition;

			class CrashInventoryIteratorsPatch : public Module
			{
			public:
				CrashInventoryIteratorsPatch();

				virtual bool HasOption() const;
				virtual bool HasCanRuntimeDisabled() const;
				virtual const char* GetOptionName() const;
				virtual const char* GetName() const;

				static int sub1(__int64 a1, __int64 a2);
				static int sub2(__int64 a1, __int64 a2);
				static int sub1_Extremly(__int64 a1, __int64 a2);
				static int sub2_Extremly(__int64 a1, __int64 a2);

				static Array<TESObjectREFR_Original*> CreateCellPersistentMapCopy(__int64 List);
				static Array<TESObjectREFR_Extremly*> CreateCellPersistentMapCopy_Extremly(__int64 List);
			protected:
				virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
					const char* lpcstrPlatformRuntimeVersion) const;
				virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
				virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			private:
				CrashInventoryIteratorsPatch(const CrashInventoryIteratorsPatch&) = default;
				CrashInventoryIteratorsPatch& operator=(const CrashInventoryIteratorsPatch&) = default;
			};
		}
	}
}