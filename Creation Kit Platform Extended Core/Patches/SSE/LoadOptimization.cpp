// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "LoadOptimization.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			thread_local WIN32_FIND_DATAA CachedFileInfo;
			thread_local char CachedBasePath[MAX_PATH];
			thread_local bool CachedFileInfoValid;
			float* pProgress1 = 0;
			float* pProgress2 = 0;

			constexpr DWORD INVALID_INDEX = 0xFFFFFFFF;

			LoadOptimizationPatch::LoadOptimizationPatch() : Module(GlobalEnginePtr)
			{}

			bool LoadOptimizationPatch::HasOption() const
			{
				return false;
			}

			bool LoadOptimizationPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* LoadOptimizationPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* LoadOptimizationPatch::GetName() const
			{
				return "Load Optimization";
			}

			bool LoadOptimizationPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> LoadOptimizationPatch::GetDependencies() const
			{
				return {};
			}

			bool LoadOptimizationPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool LoadOptimizationPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Plugin loading optimizations:
					//
					// - TESForm reference map rewrite (above)
					// - Fix an unoptimized function bottleneck (SearchArrayItem) (Large ESP files only)
					// - Fix an unoptimized function bottleneck (sub_1415D5640)
					// - Eliminate millions of calls to update the progress dialog, instead only updating 400 times (0% -> 100%)
					// - Replace old zlib decompression code with optimized libdeflate
					// - Cache results from FindFirstFile when GetFileAttributesExA is called immediately after 
					// (BSSystemDir__NextEntry, BSResource__LooseFileLocation__FileExists)
					//

					// Utilize SSE4.1 instructions if available
					if (GlobalEnginePtr->HasSSE41())
						lpRelocator->DetourJump(lpRelocationDatabaseItem->At(10), (uintptr_t)&SearchArrayItem_SSE41);
					else
						lpRelocator->DetourJump(lpRelocationDatabaseItem->At(10), (uintptr_t)&SearchArrayItem);

					pProgress1 = (float*)lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0));
					pProgress2 = (float*)lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));
					UpdateProgressBar = (void(*)())lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(2));
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(9), (uintptr_t)&sub_141589150);
					lpRelocator->Patch(lpRelocationDatabaseItem->At(3), { 0xB9, 0x90, 0x01, 0x00, 0x00, 0x90 });
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(4), (uintptr_t)&UpdateLoadProgressBar);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(5), (uintptr_t)&HKInflateInit);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(6), (uintptr_t)&HKInflate);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(7), (uintptr_t)&BSSystemDir_NextEntry);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(8), (uintptr_t)&BSResource_LooseFileLocation_FileExists);

					return true;
				}
				else if (lpRelocationDatabaseItem->Version() == 2)
				{
					//
					// Plugin loading optimizations:
					//
					// - TESForm reference map rewrite (above)
					// - Fix an unoptimized function bottleneck (SearchArrayItem) (Large ESP files only)
					// - Fix an unoptimized function bottleneck (sub_1415D5640)
					// - Eliminate millions of calls to update the progress dialog, instead only updating 400 times (0% -> 100%)
					// - Replace old zlib decompression code with optimized libdeflate
					// - Cache results from FindFirstFile when GetFileAttributesExA is called immediately after 
					// (BSSystemDir__NextEntry, BSResource__LooseFileLocation__FileExistsEx)
					//

					// Utilize SSE4.1 instructions if available
					if (GlobalEnginePtr->HasSSE41())
						lpRelocator->DetourJump(lpRelocationDatabaseItem->At(10), (uintptr_t)&SearchArrayItem_SSE41);
					else
						lpRelocator->DetourJump(lpRelocationDatabaseItem->At(10), (uintptr_t)&SearchArrayItem);

					pProgress1 = (float*)lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0));
					pProgress2 = (float*)lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));
					UpdateProgressBar = (void(*)())lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(2));
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(9), (uintptr_t)&sub_141589150);
					lpRelocator->Patch(lpRelocationDatabaseItem->At(3), { 0xB9, 0x90, 0x01, 0x00, 0x00, 0x90 });
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(4), (uintptr_t)&UpdateLoadProgressBar);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(5), (uintptr_t)&HKInflateInit);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(6), (uintptr_t)&HKInflate);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(7), (uintptr_t)&BSSystemDir_NextEntry);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(8), (uintptr_t)&BSResource_LooseFileLocation_FileExistsEx);

					return true;
				}

				return false;
			}

			bool LoadOptimizationPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			bool LoadOptimizationPatch::sub_141589150(__int64 a1, uint32_t* a2)
			{
				while (a1 && (*(uint32_t*)a1 != *a2))
					a1 = *(__int64*)(a1 + 8);

				return a1 != 0;
			}

			int LoadOptimizationPatch::HKInflateInit(z_stream_s* Stream, const char* Version, int Mode)
			{
				// Force inflateEnd to error out and skip frees
				Stream->state = nullptr;

				return 0;
			}

			int LoadOptimizationPatch::HKInflate(z_stream_s* Stream, int Flush)
			{
				size_t outBytes = 0;
				libdeflate_decompressor* decompressor = libdeflate_alloc_decompressor();

				libdeflate_result result = libdeflate_zlib_decompress(decompressor, Stream->next_in, 
					Stream->avail_in, Stream->next_out, Stream->avail_out, &outBytes);
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

			void LoadOptimizationPatch::UpdateLoadProgressBar()
			{
				static float lastPercent = 0.0f;

				// Only update every quarter percent, rather than every single form load
				float newPercent = ((*pProgress2) / (*pProgress1)) * 100.0f;
				if (abs(lastPercent - newPercent) <= 0.25f)
					return;

				UpdateProgressBar();
				lastPercent = newPercent;
			}

			uint32_t LoadOptimizationPatch::BSSystemDir_NextEntry(__int64 a1, bool* IsComplete)
			{
				auto& findHandle = *(HANDLE*)a1;
				auto findData = (LPWIN32_FIND_DATAA)(a1 + 8);
				auto& status = *(uint32_t*)(a1 + 0x24C);

				CachedFileInfoValid = true;

				if (findHandle == INVALID_HANDLE_VALUE)
				{
					// Attempting to iterate directory on an already invalid handle
					status = 6;
				}
				else if (findHandle)
				{
					*IsComplete = FindNextFileA(findHandle, findData) == FALSE;
					memcpy(&CachedFileInfo, findData, sizeof(WIN32_FIND_DATAA));

					if (*IsComplete)
						CachedFileInfoValid = false;
				}
				else
				{
					findHandle = FindFirstFileExA((LPCSTR)(a1 + 0x148), FindExInfoStandard, findData, 
						FindExSearchNameMatch, nullptr, FIND_FIRST_EX_LARGE_FETCH);
					memcpy(&CachedFileInfo, findData, sizeof(WIN32_FIND_DATAA));

					strcpy_s(CachedBasePath, (LPCSTR)(a1 + 0x148));
					*strrchr(CachedBasePath, '\\') = '\0';

					// status = x ? EC_INVALID_PARAM : EC_NONE;
					status = (findHandle == INVALID_HANDLE_VALUE) ? 6 : 0;
				}

				if (status != 0)
					CachedFileInfoValid = false;

				return status;
			}

			bool LoadOptimizationPatch::BSResource_LooseFileLocation_FileExists(const char* CanonicalFullPath, 
				uint32_t* TotalSize)
			{
				WIN32_FILE_ATTRIBUTE_DATA fileInfo
				{
					.dwFileAttributes = INVALID_FILE_ATTRIBUTES
				};

				if (CachedFileInfoValid)
				{
					const static uint32_t cwdLength = GetCurrentDirectoryA(0, nullptr);
					const size_t filePathLen = strlen(CanonicalFullPath);
					const size_t basePathLen = strlen(CachedBasePath);

					if (filePathLen > cwdLength)
					{
						// Anything under "<CWD>\\Data\\data\\" will never be valid, so discard all calls with it
						if (!_strnicmp(CanonicalFullPath + cwdLength, "Data\\data\\", 10))
							return false;

						// Check if this file path matches the query from FindFirstFileExA
						if (filePathLen > basePathLen && !_stricmp(CanonicalFullPath + basePathLen, CachedFileInfo.cFileName))
						{
							fileInfo.dwFileAttributes = CachedFileInfo.dwFileAttributes;
							fileInfo.nFileSizeLow = CachedFileInfo.nFileSizeLow;
							fileInfo.nFileSizeHigh = CachedFileInfo.nFileSizeHigh;
						}
					}
				}

				if (fileInfo.dwFileAttributes == INVALID_FILE_ATTRIBUTES)
				{
					// Cache miss
					if (!GetFileAttributesExA(CanonicalFullPath, GetFileExInfoStandard, &fileInfo))
						return false;
				}

				if (fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					return false;

				// The game doesn't support files larger than 4GB
				if (fileInfo.nFileSizeHigh > 0)
				{
					_CONSOLE("Need to move to 64-bit file sizes. '%s' exceeds 4GB.", CanonicalFullPath);
					return false;
				}

				*TotalSize = fileInfo.nFileSizeLow;
				return true;
			}

			bool LoadOptimizationPatch::BSResource_LooseFileLocation_FileExistsEx(const char* CanonicalFullPath, 
				uint64_t* TotalSize)
			{
				WIN32_FILE_ATTRIBUTE_DATA fileInfo
				{
					.dwFileAttributes = INVALID_FILE_ATTRIBUTES
				};

				if (CachedFileInfoValid)
				{
					const static uint32_t cwdLength = GetCurrentDirectoryA(0, nullptr);
					const size_t filePathLen = strlen(CanonicalFullPath);
					const size_t basePathLen = strlen(CachedBasePath);

					if (filePathLen > cwdLength)
					{
						// Anything under "<CWD>\\Data\\data\\" will never be valid, so discard all calls with it
						if (!_strnicmp(CanonicalFullPath + cwdLength, "Data\\data\\", 10))
							return false;

						// Check if this file path matches the query from FindFirstFileExA
						if (filePathLen > basePathLen && !_stricmp(CanonicalFullPath + basePathLen, CachedFileInfo.cFileName))
						{
							fileInfo.dwFileAttributes = CachedFileInfo.dwFileAttributes;
							fileInfo.nFileSizeLow = CachedFileInfo.nFileSizeLow;
							fileInfo.nFileSizeHigh = CachedFileInfo.nFileSizeHigh;
						}
					}
				}

				if (fileInfo.dwFileAttributes == INVALID_FILE_ATTRIBUTES)
				{
					// Cache miss
					if (!GetFileAttributesExA(CanonicalFullPath, GetFileExInfoStandard, &fileInfo))
						return false;
				}

				if (fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					return false;

				*TotalSize = ((uint64_t)fileInfo.nFileSizeHigh << 32) | fileInfo.nFileSizeLow;
				return true;
			}

			DWORD LoadOptimizationPatch::SearchArrayItem_SSE41(EditorAPI::BSTArray<void*>& _array,
				void* &_target, DWORD _start_index, __int64 Unused)
			{
				uint32_t index = _start_index;
				__int64* data = (__int64*)_array.QBuffer();

				const uint32_t comparesPerIter = 4;
				const uint32_t vectorizedIterations = (_array.QSize() - index) / comparesPerIter;

				//
				// Compare 4 pointers per iteration - use SIMD instructions to generate a bit mask. Set
				// bit 0 if 'array[i + 0]'=='target', set bit 1 if 'array[i + 1]'=='target', set bit X...
				//
				// AVX: mask = _mm256_movemask_pd(_mm256_castsi256_pd(_mm256_cmpeq_epi64(targets, _mm256_loadu_si256((__m256i *)&data[i]))));
				//
				const __m128i targets = _mm_set1_epi64x((__int64)_target);

				for (uint32_t iter = 0; iter < vectorizedIterations; iter++)
				{
					__m128i test1 = _mm_cmpeq_epi64(targets, _mm_loadu_si128((__m128i*)&data[index + 0]));
					__m128i test2 = _mm_cmpeq_epi64(targets, _mm_loadu_si128((__m128i*)&data[index + 2]));

					int mask = _mm_movemask_pd(_mm_castsi128_pd(_mm_or_si128(test1, test2)));

					// if (target pointer found) { break into the remainder loop to get the index }
					if (mask != 0)
						break;

					index += comparesPerIter;
				}

				// Scan the rest 1-by-1
				for (; index < _array.QSize(); index++)
				{
					if (data[index] == (__int64)_target)
						return index;
				}

				return 0xFFFFFFFF;
			}

			DWORD LoadOptimizationPatch::SearchArrayItem(EditorAPI::BSTArray<void*> &_array,
				void* &_target, DWORD _start_index, __int64 Unused)
			{
				for (uint32_t i = _start_index; i < _array.QSize(); i++)
				{
					if (_array[i] == _target)
						return i;
				}

				return 0xFFFFFFFF;
			}
		}
	}
}