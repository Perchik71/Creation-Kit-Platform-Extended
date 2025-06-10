// Author: Nukem9 
// Link: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/CKSSE/Experimental.cpp

#include <windows.h>
#include <Zydis/Zydis.h>
#include <CKPE.Segment.h>
#include <CKPE.Application.h>
#include <CKPE.Exception.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Patterns.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.RuntimeOptimization.h>
#include <concurrent_unordered_map.h>
#include <concurrent_vector.h>
#include <algorithm>
#include <execution>
#include <chrono>

namespace CKPE
{
	namespace Common
	{
		const RuntimeOptimization::NullsubPatch Patches[] =
		{
			// Nullsub || retn; int3; int3; int3; int3; || nop;
			{ { 0xC2, 0x00, 0x00 }, { 0xC3, 0xCC, 0xCC, 0xCC, 0xCC }, { 0x0F, 0x1F, 0x44, 0x00, 0x00 } },
			{ { 0xC3 }, { 0xC3, 0xCC, 0xCC, 0xCC, 0xCC }, { 0x0F, 0x1F, 0x44, 0x00, 0x00 } },
			{ { 0x48, 0x89, 0x4C, 0x24, 0x08, 0xC3 }, { 0xC3, 0xCC, 0xCC, 0xCC, 0xCC }, { 0x0F, 0x1F, 0x44, 0x00, 0x00 } },
			{ { 0x48, 0x89, 0x54, 0x24, 0x10, 0x48, 0x89, 0x4C, 0x24, 0x08, 0xC3 }, { 0xC3, 0xCC, 0xCC, 0xCC, 0xCC }, { 0x0F, 0x1F, 0x44, 0x00, 0x00 } },
			{ { 0x48, 0x89, 0x4C, 0x24, 0x08, 0x48, 0x83, 0xEC, 0x28, 0x48, 0x8B, 0x4C, 0x24, 0x30, 0x0F, 0x1F, 0x44, 0x00, 0x00, 0x48, 0x83, 0xC4, 0x28, 0xC3 }, { 0xC3, 0xCC, 0xCC, 0xCC, 0xCC }, { 0x0F, 0x1F, 0x44, 0x00, 0x00 } },

			{
				{ 0x48, 0x89, 0x4C, 0x24, 0x08, 0x48, 0x8B, 0x44, 0x24, 0x08, 0xC3 },								// return this;
				{ 0x48, 0x89, 0xC8, 0xC3, 0xCC },																	// mov rax, rcx; retn; int3;
				{ 0x48, 0x89, 0xC8, 0x66, 0x90 }																	// mov rax, rcx; nop;
			},

			{
				{ 0x48, 0x89, 0x4C, 0x24, 0x08, 0x48, 0x8B, 0x44, 0x24, 0x08, 0x48, 0x8B, 0x00, 0xC3 },				// return *(__int64 *)this;
				{ 0x48, 0x8B, 0x01, 0xC3, 0xCC },																	// mov rax, [rcx]; retn; int3;
				{ 0x48, 0x8B, 0x01, 0x66, 0x90 }																	// mov rax, [rcx]; nop;
			},

			{
				{ 0x48, 0x89, 0x4C, 0x24, 0x08, 0x48, 0x8B, 0x44, 0x24, 0x08, 0x48, 0x8B, 0x40, 0x08, 0xC3 },		// return *(__int64 *)(this + 0x8);
				{ 0x48, 0x8B, 0x41, 0x08, 0xC3 },																	// mov rax, [rcx + 0x8]; retn;
				{ 0x48, 0x8B, 0x41, 0x08, 0x90 }																	// mov rax, [rcx + 0x8]; nop;
			},

			{
				{ 0x48, 0x89, 0x4C, 0x24, 0x08, 0x48, 0x8B, 0x44, 0x24, 0x08, 0x48, 0x8B, 0x40, 0x50, 0xC3 },		// return *(__int64 *)(this + 0x50);
				{ 0x48, 0x8B, 0x41, 0x50, 0xC3 },																	// mov rax, [rcx + 0x50]; retn;
				{ 0x48, 0x8B, 0x41, 0x50, 0x90 }																	// mov rax, [rcx + 0x50]; nop;
			},

			{
				{ 0x48, 0x89, 0x4C, 0x24, 0x08, 0x48, 0x8B, 0x44, 0x24, 0x08, 0x8B, 0x00, 0xC3 },					// return *(__int32 *)this;
				{ 0x8B, 0x01, 0xC3, 0xCC, 0xCC },																	// mov eax, [rcx]; retn; int3; int3;
				{ 0x8B, 0x01, 0x0F, 0x1F, 0x00 }																	// mov eax, [rcx]; nop;
			},

			{
				{ 0x48, 0x89, 0x4C, 0x24, 0x08, 0x48, 0x8B, 0x44, 0x24, 0x08, 0x8B, 0x40, 0x08, 0xC3 },				// return *(__int32 *)(this + 0x8);
				{ 0x8B, 0x41, 0x08, 0xC3, 0xCC },																	// mov eax, [rcx + 0x8]; retn; int3;
				{ 0x8B, 0x41, 0x08, 0x66, 0x90 }																	// mov eax, [rcx + 0x8]; nop;
			},

			{
				{ 0x48, 0x89, 0x4C, 0x24, 0x08, 0x48, 0x8B, 0x44, 0x24, 0x08, 0x8B, 0x40, 0x14, 0xC3 },				// return *(__int32 *)(this + 0x14);
				{ 0x8B, 0x41, 0x14, 0xC3, 0xCC },																	// mov eax, [rcx + 0x14]; retn; int3;
				{ 0x8B, 0x41, 0x14, 0x66, 0x90 }																	// mov eax, [rcx + 0x14]; nop;
			},

			{
				{ 0x48, 0x89, 0x4C, 0x24, 0x08, 0x48, 0x8B, 0x44, 0x24, 0x08, 0x0F, 0xB6, 0x40, 0x08, 0xC3 },		// return ZERO_EXTEND(*(__int8 *)(this + 0x8));
				{ 0x0F, 0xB6, 0x41, 0x08, 0xC3 },																	// movzx eax, [rcx + 0x8]; retn;
				{ 0x0F, 0xB6, 0x41, 0x08, 0x90 }																	// movzx eax, [rcx + 0x8]; nop;
			},

			{
				{ 0x48, 0x89, 0x4C, 0x24, 0x08, 0x48, 0x8B, 0x44, 0x24, 0x08, 0x0F, 0xB6, 0x40, 0x26, 0xC3 },		// return ZERO_EXTEND(*(__int8 *)(this + 0x26));
				{ 0x0F, 0xB6, 0x41, 0x26, 0xC3 },																	// movzx eax, [rcx + 0x26]; retn;
				{ 0x0F, 0xB6, 0x41, 0x26, 0x90 }																	// movzx eax, [rcx + 0x26]; nop;
			},

			{
				{ 0x89, 0x54, 0x24, 0x10, 0x48, 0x89, 0x4C, 0x24, 0x08, 0x8B, 0x44, 0x24, 0x10, 0x48, 0x8B, 0x4C, 0x24, 0x08, 0x0F, 0xB7, 0x04, 0x41, 0xC3 },	// return ZERO_EXTEND(*(unsigned __int16 *)(a1 + 2i64 * a2));
				{ 0x0F, 0xB7, 0x04, 0x51, 0xC3 },																												// movzx eax, word ptr ds:[rcx+rdx*2]; retn;
				{ 0x0F, 0xB7, 0x04, 0x51, 0x90 }																												// movzx eax, word ptr ds:[rcx+rdx*2]; nop;
			},

			// Added perchik71

			{
				{ 0xF3, 0x0F, 0x11, 0x44, 0x24, 0x08, 0xF3, 0x0F, 0x2C, 0x44, 0x24, 0x08, 0xC3 },		// return (int)arg1;
				{ 0xF3, 0x0F, 0x2C, 0xC0, 0xC3 },														// cvttss2si eax, xmm0; retn;
				{ 0xF3, 0x0F, 0x2C, 0xC0, 0x90 }														// cvttss2si eax, xmm0; nop;
			},
		};

		static const RuntimeOptimization::NullsubPatch* FindNullsubPatch(std::uintptr_t SourceAddress,
			std::uintptr_t TargetFunction) noexcept(true)
		{
			for (auto& patch : Patches)
			{
				if (!memcmp((void*)TargetFunction, patch.Signature.begin(), patch.Signature.size()))
					return &patch;
			}

			return nullptr;
		}

		static bool PatchNullsub(std::uintptr_t SourceAddress, std::uintptr_t TargetFunction,
			const RuntimeOptimization::NullsubPatch* Patch = nullptr) noexcept(true)
		{
			const bool isJump = *(uint8_t*)SourceAddress == 0xE9;

			// Check if the given function is "unoptimized" and remove the branch completely
			if (!Patch)
				Patch = FindNullsubPatch(SourceAddress, TargetFunction);

			if (Patch)
			{
				if (isJump)
					memcpy((void*)SourceAddress, Patch->JumpPatch, 5);
				else
					memcpy((void*)SourceAddress, Patch->CallPatch, 5);

				return true;
			}

			return false;
		}

		std::uint64_t RuntimeOptimization::RemoveTrampolinesAndNullsubs(std::uintptr_t target, std::uintptr_t size) const
		{
			auto interface = Interface::GetSingleton();
			auto app = interface->GetApplication();

			//
			// Remove any references to the giant trampoline table generated for edit & continue
			//
			// Before: [Function call] -> [E&C trampoline] -> [Function]
			// After:  [Function call] -> [Function]
			//
			concurrency::concurrent_unordered_map<std::uintptr_t, const NullsubPatch*, std::hash<std::uintptr_t>,
				std::equal_to<std::uintptr_t>> nullsubTargets;
			concurrency::concurrent_vector<std::uintptr_t> branchTargets;

			// Enumerate all functions present in the x64 exception directory section
			const auto dir_exception = app->GetPEDirectory(PEDirectory::e_exception);
			if (!dir_exception.GetAddress() || !dir_exception.GetSize()) return 0;

			const auto functionEntries = dir_exception.GetPointer<RUNTIME_FUNCTION>();
			const auto functionEntryCount = dir_exception.GetSize() / sizeof(RUNTIME_FUNCTION);

			// 
			// find trampoline table
			//
			const auto code = (std::uint8_t*)(target);

			// offset 0x5 default for trampoline table
			std::uint32_t carret = 0x5;
			const std::uintptr_t ecTableStart = (const std::uintptr_t)code + carret;

			do
			{
				if (code[carret] != 0xE9)
					break;
				carret += 5;
			} while (carret <= size);

			const std::uintptr_t ecTableEnd = (const std::uintptr_t)code + carret;
			if (ecTableStart == ecTableEnd) return 0;

			// Init threadsafe instruction decoder
			ZydisDecoder decoder;
			if (ZYDIS_SUCCESS(ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_ADDRESS_WIDTH_64)))
			{
				std::for_each(std::execution::par_unseq, &functionEntries[0], &functionEntries[functionEntryCount],
					[&branchTargets, &nullsubTargets, &decoder, &app, &ecTableStart, &ecTableEnd](const RUNTIME_FUNCTION& Function)
					{
						const std::uintptr_t base = app->GetBase();

						for (std::uint32_t offset = Function.BeginAddress; offset < Function.EndAddress;)
						{
							const std::uintptr_t ip = base + offset;
							const std::uint8_t opcode = *(std::uint8_t*)ip;
							ZydisDecodedInstruction instruction;

							if (!ZYDIS_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (void*)ip, ZYDIS_MAX_INSTRUCTION_LENGTH, ip, &instruction)))
							{
								// Decode failed. Always increase byte offset by 1.
								offset += 1;
								continue;
							}

							offset += instruction.length;

							// Must be a call or a jump
							if (opcode != 0xE9 && opcode != 0xE8)
								continue;

							std::uintptr_t destination = ip + *(std::int32_t*)(ip + 1) + 5;

							// if (destination is within E&C table)
							if ((destination >= ecTableStart) && (destination < ecTableEnd) &&
								(*(std::uint8_t*)destination == 0xE9))
							{
								// Determine where the E&C trampoline jumps to, then remove it. 
								// Each function is processed separately so thread safety is not an issue when patching. 
								// The 0xE9 opcode never changes.
								std::uintptr_t real = destination + *(std::int32_t*)(destination + 1) + 5;

								std::int32_t disp = (std::int32_t)(real - ip) - 5;
								memcpy((void*)(ip + 1), &disp, sizeof(disp));

								if (auto patch = FindNullsubPatch(ip, real))
									nullsubTargets.insert(std::make_pair(ip, patch));
								else
									branchTargets.push_back(ip);
							}
						}
					});
			}
			else
				throw RuntimeError("RuntimeOptimization: ZydisDecoderInit returned failed");

			std::uint64_t patchCount = nullsubTargets.size() + branchTargets.size();

			for (auto& [ip, patch] : nullsubTargets)
			{
				uintptr_t destination = ip + *(std::int32_t*)(ip + 1) + 5;

				if (PatchNullsub(ip, destination, patch))
					patchCount++;
			}

			// Secondary pass to remove nullsubs missed or created above
			for (uintptr_t ip : branchTargets)
			{
				uintptr_t destination = ip + *(std::int32_t*)(ip + 1) + 5;

				if (PatchNullsub(ip, destination))
					patchCount++;
			}

			return patchCount;
		}

		std::uint64_t RuntimeOptimization::RemoveMemInit(std::uintptr_t target, std::uintptr_t size) const noexcept(true)
		{
			// This is found in all the CK, so it is selected as common.
			// But in SF are some missed operations, of the same type.

			//
			// Remove the thousands of [code below] since they're useless checks:
			//
			// if ( dword_141ED6C88 != 2 ) // MemoryManager initialized flag
			//     sub_140C00D30((__int64)&unk_141ED6800, &dword_141ED6C88);
			//
			auto matches = Patterns::FindsByMask(target, size, "83 3D ? ? ? ? 02 74 13 48 8D 15 ? ? ? ? 48 8D 0D ? ? ? ? E8");
			
			for (uintptr_t match : matches)
				memcpy((void*)match, "\xEB\x1A", 2);

			return matches.size();
		}

		void RuntimeOptimization::Apply()
		{
			auto interface = Interface::GetSingleton();
			auto app = interface->GetApplication();
			auto seg_code = app->GetSegment(Segment::text);

			ScopeSafeWrite protect(seg_code.GetAddress(), seg_code.GetSize());

			_base = app->GetBase();
			std::vector<std::uint64_t> tasks;

			try
			{
				using namespace std::chrono;
				auto timerStart = high_resolution_clock::now();

				tasks.push_back(RemoveMemInit(seg_code.GetAddress(), seg_code.GetSize()));
				tasks.push_back(RemoveTrampolinesAndNullsubs(seg_code.GetAddress(), seg_code.GetSize()));

				auto duration = duration_cast<milliseconds>(high_resolution_clock::now() - timerStart).count();

				std::uint64_t total = 0;
				for (auto i : tasks) total += i;

				_CONSOLE("%s: %llu patches applied in %llums.", __FUNCTION__, total, duration);
			}
			catch (const std::exception& e)
			{
				_CONSOLE("[ERROR] %s", e.what());
			}
		}
	}
}