// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Core/Module.h"
#include "Core/Relocator.h"
#include "Core/RelocationDatabase.h"
#include "Editor API/BSTArray.h"

#include <libdeflate.h>

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Starfield
		{
			using namespace CreationKitPlatformExtended::Core;

			class OptimizationLoadPatch : public Module
			{
			public:
				struct z_stream_s
				{
					const void* next_in;
					uint32_t avail_in;
					uint32_t total_in;
					void* next_out;
					uint32_t avail_out;
					uint32_t total_out;
					const char* msg;
					struct internal_state* state;
				};

				OptimizationLoadPatch();

				virtual bool HasOption() const;
				virtual bool HasCanRuntimeDisabled() const;
				virtual const char* GetOptionName() const;
				virtual const char* GetName() const;
				virtual bool HasDependencies() const;
				virtual Array<String> GetDependencies() const;

				static int HKInflateInit(z_stream_s* Stream, const char* Version, int Mode);
				static int HKInflate(z_stream_s* Stream, int Flush);
				static HANDLE HKFindFirstFileA(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData);
			protected:
				virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
					const char* lpcstrPlatformRuntimeVersion) const;
				virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
				virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			private:
				OptimizationLoadPatch(const OptimizationLoadPatch&) = default;
				OptimizationLoadPatch& operator=(const OptimizationLoadPatch&) = default;
			};
		}
	}
}