// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "OptimizationLoadF4.h"
#include "Editor API/FO4/BSFile.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			extern uintptr_t pointer_BSFile_sub;
		}
	}

	namespace Patches
	{
		namespace Fallout4
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
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool OptimizationLoadPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				auto verPatch = lpRelocationDatabaseItem->Version();

				if ((verPatch == 1) || (verPatch == 2))
				{
					//
					// Plugin loading optimizations:
					//
					// - Eliminate millions of calls to update the statusbar, instead only updating to 250ms 
					// - Replace old zlib decompression code with optimized libdeflate
					// - Skip remove failed forms (This function is more likely to result in CTD)
					// - Increasing the read memory buffer to reduce disk access
					// - (Optional) Removing animation export when loading the mod, it will cause CTD if animation is needed
					// - Replacing all unoptimized functions related to searching in index arrays, and maybe not only	
					// - Replacing FindFirstNextA with a more optimized function FindFirstFileExA

					// Spam in the status bar no more than 250ms
					lpRelocator->DetourCall(_RELDATA_RAV(0), (uintptr_t)&sub);
					pointer_OptimizationLoad_sub1 = _RELDATA_ADDR(1);

					lpRelocator->DetourCall(_RELDATA_RAV(2), (uintptr_t)&HKInflateInit);
					lpRelocator->DetourCall(_RELDATA_RAV(3), (uintptr_t)&HKInflate);

					PatchIAT(HKFindFirstFileA, "kernel32.dll", "FindFirstFileA");

					// Skip remove failed forms
					lpRelocator->Patch(_RELDATA_RAV(4), { 0xEB });

					{
						ScopeRelocator text;

						auto rva = _RELDATA_RAV(12);
						EditorAPI::Fallout4::pointer_BSFile_sub = lpRelocator->Rav2Off(rva);
						// 2 kb -> x kb >= 256 kb
						*(uintptr_t*)&EditorAPI::Fallout4::BSFile::ICreateInstance =
							voltek::detours_function_class_jump(EditorAPI::Fallout4::pointer_BSFile_sub,
								(uintptr_t)&EditorAPI::Fallout4::BSFile::HKCreateInstance);
						// 2 kb -> 256 kb
						if (verPatch == 1)
						{
							lpRelocator->Patch(_RELDATA_RAV(13), { 0x41, 0xB9, 0x00, 0x00, 0x04, 0x00, 0x90 });
							lpRelocator->PatchNop(_RELDATA_RAV(15), 4);
						}
						else
							lpRelocator->Patch(_RELDATA_RAV(13), { 0x41, 0x89, 0xC6, 0x90 });
						
						lpRelocator->Patch(_RELDATA_RAV(14), { 0x04 });

						if (_READ_OPTION_BOOL("Animation", "bSkipAnimationBuildData", false))
						{
							// Skipping Export Anim
							lpRelocator->Patch(_RELDATA_RAV(16), { 0xC3 });
							// Remove temporary files
							DeleteFileA("TemporaryBehaviorEventInfoOutput.txt");
							DeleteFileA("TemporaryClipDataOutput.txt");
							DeleteFileA("TemporarySyncAnimDataOutput.txt");
						}
					}

					// Without SIMD support, there is no pointand
					if (_engine->HasSSE41())
					{
						if (verPatch == 1)
						{
							std::size_t count = 0;

								auto Sec = _engine->GetSection(Core::SECTION_TEXT);

								std::vector<uintptr_t>::iterator match;
								std::vector<uintptr_t> matches = voltek::find_patterns(Sec.base, Sec.end - Sec.base,
									"48 83 C5 08 41 3B FE 72 9F 48 8B 6C 24 50 8B C3 48 8B 5C 24 58 48 8B 74"
									" 24 60 48 83 C4 30 41 5F 41 5E 5F C3 8B C3 EB E8");

								std::for_each(match = matches.begin(), matches.end(), [&count](auto it)
									{ voltek::detours_jump(it - 0x8A, (uintptr_t)&HKSearchIndex64); count++; });

							matches = voltek::find_patterns(Sec.base, Sec.end - Sec.base,
								"48 83 C5 08 41 3B FE 72 9F 48 8B 6C 24 58 48 8B 74 24 60 8B C3 48 8B 5C"
								" 24 50 48 83 C4 30 41 5F 41 5E 5F C3");

							std::for_each(match = matches.begin(), matches.end(), [&count](auto it)
								{ voltek::detours_jump(it - 0x8D, (uintptr_t)&HKSearchIndexOffset64); count++; });

							lpRelocator->DetourJump(lpRelocationDatabaseItem->At(5), (uintptr_t)&HKSearchIndex32);
							lpRelocator->DetourJump(lpRelocationDatabaseItem->At(6), (uintptr_t)&HKSearchIndex32);
							lpRelocator->DetourJump(lpRelocationDatabaseItem->At(7), (uintptr_t)&HKSearchIndex32);

							class Search_IA128 : public Xbyak::CodeGenerator {
							public:
								Search_IA128(VOID) : Xbyak::CodeGenerator() {
									mov(ptr[rsp + 0x10], rbx);
									mov(ptr[rsp + 0x18], rsi);
									push(rdi);
									push(r14);
									push(r15);
									sub(rsp, 0x30);
									mov(r14d, dword[rcx + 0x10]);
									or_(ebx, 0xFFFFFFFF);
									xor_(edi, edi);
									mov(r15, rdx);
									mov(rsi, rcx);
									test(r14d, r14d);
									je(".quit_no");
									push(rbp);
									mov(ebp, edi);
									mov(rcx, qword[r15]);
									mov(rdx, qword[rsi]);
									L(".c1");
									cmp(ebx, -1);
									jne(".quit_yes");
									cmp(qword[rdx + rbp], rcx);
									cmove(ebx, edi);
									inc(edi);
									lea(rbp, qword[rbp + 0x10]);
									cmp(edi, r14d);
									jb(".c1");
									L(".quit_yes");
									pop(rbp);
									L(".quit_no");
									mov(eax, ebx);
									mov(rbx, qword[rsp + 0x58]);
									mov(rsi, qword[rsp + 0x60]);
									add(rsp, 0x30);
									pop(r15);
									pop(r14);
									pop(rdi);
									ret();
								}

								static VOID Generate(uintptr_t Target) {
									auto hook = new Search_IA128();
									GlobalRelocatorPtr->DetourJump(Target, (uintptr_t)hook->getCode());
								}

							};

							Search_IA128::Generate(_RELDATA_RAV(8));
							Search_IA128::Generate(_RELDATA_RAV(9));
							Search_IA128::Generate(_RELDATA_RAV(10));
							Search_IA128::Generate(_RELDATA_RAV(11));

							count += 3;

							_MESSAGE("Replaced function with SIMD function: %d.", count);
						}
					}

					return true;
				}

				return false;
			}

			bool OptimizationLoadPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void OptimizationLoadPatch::sub(int64_t a1)
			{
				static uint64_t timeStartTick = 0;
				static uint64_t timeCurrentTick = GetTickCount64();

				// 250ms
				if ((timeCurrentTick - timeStartTick) >= 250)
				{
					fastCall<void>(pointer_OptimizationLoad_sub1, a1);

					timeStartTick = GetTickCount64();
					timeCurrentTick = timeStartTick;
				}
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

			uint32_t OptimizationLoadPatch::HKSearchIndexOffset32(EditorAPI::BSTArray<uint32_t>& _array,
				uint32_t& _target, uint32_t _start_index)
			{
				uint32_t index = _start_index;
				const uint32_t size_array = _array.QSize();

				if (size_array <= _start_index)
					return INVALID_INDEX;

				uint32_t* data = (uint32_t*)_array.QBuffer() + index;

				if ((size_array - _start_index) < 50) 
				{
					for (; index < size_array; index++) 
					{
						if (data[index] == _target)
							return index;
					}

					return INVALID_INDEX;
				}

				uint32_t res = INVALID_INDEX;

				//
				// Compare 16 indexes per iteration - use SIMD instructions to generate a bit mask. Set
				// bit 0 if 'array[i + 0]'=='target', set bit 1 if 'array[i + 1]'=='target', set bit X...
				const uint32_t comparesPerIter = 16;
				const uint32_t vectorizedIterations = (size_array - index) / comparesPerIter;

				uint32_t bsr_v;
				uint32_t mask;
				std::size_t iter = 0;
				uint32_t* array_p = (uint32_t*)(data + index);

				__m128i* ebp = (__m128i*)array_p;
				const auto target = _mm_set_epi32(_target, _target, _target, _target);

				for (; iter < vectorizedIterations; ebp += 4, iter++) {
					if ((mask = (_mm_movemask_ps(_mm_castsi128_ps(_mm_cmpeq_epi32(target, _mm_loadu_si128(ebp))))) |
						((_mm_movemask_ps(_mm_castsi128_ps(_mm_cmpeq_epi32(target, _mm_loadu_si128(ebp + 1))))) << 4) |
						((_mm_movemask_ps(_mm_castsi128_ps(_mm_cmpeq_epi32(target, _mm_loadu_si128(ebp + 2))))) << 8) |
						((_mm_movemask_ps(_mm_castsi128_ps(_mm_cmpeq_epi32(target, _mm_loadu_si128(ebp + 3))))) << 12))) {
						_BitScanForward((unsigned long*)&bsr_v, mask);

						index += comparesPerIter * (uint32_t)iter;

						return index + bsr_v;
					}
				}

				index += comparesPerIter * (uint32_t)iter;

				// Scan the rest 1-by-1
				for (; index < size_array; index++) 
				{
					if (data[index] == _target) 
					{
						res = index;
						break;
					}
				}

				return res;
			}

			uint32_t OptimizationLoadPatch::HKSearchIndexOffset64(EditorAPI::BSTArray<uint64_t>& _array,
				uint64_t& _target, uint32_t _start_index)
			{
				uint32_t index = _start_index;
				const uint32_t size_array = _array.QSize();

				if (size_array <= _start_index)
					return INVALID_INDEX;

				uint64_t* data = (uint64_t*)_array.QBuffer() + index;
				if ((size_array - _start_index) < 50) 
				{
					for (; index < size_array; index++) 
					{
						if (data[index] == _target)
							return index;
					}

					return INVALID_INDEX;
				}

				uint32_t res = INVALID_INDEX;

				//
				// Compare 16 pointers per iteration - use SIMD instructions to generate a bit mask. Set
				// bit 0 if 'array[i + 0]'=='target', set bit 1 if 'array[i + 1]'=='target', set bit X...
				const uint32_t comparesPerIter = 16;
				const uint32_t vectorizedIterations = (size_array - index) / comparesPerIter;
				uint64_t* array_p = (uint64_t*)(data + index);
				uint32_t bsr_v;
				uint64_t mask;
				std::size_t iter = 0;

				__m128i* ebp = (__m128i*)array_p;
				const __m128i target = _mm_set_epi64x(_target, _target);

				for (; iter < vectorizedIterations; ebp += 8, iter++) {
					if ((mask = ((uint64_t)_mm_movemask_pd(_mm_castsi128_pd(_mm_cmpeq_epi64(target, _mm_loadu_si128(ebp))))) |
						(((uint64_t)_mm_movemask_pd(_mm_castsi128_pd(_mm_cmpeq_epi64(target, _mm_loadu_si128(ebp + 1))))) << 2) |
						(((uint64_t)_mm_movemask_pd(_mm_castsi128_pd(_mm_cmpeq_epi64(target, _mm_loadu_si128(ebp + 2))))) << 4) |
						(((uint64_t)_mm_movemask_pd(_mm_castsi128_pd(_mm_cmpeq_epi64(target, _mm_loadu_si128(ebp + 3))))) << 6) |
						(((uint64_t)_mm_movemask_pd(_mm_castsi128_pd(_mm_cmpeq_epi64(target, _mm_loadu_si128(ebp + 4))))) << 8) |
						(((uint64_t)_mm_movemask_pd(_mm_castsi128_pd(_mm_cmpeq_epi64(target, _mm_loadu_si128(ebp + 5))))) << 10) |
						(((uint64_t)_mm_movemask_pd(_mm_castsi128_pd(_mm_cmpeq_epi64(target, _mm_loadu_si128(ebp + 6))))) << 12) |
						(((uint64_t)_mm_movemask_pd(_mm_castsi128_pd(_mm_cmpeq_epi64(target, _mm_loadu_si128(ebp + 7))))) << 14))) {
						_BitScanForward64((unsigned long*)&bsr_v, mask);
						index += comparesPerIter * (uint32_t)iter;

						return index + bsr_v;
					}
				}

				index += comparesPerIter * (uint32_t)iter;

				// Scan the rest 1-by-1
				for (; index < size_array; index++) 
				{
					if (data[index] == _target) 
					{
						res = index;
						break;
					}
				}

				return res;
			}
		}
	}
}