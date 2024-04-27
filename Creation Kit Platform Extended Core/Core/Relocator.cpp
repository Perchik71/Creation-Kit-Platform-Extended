// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Engine.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		Relocator* GlobalRelocatorPtr = nullptr;

		Relocator::Relocator(Engine* lpEngine) : _engine(lpEngine)
		{
			_base = _engine->GetModuleBase();
		}

		uintptr_t Relocator::Rav2Off(uintptr_t rav) const
		{ 
			return _base + rav;
		}

		void Relocator::Patch(uintptr_t rav, uint8_t* bytes, uint32_t size) const
		{
			if (!rav) return;

			auto offset = Rav2Off(rav);

			if (IsLock(offset, size))
				voltek::detours_patch_memory(offset, bytes, size);
			else
				voltek::detours_patch_memory_unsafe(offset, bytes, size);
		}

		void Relocator::Patch(uintptr_t rav, std::initializer_list<uint8_t> data) const
		{
			if (!rav) return;

			auto offset = Rav2Off(rav);

			if (IsLock(offset, data.size()))
				voltek::detours_patch_memory(offset, data);
			else
				voltek::detours_patch_memory_unsafe(offset, data);
		}

		void Relocator::PatchNop(uintptr_t rav, uint32_t size) const
		{
			if (!rav) return;

			auto offset = Rav2Off(rav);

			if (IsLock(offset, size))
				voltek::detours_patch_memory_nop(offset, size);
			else
				voltek::detours_patch_memory_nop_unsafe(offset, size);
		}

		bool Relocator::DetourJump(uintptr_t rav, uintptr_t function) const
		{
			if (!rav) return false;

			auto offset = Rav2Off(rav);

			if (!IsLock(offset, 5))
				return false;

			voltek::detours_jump(offset, function);

			return true;
		}

		bool Relocator::DetourCall(uintptr_t rav, uintptr_t function) const
		{
			if (!rav) return false;

			auto offset = Rav2Off(rav);

			if (!IsLock(offset, 5))
				return false;

			voltek::detours_call(offset, function);

			return true;
		}

		uintptr_t Relocator::DetourFunction(uintptr_t rav, uintptr_t function) const
		{
			if (!rav) return 0;

			auto offset = Rav2Off(rav);

			if (!IsLock(offset, 6))
				return 0;

			return voltek::detours_jump(offset, function);
		}

		bool Relocator::IsLock(uintptr_t base, uint64_t size) const
		{
			for (auto It = Locks.begin(); It != Locks.end(); It++)
			{
				if ((It->base >= base) && ((It->base + It->size) >= (base + size)))
					return false;
			}

			return true;
		}

		void Relocator::Unlock()
		{
			auto Sec = _engine->GetSection(SECTION_TEXT);
			Unlock(Sec.base, Sec.end - Sec.base);
		}

		void Relocator::Unlock(uintptr_t base, uint64_t size)
		{
			Region r = {
				base,
				size,
				0
			};

			if (!IsLock(r.base, r.size)) return;

			r.flag_lock = voltek::detours_unlock_protected(r.base, r.size);
			//_MESSAGE("Removing protection from the region: %016X %llu", r.base, r.size);

			Locks.push_back(r);
		}

		void Relocator::Lock(uintptr_t base, uint64_t size)
		{
			for (auto It = Locks.begin(); It != Locks.end(); It++)
			{
				if ((It->base >= base) && ((It->base + It->size) >= (base + size)))
				{
					voltek::detours_lock_protected(It->base, It->size, It->flag_lock);
					//_MESSAGE("Restore protection from the region: %016X %llu", It->base, It->size);
					Locks.erase(It);

					break;
				}
			}
		}

		ScopeRelocator::ScopeRelocator() : _base(0), _size(0)
		{
			auto Sec = GlobalRelocatorPtr->GetEngine()->GetSection(SECTION_TEXT);

			_base = Sec.base;
			_size = Sec.end - Sec.base;

			GlobalRelocatorPtr->Unlock(_base, _size);
		}

		ScopeRelocator::ScopeRelocator(uintptr_t base, uint64_t size) : _base(base), _size(size)
		{
			GlobalRelocatorPtr->Unlock(_base, _size);
		}

		ScopeRelocator::~ScopeRelocator()
		{
			GlobalRelocatorPtr->Lock(_base, _size);
		}
	}
}