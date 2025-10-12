// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <intrin.h>

#include <CKPE.Asserts.h>
#include <CKPE.HardwareInfo.h>
#include <CKPE.Patterns.h>
#include <CKPE.Utils.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/BSFile.h>
#include <EditorAPI/BSTArray.h>
#include <Patches/CKPE.Fallout4.Patch.OptimizationLoad.h>

#include <xbyak.h>
#include <libdeflate.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			extern std::uintptr_t pointer_BSFile_sub;
		}

		namespace Patch
		{
			constexpr std::uint32_t INVALID_INDEX = 0xFFFFFFFF;

			std::uintptr_t pointer_OptimizationLoad_sub1 = 0;

			OptimizationLoad::OptimizationLoad() : Common::Patch()
			{
				SetName("Optimization load");
			}

			bool OptimizationLoad::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* OptimizationLoad::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool OptimizationLoad::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> OptimizationLoad::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool OptimizationLoad::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool OptimizationLoad::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto verPatch = db->GetVersion();
				if ((verPatch != 1) && (verPatch != 2))
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

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
				Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&sub);
				pointer_OptimizationLoad_sub1 = __CKPE_OFFSET(1);

				Detours::DetourCall(__CKPE_OFFSET(2), (std::uintptr_t)&HKInflateInit);
				Detours::DetourCall(__CKPE_OFFSET(3), (std::uintptr_t)&HKInflate);

				Detours::DetourIAT(base, "kernel32.dll", "FindFirstFileA", (std::uintptr_t)HKFindFirstFileA);

				// Skip remove failed forms
				SafeWrite::Write(__CKPE_OFFSET(4), { 0xEB });

				EditorAPI::pointer_BSFile_sub = __CKPE_OFFSET(12);
				// 2 kb -> x kb >= 256 kb
				*(std::uintptr_t*)&EditorAPI::BSFile::ICreateInstance =
					Detours::DetourClassJump(EditorAPI::pointer_BSFile_sub,
						(std::uintptr_t)&EditorAPI::BSFile::HKCreateInstance);
				// 2 kb -> 256 kb
				if (verPatch == 1)
				{
					SafeWrite::Write(__CKPE_OFFSET(13), { 0x41, 0xB9, 0x00, 0x00, 0x04, 0x00, 0x90 });
					SafeWrite::WriteNop(__CKPE_OFFSET(15), 4);
				}
				else
					SafeWrite::Write(__CKPE_OFFSET(13), { 0x41, 0x89, 0xC6, 0x90 });

				SafeWrite::Write(__CKPE_OFFSET(14), { 0x04 });

				if (_READ_OPTION_BOOL("Animation", "bSkipAnimationBuildData", false))
				{
					// Skipping Export Anim
					SafeWrite::Write(__CKPE_OFFSET(16), { 0xC3 });
					// Remove temporary files
					DeleteFileA("TemporaryBehaviorEventInfoOutput.txt");
					DeleteFileA("TemporaryClipDataOutput.txt");
					DeleteFileA("TemporarySyncAnimDataOutput.txt");
				}

				// Without SIMD support, there is no pointand
				if ((verPatch == 1) && HardwareInfo::CPU::HasSupportSSE41())
				{
					std::size_t count = 0;

					// Cutting a lot is faster this way
					auto stext = interface->GetApplication()->GetSegment(Segment::text);

					std::vector<std::uintptr_t>::iterator match;
					auto matches = Patterns::FindsByMask(stext.GetAddress(), stext.GetSize(),
						"48 83 C5 08 41 3B FE 72 9F 48 8B 6C 24 50 8B C3 48 8B 5C 24 58 48 8B 74"
						" 24 60 48 83 C4 30 41 5F 41 5E 5F C3 8B C3 EB E8");

					std::for_each(match = matches.begin(), matches.end(), [&count](auto it)
						{ Detours::DetourJump(it - 0x8A, (std::uintptr_t)&HKSearchIndex64); count++; });

					matches = Patterns::FindsByMask(stext.GetAddress(), stext.GetSize(),
						"48 83 C5 08 41 3B FE 72 9F 48 8B 6C 24 58 48 8B 74 24 60 8B C3 48 8B 5C"
						" 24 50 48 83 C4 30 41 5F 41 5E 5F C3");

					std::for_each(match = matches.begin(), matches.end(), [&count](auto it)
						{ Detours::DetourJump(it - 0x8D, (std::uintptr_t)&HKSearchIndexOffset64); count++; });

					Detours::DetourJump(__CKPE_OFFSET(5), (std::uintptr_t)&HKSearchIndex32);
					Detours::DetourJump(__CKPE_OFFSET(6), (std::uintptr_t)&HKSearchIndex32);
					Detours::DetourJump(__CKPE_OFFSET(7), (std::uintptr_t)&HKSearchIndex32);

					class Search_IA128 : public Xbyak::CodeGenerator 
					{
					public:
						Search_IA128(VOID) : Xbyak::CodeGenerator() 
						{
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

						static VOID Generate(std::uintptr_t Target)
						{
							auto hook = new Search_IA128();
							Detours::DetourJump(Target, (std::uintptr_t)hook->getCode());
						}

					};

					Search_IA128::Generate(__CKPE_OFFSET(8));
					Search_IA128::Generate(__CKPE_OFFSET(9));
					Search_IA128::Generate(__CKPE_OFFSET(10));
					Search_IA128::Generate(__CKPE_OFFSET(11));

					count += 3;

					_MESSAGE("Replaced function with SIMD function: %d.", count);
				}

				return true;
			}

			static std::uint64_t timeStartTick = 0;
			static std::uint64_t timeCurrentTick = 0;

			void OptimizationLoad::sub(std::int64_t a1) noexcept(true)
			{
				timeCurrentTick = GetTickCount64();

				// 250ms
				if ((timeCurrentTick - timeStartTick) >= 250)
				{
					fast_call<void>(pointer_OptimizationLoad_sub1, a1);

					timeStartTick = GetTickCount64();
					timeCurrentTick = timeStartTick;
				}
			}

			int OptimizationLoad::HKInflateInit(z_stream_s* Stream, const char* Version, std::int32_t Mode) noexcept(true)
			{
				// Force inflateEnd to error out and skip frees
				Stream->state = nullptr;

				return 0;
			}

			int OptimizationLoad::HKInflate(z_stream_s* Stream, std::int32_t Flush) noexcept(true)
			{
				std::size_t outBytes = 0;
				libdeflate_decompressor* decompressor = libdeflate_alloc_decompressor();

				libdeflate_result result = libdeflate_zlib_decompress(decompressor, Stream->next_in, Stream->avail_in, Stream->next_out, Stream->avail_out, &outBytes);
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

			HANDLE OptimizationLoad::HKFindFirstFileA(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData) noexcept(true)
			{
				return FindFirstFileExA(lpFileName, FindExInfoStandard, lpFindFileData, FindExSearchNameMatch,
					NULL, FIND_FIRST_EX_LARGE_FETCH);
			}

			std::uint32_t OptimizationLoad::HKSearchIndexOffset32(EditorAPI::BSTArray<std::uint32_t>& _array,
				std::uint32_t& _target, std::uint32_t _start_index) noexcept(true)
			{
				std::uint32_t index = _start_index;
				const std::uint32_t size_array = _array.size();

				if (size_array <= _start_index)
					return INVALID_INDEX;

				std::uint32_t* data = (std::uint32_t*)_array.data() + index;

				if ((size_array - _start_index) < 50)
				{
					for (; index < size_array; index++)
					{
						if (data[index] == _target)
							return index;
					}

					return INVALID_INDEX;
				}

				std::uint32_t res = INVALID_INDEX;

				//
				// Compare 16 indexes per iteration - use SIMD instructions to generate a bit mask. Set
				// bit 0 if 'array[i + 0]'=='target', set bit 1 if 'array[i + 1]'=='target', set bit X...
				const std::uint32_t comparesPerIter = 16;
				const std::uint32_t vectorizedIterations = (size_array - index) / comparesPerIter;

				std::uint32_t bsr_v;
				std::uint32_t mask;
				std::size_t iter = 0;
				std::uint32_t* array_p = (std::uint32_t*)(data + index);

				__m128i* ebp = (__m128i*)array_p;
				const auto target = _mm_set_epi32(_target, _target, _target, _target);

				for (; iter < vectorizedIterations; ebp += 4, iter++) {
					if ((mask = (_mm_movemask_ps(_mm_castsi128_ps(_mm_cmpeq_epi32(target, _mm_loadu_si128(ebp))))) |
						((_mm_movemask_ps(_mm_castsi128_ps(_mm_cmpeq_epi32(target, _mm_loadu_si128(ebp + 1))))) << 4) |
						((_mm_movemask_ps(_mm_castsi128_ps(_mm_cmpeq_epi32(target, _mm_loadu_si128(ebp + 2))))) << 8) |
						((_mm_movemask_ps(_mm_castsi128_ps(_mm_cmpeq_epi32(target, _mm_loadu_si128(ebp + 3))))) << 12))) {
						_BitScanForward((unsigned long*)&bsr_v, mask);

						index += comparesPerIter * (std::uint32_t)iter;

						return index + bsr_v;
					}
				}

				index += comparesPerIter * (std::uint32_t)iter;

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

			std::uint32_t OptimizationLoad::HKSearchIndexOffset64(EditorAPI::BSTArray<std::uint64_t>& _array,
				std::uint64_t& _target, std::uint32_t _start_index) noexcept(true)
			{
				std::uint32_t index = _start_index;
				const std::uint32_t size_array = _array.size();

				if (size_array <= _start_index)
					return INVALID_INDEX;

				std::uint64_t* data = (std::uint64_t*)_array.data() + index;
				if ((size_array - _start_index) < 50)
				{
					for (; index < size_array; index++)
					{
						if (data[index] == _target)
							return index;
					}

					return INVALID_INDEX;
				}

				std::uint32_t res = INVALID_INDEX;

				//
				// Compare 16 pointers per iteration - use SIMD instructions to generate a bit mask. Set
				// bit 0 if 'array[i + 0]'=='target', set bit 1 if 'array[i + 1]'=='target', set bit X...
				const std::uint32_t comparesPerIter = 16;
				const std::uint32_t vectorizedIterations = (size_array - index) / comparesPerIter;
				std::uint64_t* array_p = (std::uint64_t*)(data + index);
				std::uint32_t bsr_v;
				std::uint64_t mask;
				std::size_t iter = 0;

				__m128i* ebp = (__m128i*)array_p;
				const __m128i target = _mm_set_epi64x(_target, _target);

				for (; iter < vectorizedIterations; ebp += 8, iter++) {
					if ((mask = ((std::uint64_t)_mm_movemask_pd(_mm_castsi128_pd(_mm_cmpeq_epi64(target, _mm_loadu_si128(ebp))))) |
						(((std::uint64_t)_mm_movemask_pd(_mm_castsi128_pd(_mm_cmpeq_epi64(target, _mm_loadu_si128(ebp + 1))))) << 2) |
						(((std::uint64_t)_mm_movemask_pd(_mm_castsi128_pd(_mm_cmpeq_epi64(target, _mm_loadu_si128(ebp + 2))))) << 4) |
						(((std::uint64_t)_mm_movemask_pd(_mm_castsi128_pd(_mm_cmpeq_epi64(target, _mm_loadu_si128(ebp + 3))))) << 6) |
						(((std::uint64_t)_mm_movemask_pd(_mm_castsi128_pd(_mm_cmpeq_epi64(target, _mm_loadu_si128(ebp + 4))))) << 8) |
						(((std::uint64_t)_mm_movemask_pd(_mm_castsi128_pd(_mm_cmpeq_epi64(target, _mm_loadu_si128(ebp + 5))))) << 10) |
						(((std::uint64_t)_mm_movemask_pd(_mm_castsi128_pd(_mm_cmpeq_epi64(target, _mm_loadu_si128(ebp + 6))))) << 12) |
						(((std::uint64_t)_mm_movemask_pd(_mm_castsi128_pd(_mm_cmpeq_epi64(target, _mm_loadu_si128(ebp + 7))))) << 14))) {
						_BitScanForward64((unsigned long*)&bsr_v, mask);
						index += comparesPerIter * (std::uint32_t)iter;

						return index + bsr_v;
					}
				}

				index += comparesPerIter * (std::uint32_t)iter;

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