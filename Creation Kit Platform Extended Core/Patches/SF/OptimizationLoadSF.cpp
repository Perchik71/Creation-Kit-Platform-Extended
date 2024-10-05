// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "OptimizationLoadSF.h"
#include "Editor API/SF/BSFile.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Starfield
		{
			constexpr uint32_t INVALID_INDEX = 0xFFFFFFFF;

			uintptr_t pointer_OptimizationLoad_sub1 = 0;

			OptimizationLoadPatch::OptimizationLoadPatch() : Module(GlobalEnginePtr)
			{}

			bool OptimizationLoadPatch::HasOption() const
			{
				return false;
			}

			bool OptimizationLoadPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* OptimizationLoadPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* OptimizationLoadPatch::GetName() const
			{
				return "Optimization load";
			}

			bool OptimizationLoadPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> OptimizationLoadPatch::GetDependencies() const
			{
				return {};
			}

			bool OptimizationLoadPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return (eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_1_14_70_0) &&
					(eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_LAST);
			}

			bool OptimizationLoadPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				auto verPatch = lpRelocationDatabaseItem->Version();

				if (verPatch == 1)
				{
					//
					// Plugin loading optimizations:
					//
					// - Replacing FindFirstNextA with a more optimized function FindFirstFileExA
					// - Replace zlib decompression code with optimized libdeflate
					// - Skip remove failed forms (This function is more likely to result in CTD) 
					// - Eliminate millions of calls to update the statusbar
					// - Increasing the read memory buffer to reduce disk access

					PatchIAT(HKFindFirstFileA, "kernel32.dll", "FindFirstFileA");

					lpRelocator->DetourCall(_RELDATA_RAV(0), (uintptr_t)&HKInflateInit);
					lpRelocator->DetourCall(_RELDATA_RAV(1), (uintptr_t)&HKInflate);

					{
						ScopeRelocator text;

						// If there is an ZLib error, will try to leave the function 
						lpRelocator->Patch(_RELDATA_RAV(2), { 0xE9, 0x41, 0x01, 0x00, 0x00, 0x90 });
						// Skip remove failed forms
						lpRelocator->Patch(_RELDATA_RAV(3), { 0xEB });
						// Spam in the status bar
						lpRelocator->Patch(_RELDATA_RAV(4), { 0xC3, 0x90 });
						// Spam Primitive with zero extents encountered for reference %08X
						lpRelocator->Patch(_RELDATA_RAV(6), { 0xEB });
					}

					// 2 kb -> x kb >= 256 kb
					*(uintptr_t*)&EditorAPI::Starfield::BSFile::ICreateInstance =
						voltek::detours_function_class_jump(_RELDATA_ADDR(5), (uintptr_t)&EditorAPI::Starfield::BSFile::HKCreateInstance);

					return true;
				}

				return false;
			}

			bool OptimizationLoadPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			int OptimizationLoadPatch::HKInflateInit(z_stream_s* Stream, const char* Version, int Mode)
			{
				// Force inflateEnd to error out and skip frees
				Stream->state = nullptr;

				return 0;
			}

			int OptimizationLoadPatch::HKInflate(z_stream_s* Stream, int Flush)
			{
				size_t outBytes = 0;
				libdeflate_decompressor* decompressor = libdeflate_alloc_decompressor();

				libdeflate_result result = libdeflate_zlib_decompress(decompressor, Stream->next_in, Stream->avail_in, Stream->next_out, Stream->avail_out, &outBytes);
				libdeflate_free_decompressor(decompressor);

				if (result == LIBDEFLATE_SUCCESS)
				{
					Assert(outBytes < std::numeric_limits<uint32_t>::max());

					Stream->total_in = Stream->avail_in;
					Stream->total_out = (uint32_t)outBytes;

					return 1;
				}

				if (result == LIBDEFLATE_INSUFFICIENT_SPACE)
					return -5;

				return -2;
			}

			HANDLE OptimizationLoadPatch::HKFindFirstFileA(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData)
			{
				return FindFirstFileExA(lpFileName, FindExInfoStandard, lpFindFileData, FindExSearchNameMatch, 
					NULL, FIND_FIRST_EX_LARGE_FETCH);
			}
		}
	}
}