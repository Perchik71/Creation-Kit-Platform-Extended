// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.HardwareInfo.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Patterns.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.RuntimeOptimization.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			std::uint64_t RuntimeOptimization::PatchLinkedList(std::uintptr_t beg, std::uintptr_t end) noexcept(true)
			{
				//
				// Optimize a linked list HasValue<T>() hot-code-path function. Checks if the 16-byte structure
				// is 0 (list->next pointer, list->data pointer)
				//
				const bool hasSSE41 = HardwareInfo::CPU::HasSupportSSE41();
				const char* pattern = "48 89 4C 24 08 48 83 EC 18 48 8B 44 24 20 48 83 78 08 00 75 14"
					" 48 8B 44 24 20 48 83 38 00 75 09 C7 04 24 01 00 00 00 EB 07 C7 04 24 00 00 00 00"
					" 0F B6 04 24 48 83 C4 18 C3";

				auto matches = Patterns::FindsByMask(beg, end, pattern);
				for (std::uintptr_t match : matches)
				{
					if (hasSSE41)
						memcpy((void*)match, "\xF3\x0F\x6F\x01\x66\x0F\x38\x17\xC0\x0F\x94\xC0\xC3", 13);
					else
						memcpy((void*)match, "\x48\x83\x39\x00\x75\x0A\x48\x83\x79\x08\x00\x75\x03\xB0\x01\xC3\x32\xC0\xC3", 19);
				}

				return matches.size();
			}

			std::uint64_t RuntimeOptimization::PatchTemplatedFormIterator(std::uintptr_t _beg, std::uintptr_t _end,
				std::uintptr_t base, Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				//
				// Add a callback that sets a global variable indicating UI dropdown menu entries can be
				// deferred to prevent a redraw after every new item insert. This reduces dialog initialization time.
				//
				// The _templated_ function is designed to iterate over all FORMs of a specific type - it
				// requires hooking 100-200 separate functions in the EXE as a result. False positives are
				// a non-issue as long as ctor/dtor calls are balanced.
				//
				std::uint64_t patchCount = 0;
				const char* pattern = "E8 ? ? ? ? 48 89 44 24 30 48 8B 44 24 30 48 89 44 24 38 48 8B 54 24 38 48 8D 4C 24 28";

				auto matches = Patterns::FindsByMask(_beg, _end, pattern);
				for (std::uintptr_t addr : matches)
				{
					// Make sure the next call points to sub_14102CBEF (a no-op function)
					addr += 30ull /* strlen(maskStr) */ + 11;
					std::uintptr_t destination = addr + (std::uintptr_t)(*(std::int32_t*)(addr + 1)) + 5;

					if (destination != __CKPE_OFFSET(2))
						continue;

					// Now look for the matching destructor call
					std::uintptr_t end = Patterns::FindByMask(addr, std::min<std::uintptr_t>((_beg + _end) - addr, 512),
						"E8 ? ? ? ? 0F B6 ? ? ? 48 81 C4 ? ? ? ? C3"); // sub_140FF81CE, movzx return

					if (!end)
						end = Patterns::FindByMask(addr, std::min<std::uintptr_t>((_beg + _end) - addr, 512),
							"E8 ? ? ? ? 48 81 C4 ? ? ? ? C3"); // sub_140FF81CE

					if (!end)
						continue;

					// Blacklisted (000000014148C1FF): The "Use Info" dialog has more than one list view and causes problems
					// Blacklisted (000000014169DFAD): Adding a new faction to an NPC has more than one list view
					if ((addr == __CKPE_OFFSET(3)) || (addr == __CKPE_OFFSET(4)))
						continue;

					Detours::DetourCall(addr, (std::uintptr_t)&Common::EditorUI::Hook::HKBeginUIDefer);
					Detours::DetourCall(end, (std::uintptr_t)&Common::EditorUI::Hook::HKEndUIDefer);

					patchCount += 2;
				}

				return patchCount;
			}

			RuntimeOptimization::RuntimeOptimization() : Common::Patch()
			{
				SetName("Runtime Optimization");
			}

			bool RuntimeOptimization::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* RuntimeOptimization::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool RuntimeOptimization::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> RuntimeOptimization::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool RuntimeOptimization::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool RuntimeOptimization::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// Cutting a lot is faster this way
				auto stext = interface->GetApplication()->GetSegment(Segment::text);
				ScopeSafeWrite text(stext.GetAddress(), stext.GetSize());
				
				PatchLinkedList(stext.GetAddress(), stext.GetSize());
				PatchTemplatedFormIterator(stext.GetAddress(), stext.GetSize(), base, db);

				return true;
			}
		}
	}
}