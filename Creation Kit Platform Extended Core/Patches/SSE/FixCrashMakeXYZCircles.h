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

			class FixCrashMakeXYZCirclesPatch : public Module
			{
			public:
				FixCrashMakeXYZCirclesPatch();

				virtual bool HasOption() const;
				virtual bool HasCanRuntimeDisabled() const;
				virtual const char* GetOptionName() const;
				virtual const char* GetName() const;

				static void* sub_unknown(__int64 ResourceManager, uint32_t IndexCount, __int64 a3, __int64 a4,
					__int64 a5, __int64 a6, __int64 a7);
			protected:
				virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
					const char* lpcstrPlatformRuntimeVersion) const;
				virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
				virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			private:
				FixCrashMakeXYZCirclesPatch(const FixCrashMakeXYZCirclesPatch&) = default;
				FixCrashMakeXYZCirclesPatch& operator=(const FixCrashMakeXYZCirclesPatch&) = default;
			};
		}
	}
}