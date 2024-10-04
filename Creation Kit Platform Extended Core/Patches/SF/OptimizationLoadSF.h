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

				static void sub(int64_t a1);
				static int HKInflateInit(z_stream_s* Stream, const char* Version, int Mode);
				static int HKInflate(z_stream_s* Stream, int Flush);
				static HANDLE HKFindFirstFileA(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData);

				static uint32_t HKSearchIndexOffset32(EditorAPI::BSTArray<uint32_t>& _array,
					uint32_t& _target, uint32_t _start_index);
				inline static uint32_t HKSearchIndex32(EditorAPI::BSTArray<uint32_t>& _array, uint32_t& _target)
				{
					return HKSearchIndexOffset32(_array, _target, 0);
				}
				static uint32_t HKSearchIndexOffset64(EditorAPI::BSTArray<uint64_t>& _array, 
					uint64_t& _target, uint32_t _start_index);
				inline static uint32_t HKSearchIndex64(EditorAPI::BSTArray<uint64_t>& _array, uint64_t& _target)
				{
					return HKSearchIndexOffset64(_array, _target, 0);
				}
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