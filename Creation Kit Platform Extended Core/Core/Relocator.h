// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		class Engine;

		class Relocator
		{
		public:
			Relocator(Engine* lpEngine);
			~Relocator() = default;

			inline uintptr_t Rav2Off(uintptr_t rav) const { return _base + rav; }

			void Patch(uintptr_t rav, uint8_t* bytes, uint32_t size) const;
			void Patch(uintptr_t rav, std::initializer_list<uint8_t> data) const;
			void PatchNop(uintptr_t rav, uint32_t size) const;

			bool DetourJump(uintptr_t rav, uintptr_t function) const;
			bool DetourCall(uintptr_t rav, uintptr_t function) const;

			template<class T> uintptr_t DetourFunctionClass(uintptr_t rav, T function) const
			{
				auto offset = Rav2Off(rav);

				if (!IsLock(offset, 6))
					return 0;

				return Detours::X64::DetourFunctionClass(offset, function);
			}

			void Unlock();
			void Unlock(uintptr_t base, uint64_t size);
			void Lock(uintptr_t base, uint64_t size);

			bool IsLock(uintptr_t base, uint64_t size) const;
			inline Engine* GetEngine() const { return _engine; }
			inline uintptr_t GetBase() const { return _base; }
		private:
			Engine* _engine;
			uintptr_t _base;
			struct Region
			{
				uintptr_t base;
				uint64_t size;
				DWORD flag_lock;
			};
			Array<Region> Locks;
		};

		extern Relocator* GlobalRelocatorPtr;

		class ScopeRelocator
		{
		public:
			ScopeRelocator();
			ScopeRelocator(uintptr_t base, uint64_t size);
			~ScopeRelocator();
		private:
			ScopeRelocator(const ScopeRelocator&) = default;
			ScopeRelocator& operator=(const ScopeRelocator&) = default;

			uintptr_t _base;
			uint64_t _size;
		};
	}
}