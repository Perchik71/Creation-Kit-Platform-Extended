// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Core/Module.h"
#include "Core/Relocator.h"
#include "Core/RelocationDatabase.h"

#include "NiAPI/NiPoint.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			using namespace CreationKitPlatformExtended::Core;

			class CrashMHDTMoreThan70Patch : public Module
			{
			public:
				CrashMHDTMoreThan70Patch();

				virtual bool HasOption() const;
				virtual bool HasCanRuntimeDisabled() const;
				virtual const char* GetOptionName() const;
				virtual const char* GetName() const;
				virtual bool HasDependencies() const;
				virtual Array<String> GetDependencies() const;

				static void* sub1(void* world, int x, int y);
				static void sub2(void* shape, NiAPI::NiPoint3* vec, NiAPI::NiPoint2* point, void* unk1, int unk2);
			protected:
				virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
					const char* lpcstrPlatformRuntimeVersion) const;
				virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
				virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			private:
				CrashMHDTMoreThan70Patch(const CrashMHDTMoreThan70Patch&) = default;
				CrashMHDTMoreThan70Patch& operator=(const CrashMHDTMoreThan70Patch&) = default;
			};
		}
	}
}