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
		namespace SkyrimSpectialEdition
		{
			using namespace CreationKitPlatformExtended::Core;

			class LoadOptimizationPatch : public Module
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

				LoadOptimizationPatch();

				virtual bool HasOption() const;
				virtual bool HasCanRuntimeDisabled() const;
				virtual const char* GetOptionName() const;
				virtual const char* GetName() const;

				static bool sub_141589150(__int64 a1, uint32_t* a2);
				static int HKInflateInit(z_stream_s* Stream, const char* Version, int Mode);
				static int HKInflate(z_stream_s* Stream, int Flush);
				static void UpdateLoadProgressBar();
				static uint32_t BSSystemDir_NextEntry(__int64 a1, bool* IsComplete);
				static bool BSResource_LooseFileLocation_FileExists(const char* CanonicalFullPath, uint32_t* TotalSize);
				static bool BSResource_LooseFileLocation_FileExistsEx(const char* CanonicalFullPath, uint64_t* TotalSize);
				static DWORD SearchArrayItem_SSE41(EditorAPI::BSTArray<void*>& _array, void* &_target,
					DWORD _start_index, __int64 Unused);
				static DWORD SearchArrayItem(EditorAPI::BSTArray<void*>& _array, void* &_target,
					DWORD _start_index, __int64 Unused);

				inline static void (*UpdateProgressBar)(void);
			protected:
				virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
					const char* lpcstrPlatformRuntimeVersion) const;
				virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
				virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			private:
				LoadOptimizationPatch(const LoadOptimizationPatch&) = default;
				LoadOptimizationPatch& operator=(const LoadOptimizationPatch&) = default;
			};
		}
	}
}