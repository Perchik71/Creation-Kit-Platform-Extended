// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <libdeflate.h>
#include <intrin.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.HardwareInfo.h>
#include <CKPE.Asserts.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/BSTArray.h>
#include <Patches/CKPE.SkyrimSE.Patch.LoadOptimization.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			struct z_stream_s
			{
				const void* next_in;
				std::uint32_t avail_in;
				std::uint32_t total_in;
				void* next_out;
				std::uint32_t avail_out;
				std::uint32_t total_out;
				const char* msg;
				struct internal_state* state;
			};

			thread_local WIN32_FIND_DATAA CachedFileInfo;
			thread_local char CachedBasePath[MAX_PATH];
			thread_local bool CachedFileInfoValid;
			float* pProgress1 = 0;
			float* pProgress2 = 0;

			static bool sub_141589150(std::int64_t a1, uint32_t* a2) noexcept(true)
			{
				while (a1 && (*(std::uint32_t*)a1 != *a2))
					a1 = *(std::int64_t*)(a1 + 8);
				return a1 != 0;
			}

			static std::int32_t HKInflateInit(z_stream_s* Stream, const char* Version, std::int32_t Mode) noexcept(true)
			{
				// Force inflateEnd to error out and skip frees
				Stream->state = nullptr;
				return 0;
			}

			static std::int32_t HKInflate(z_stream_s* Stream, std::int32_t Flush) noexcept(true)
			{
				std::size_t outBytes = 0;
				libdeflate_decompressor* decompressor = libdeflate_alloc_decompressor();

				libdeflate_result result = libdeflate_zlib_decompress(decompressor, Stream->next_in,
					Stream->avail_in, Stream->next_out, Stream->avail_out, &outBytes);
				libdeflate_free_decompressor(decompressor);

				if (result == LIBDEFLATE_SUCCESS)
				{
					CKPE_ASSERT(outBytes < std::numeric_limits<std::uint32_t>::max());

					Stream->total_in = Stream->avail_in;
					Stream->total_out = (std::uint32_t)outBytes;

					return 1;
				}

				if (result == LIBDEFLATE_INSUFFICIENT_SPACE)
					return -5;

				return -2;
			}

			inline static void (*UpdateProgressBar)(void);
			static void UpdateLoadProgressBar() noexcept(true)
			{
				static float lastPercent = 0.0f;

				// Only update every quarter percent, rather than every single form load
				float newPercent = ((*pProgress2) / (*pProgress1)) * 100.0f;
				if (abs(lastPercent - newPercent) <= 0.25f)
					return;

				//GlobalProgressWindowPtr->step();
				UpdateProgressBar();
				lastPercent = newPercent;
			}

			static std::uint32_t BSSystemDir_NextEntry(std::int64_t a1, bool* IsComplete)
			{
				auto& findHandle = *(HANDLE*)a1;
				auto findData = (LPWIN32_FIND_DATAA)(a1 + 8);
				auto& status = *(std::uint32_t*)(a1 + 0x24C);

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

			static bool BSResource_LooseFileLocation_FileExists(const char* CanonicalFullPath, std::uint32_t* TotalSize)
			{
				WIN32_FILE_ATTRIBUTE_DATA fileInfo
				{
					.dwFileAttributes = INVALID_FILE_ATTRIBUTES
				};

				if (CachedFileInfoValid)
				{
					const static std::uint32_t cwdLength = GetCurrentDirectoryA(0, nullptr);
					const std::size_t filePathLen = strlen(CanonicalFullPath);
					const std::size_t basePathLen = strlen(CachedBasePath);

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

			static bool BSResource_LooseFileLocation_FileExistsEx(const char* CanonicalFullPath, std::uint64_t* TotalSize)
			{
				WIN32_FILE_ATTRIBUTE_DATA fileInfo
				{
					.dwFileAttributes = INVALID_FILE_ATTRIBUTES
				};

				if (CachedFileInfoValid)
				{
					const static std::uint32_t cwdLength = GetCurrentDirectoryA(0, nullptr);
					const std::size_t filePathLen = strlen(CanonicalFullPath);
					const std::size_t basePathLen = strlen(CachedBasePath);

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

			static DWORD SearchArrayItem_SSE41(EditorAPI::BSTArray<void*>& _array, void*& _target,
				DWORD _start_index, std::int64_t Unused)
			{
				std::uint32_t index = _start_index;
				std::int64_t* data = (std::int64_t*)_array.QBuffer();

				const std::uint32_t comparesPerIter = 4;
				const std::uint32_t vectorizedIterations = (_array.QSize() - index) / comparesPerIter;

				//
				// Compare 4 pointers per iteration - use SIMD instructions to generate a bit mask. Set
				// bit 0 if 'array[i + 0]'=='target', set bit 1 if 'array[i + 1]'=='target', set bit X...
				//
				// AVX: mask = _mm256_movemask_pd(_mm256_castsi256_pd(_mm256_cmpeq_epi64(targets, _mm256_loadu_si256((__m256i *)&data[i]))));
				//
				const __m128i targets = _mm_set1_epi64x((std::int64_t)_target);

				for (std::uint32_t iter = 0; iter < vectorizedIterations; iter++)
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
					if (data[index] == (std::int64_t)_target)
						return index;
				}

				return 0xFFFFFFFF;
			}

			static DWORD SearchArrayItem(EditorAPI::BSTArray<void*>& _array, void*& _target,
				DWORD _start_index, std::int64_t Unused)
			{
				for (std::uint32_t i = _start_index; i < _array.QSize(); i++)
				{
					if (_array[i] == _target)
						return i;
				}

				return 0xFFFFFFFF;
			}

			

			LoadOptimization::LoadOptimization() : Common::Patch()
			{
				SetName("Load Optimization");
			}

			bool LoadOptimization::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* LoadOptimization::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool LoadOptimization::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> LoadOptimization::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool LoadOptimization::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool LoadOptimization::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto verPatch = db->GetVersion();

				if ((verPatch != 1) && (verPatch != 2))
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Plugin loading optimizations:
				//
				// - Fix an unoptimized function bottleneck (SearchArrayItem) (Large ESP files only)
				// - Fix an unoptimized function bottleneck (sub_1415D5640)
				// - Eliminate millions of calls to update the progress dialog, instead only updating 400 times (0% -> 100%)
				// - Replace old zlib decompression code with optimized libdeflate
				// - Cache results from FindFirstFile when GetFileAttributesExA is called immediately after 
				// (BSSystemDir__NextEntry, BSResource__LooseFileLocation__FileExists)
				//

				// Utilize SSE4.1 instructions if available
				if (HardwareInfo::CPU::HasSupportSSE41())
					Detours::DetourJump(__CKPE_OFFSET(10), (std::uintptr_t)&SearchArrayItem_SSE41);
				else
					Detours::DetourJump(__CKPE_OFFSET(10), (std::uintptr_t)&SearchArrayItem);

				pProgress1 = (float*)__CKPE_OFFSET(0);
				pProgress2 = (float*)__CKPE_OFFSET(1);
				UpdateProgressBar = (void(*)())__CKPE_OFFSET(2);
				Detours::DetourJump(__CKPE_OFFSET(9), (std::uintptr_t)&sub_141589150);
				SafeWrite::Write(__CKPE_OFFSET(3), { 0xB9, 0x90, 0x01, 0x00, 0x00, 0x90 });
				Detours::DetourCall(__CKPE_OFFSET(4), (std::uintptr_t)&UpdateLoadProgressBar);
				Detours::DetourCall(__CKPE_OFFSET(5), (std::uintptr_t)&HKInflateInit);
				Detours::DetourCall(__CKPE_OFFSET(6), (std::uintptr_t)&HKInflate);
				Detours::DetourJump(__CKPE_OFFSET(7), (std::uintptr_t)&BSSystemDir_NextEntry);
				
				if (verPatch == 1)
					Detours::DetourJump(__CKPE_OFFSET(8), (std::uintptr_t)&BSResource_LooseFileLocation_FileExists);
				else
					Detours::DetourJump(__CKPE_OFFSET(8), (std::uintptr_t)&BSResource_LooseFileLocation_FileExistsEx);

				return true;
			}
		}
	}
}