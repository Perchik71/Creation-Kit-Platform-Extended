// Author: Nukem9 
// Link: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/CKSSE/Experimental.cpp

#include <Zydis/Zydis.h>
#include "Core/Engine.h"
#include "Core/Relocator.h"
#include "RuntimeOptimization.h"
#include "Editor API/EditorUI.h"

namespace CreationKitPlatformExtended
{
	namespace Experimental
	{
		void RunOptimizations()
		{
			auto Patch = Core::GlobalRelocationDatabasePtr->GetByName("Runtime Optimization");
			if (Patch.Empty())
			{
				_WARNING("The module was not found in the database: \"Runtime Optimization\"");
				return;
			}
			auto textSection = Core::GlobalEnginePtr->GetSection(Core::SECTION_TEXT);
			auto dataSection = Core::GlobalEnginePtr->GetSection(Core::SECTION_DATA);
			auto rdataSection = Core::GlobalEnginePtr->GetSection(Core::SECTION_DATA_READONLY);

			// Mark every page as writable
			Core::ScopeRelocator text(textSection.base, textSection.end - textSection.base);
			Core::ScopeRelocator data(dataSection.base, dataSection.end - dataSection.base);
			Core::ScopeRelocator rdata(rdataSection.base, rdataSection.end - rdataSection.base);

			using namespace std::chrono;
			auto timerStart = high_resolution_clock::now();

			if (Patch->Version() == 1)
			{
				std::array<uint64_t, 4> counts
				{
					PatchMemInit(),
					PatchLinkedList(),
					PatchTemplatedFormIterator(Patch),
					PatchEditAndContinue(Patch),
				};

				auto duration = duration_cast<milliseconds>(high_resolution_clock::now() - timerStart).count();
				_CONSOLE("%s: (%llu + %llu + %llu + %llu) = %llu patches applied in %llums.\n", __FUNCTION__,
					counts[0], counts[1], counts[2], counts[3], counts[0] + counts[1] + counts[2] + counts[3], duration);
			}
			else if (Patch->Version() == 2)
			{
				std::array<uint64_t, 2> counts
				{
					PatchMemInit(),
					PatchEditAndContinue(Patch),
				};

				auto duration = duration_cast<milliseconds>(high_resolution_clock::now() - timerStart).count();
				_CONSOLE("%s: (%llu + %llu) = %llu patches applied in %llums.\n", __FUNCTION__,
					counts[0], counts[1], counts[0] + counts[1], duration);
			}
		}

		uint64_t PatchEditAndContinue(SmartPointer<Core::RelocationDatabaseItem> patch)
		{
			//
			// Remove any references to the giant trampoline table generated for edit & continue
			//
			// Before: [Function call] -> [E&C trampoline] -> [Function]
			// After:  [Function call] -> [Function]
			//
			concurrency::concurrent_unordered_map<uintptr_t, const NullsubPatch*, std::hash<uintptr_t>,
				std::equal_to<uintptr_t>, voltek::allocator<std::pair<const uintptr_t, const NullsubPatch*>>> nullsubTargets;
			concurrency::concurrent_vector<uintptr_t, voltek::allocator<uintptr_t>> branchTargets;

			// Enumerate all functions present in the x64 exception directory section
			auto moduleBase = Core::GlobalEnginePtr->GetModuleBase();
			auto ntHeaders = (PIMAGE_NT_HEADERS64)(moduleBase + ((PIMAGE_DOS_HEADER)moduleBase)->e_lfanew);
			const auto sectionRVA = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].VirtualAddress;
			const auto sectionSize = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].Size;

			Assert(sectionRVA > 0 && sectionSize > 0);

			auto functionEntries = (PRUNTIME_FUNCTION)(moduleBase + sectionRVA);
			auto functionEntryCount = sectionSize / sizeof(RUNTIME_FUNCTION);

			// Init threadsafe instruction decoder
			ZydisDecoder decoder;
			Assert(ZYDIS_SUCCESS(ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_ADDRESS_WIDTH_64)));
			SmartPointer<Core::RelocationDatabaseItem> _Patch = patch;

			std::for_each(std::execution::par_unseq, &functionEntries[0], &functionEntries[functionEntryCount],
				[&branchTargets, &nullsubTargets, &decoder, &_Patch](const RUNTIME_FUNCTION& Function)
				{
					const uintptr_t ecTableStart = Core::GlobalRelocatorPtr->Rav2Off(_Patch->At(0));
					const uintptr_t ecTableEnd = Core::GlobalRelocatorPtr->Rav2Off(_Patch->At(1));

					for (uint32_t offset = Function.BeginAddress; offset < Function.EndAddress;)
					{
						const uintptr_t ip = Core::GlobalEnginePtr->GetModuleBase() + offset;
						const uint8_t opcode = *(uint8_t*)ip;
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

						uintptr_t destination = ip + *(int32_t*)(ip + 1) + 5;

						// if (destination is within E&C table)
						if (destination >= ecTableStart && destination < ecTableEnd && *(uint8_t*)destination == 0xE9)
						{
							// Determine where the E&C trampoline jumps to, then remove it. Each function is processed separately so thread
							// safety is not an issue when patching. The 0xE9 opcode never changes.
							uintptr_t real = destination + *(int32_t*)(destination + 1) + 5;

							int32_t disp = (int32_t)(real - ip) - 5;
							memcpy((void*)(ip + 1), &disp, sizeof(disp));

							if (auto patch = FindNullsubPatch(ip, real))
								nullsubTargets.insert(std::make_pair(ip, patch));
							else
								branchTargets.push_back(ip);
						}
					}
				});

			uint64_t patchCount = nullsubTargets.size() + branchTargets.size();

			for (auto [ip, patch] : nullsubTargets)
			{
				uintptr_t destination = ip + *(int32_t*)(ip + 1) + 5;

				if (PatchNullsub(ip, destination, patch))
					patchCount++;
			}

			// Secondary pass to remove nullsubs missed or created above
			for (uintptr_t ip : branchTargets)
			{
				uintptr_t destination = ip + *(int32_t*)(ip + 1) + 5;

				if (PatchNullsub(ip, destination))
					patchCount++;
			}

			return patchCount;
		}

		uint64_t PatchMemInit()
		{
			//
			// Remove the thousands of [code below] since they're useless checks:
			//
			// if ( dword_141ED6C88 != 2 ) // MemoryManager initialized flag
			//     sub_140C00D30((__int64)&unk_141ED6800, &dword_141ED6C88);
			//
			auto Sec = Core::GlobalEnginePtr->GetSection(Core::SECTION_TEXT);
			auto matches = Utils::FindPatterns(Sec.base, Sec.end - Sec.base,
				"83 3D ? ? ? ? 02 74 13 48 8D 15 ? ? ? ? 48 8D 0D ? ? ? ? E8");

			for (uintptr_t match : matches)
				memcpy((void*)match, "\xEB\x1A", 2);

			return matches.size();
		}

		uint64_t PatchLinkedList()
		{
			//
			// Optimize a linked list HasValue<T>() hot-code-path function. Checks if the 16-byte structure
			// is 0 (list->next pointer, list->data pointer)
			//
			int cpuinfo[4];
			__cpuid(cpuinfo, 1);

			const bool hasSSE41 = Core::GlobalEnginePtr->HasSSE41();
			const char* pattern = "48 89 4C 24 08 48 83 EC 18 48 8B 44 24 20 48 83 78 08 00 75 14"
				" 48 8B 44 24 20 48 83 38 00 75 09 C7 04 24 01 00 00 00 EB 07 C7 04 24 00 00 00 00"
				" 0F B6 04 24 48 83 C4 18 C3";

			auto Sec = Core::GlobalEnginePtr->GetSection(Core::SECTION_TEXT);
			auto matches = Utils::FindPatterns(Sec.base, Sec.end - Sec.base, pattern);

			for (uintptr_t match : matches)
			{
				if (hasSSE41)
					memcpy((void*)match, "\xF3\x0F\x6F\x01\x66\x0F\x38\x17\xC0\x0F\x94\xC0\xC3", 13);
				else
					memcpy((void*)match, "\x48\x83\x39\x00\x75\x0A\x48\x83\x79\x08\x00\x75\x03\xB0\x01\xC3\x32\xC0\xC3", 19);
			}

			return matches.size();
		}

		uint64_t PatchTemplatedFormIterator(SmartPointer<Core::RelocationDatabaseItem> patch)
		{
			//
			// Add a callback that sets a global variable indicating UI dropdown menu entries can be
			// deferred to prevent a redraw after every new item insert. This reduces dialog initialization time.
			//
			// The _templated_ function is designed to iterate over all FORMs of a specific type - it
			// requires hooking 100-200 separate functions in the EXE as a result. False positives are
			// a non-issue as long as ctor/dtor calls are balanced.
			//
			uint64_t patchCount = 0;
			const char* pattern = "E8 ? ? ? ? 48 89 44 24 30 48 8B 44 24 30 48 89 44 24 38 48 8B 54 24 38 48 8D 4C 24 28";

			auto Sec = Core::GlobalEnginePtr->GetSection(Core::SECTION_TEXT);
			auto matches = Utils::FindPatterns(Sec.base, Sec.end - Sec.base, pattern);

			for (uintptr_t addr : matches)
			{
				// Make sure the next call points to sub_14102CBEF (a no-op function)
				addr += 30 /* strlen(maskStr) */ + 11;
				uintptr_t destination = addr + *(int32_t*)(addr + 1) + 5;

				if (destination != Core::GlobalRelocatorPtr->Rav2Off(patch->At(2)))
					continue;

				// Now look for the matching destructor call
				uintptr_t end = Utils::FindPattern(addr, std::min<uintptr_t>(Sec.end - addr, 512), 
					"E8 ? ? ? ? 0F B6 ? ? ? 48 81 C4 ? ? ? ? C3"); // sub_140FF81CE, movzx return

				if (!end)
					end = Utils::FindPattern(addr, std::min<uintptr_t>(Sec.end - addr, 512), 
						"E8 ? ? ? ? 48 81 C4 ? ? ? ? C3"); // sub_140FF81CE

				if (!end)
					continue;

				// Blacklisted (000000014148C1FF): The "Use Info" dialog has more than one list view and causes problems
				// Blacklisted (000000014169DFAD): Adding a new faction to an NPC has more than one list view
				if ((addr == Core::GlobalRelocatorPtr->Rav2Off(patch->At(3))) || 
					(addr == Core::GlobalRelocatorPtr->Rav2Off(patch->At(4))))
					continue;

				Utils::DetourCall(addr, (uintptr_t)&EditorAPI::EditorUI::HKBeginUIDefer);
				Utils::DetourCall(end, (uintptr_t)&EditorAPI::EditorUI::HKEndUIDefer);

				patchCount += 2;
			}

			return patchCount;
		}

		const NullsubPatch Patches[] =
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
		};

		const NullsubPatch* FindNullsubPatch(uintptr_t SourceAddress, uintptr_t TargetFunction)
		{
			for (auto& patch : Patches)
			{
				if (!memcmp((void*)TargetFunction, patch.Signature.begin(), patch.Signature.size()))
					return &patch;
			}

			return nullptr;
		}

		bool PatchNullsub(uintptr_t SourceAddress, uintptr_t TargetFunction, const NullsubPatch* Patch)
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
	}
}