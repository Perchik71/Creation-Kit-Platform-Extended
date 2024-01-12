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

			template<class T> bool DetourJump(uintptr_t rav, T function) const
			{
				return DetourJump(rav, *(uintptr_t*)&function);
			}

			template<class T> bool DetourCall(uintptr_t rav, T function) const
			{
				return DetourCall(rav, *(uintptr_t*)&function);
			}

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

		// thread-safe template versions of fastCall()

		template<typename TR>
		__forceinline TR fastCall(size_t reloff) {
			return ((TR(__fastcall*)())GlobalRelocatorPtr->Rav2Off(reloff))();
		}

		template<typename TR, typename T1>
		__forceinline TR fastCall(size_t reloff, T1 a1) {
			return ((TR(__fastcall*)(T1))GlobalRelocatorPtr->Rav2Off(reloff))(a1);
		}

		template<typename TR, typename T1, typename T2>
		__forceinline TR fastCall(size_t reloff, T1 a1, T2 a2) {
			return ((TR(__fastcall*)(T1, T2))GlobalRelocatorPtr->Rav2Off(reloff))(a1, a2);
		}

		template<typename TR, typename T1, typename T2, typename T3>
		__forceinline TR fastCall(size_t reloff, T1 a1, T2 a2, T3 a3) {
			return ((TR(__fastcall*)(T1, T2, T3))GlobalRelocatorPtr->Rav2Off(reloff))(a1, a2, a3);
		}

		template<typename TR, typename T1, typename T2, typename T3, typename T4>
		__forceinline TR fastCall(size_t reloff, T1 a1, T2 a2, T3 a3, T4 a4) {
			return ((TR(__fastcall*)(T1, T2, T3, T4))GlobalRelocatorPtr->Rav2Off(reloff))(a1, a2, a3, a4);
		}

		template<typename TR, typename T1, typename T2, typename T3, typename T4, typename T5>
		__forceinline TR fastCall(size_t reloff, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) {
			return ((TR(__fastcall*)(T1, T2, T3, T4, T5))GlobalRelocatorPtr->Rav2Off(reloff))(a1, a2, a3, a4, a5);
		}

		template<typename TR, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
		__forceinline TR fastCall(size_t reloff, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) {
			return ((TR(__fastcall*)(T1, T2, T3, T4, T5, T6))GlobalRelocatorPtr->Rav2Off(reloff))(a1, a2, a3, a4, a5, a6);
		}

		template<typename TR, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
		__forceinline TR fastCall(size_t reloff, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7) {
			return ((TR(__fastcall*)(T1, T2, T3, T4, T5, T6, T7))GlobalRelocatorPtr->Rav2Off(reloff))(a1, a2, a3, a4, a5, a6, a7);
		}

		template<typename TR, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
		__forceinline TR fastCall(size_t reloff, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8) {
			return ((TR(__fastcall*)(T1, T2, T3, T4, T5, T6, T7, T8))GlobalRelocatorPtr->Rav2Off(reloff))(a1, a2, a3, a4, a5, a6, a7, a8);
		}

		// thread-safe template versions of thisCall()

		template<typename TR>
		__forceinline TR thisCall(size_t reloff, const void* ths) {
			return ((TR(__fastcall*)(const void*))GlobalRelocatorPtr->Rav2Off(reloff))(ths);
		}

		template<typename TR, typename T1>
		__forceinline TR thisCall(size_t reloff, const void* ths, T1 a1) {
			return ((TR(__fastcall*)(const void*, T1))GlobalRelocatorPtr->Rav2Off(reloff))(ths, a1);
		}

		template<typename TR, typename T1, typename T2>
		__forceinline TR thisCall(size_t reloff, const void* ths, T1 a1, T2 a2) {
			return ((TR(__fastcall*)(const void*, T1, T2))GlobalRelocatorPtr->Rav2Off(reloff))(ths, a1, a2);
		}

		template<typename TR, typename T1, typename T2, typename T3>
		__forceinline TR thisCall(size_t reloff, const void* ths, T1 a1, T2 a2, T3 a3) {
			return ((TR(__fastcall*)(const void*, T1, T2, T3))GlobalRelocatorPtr->Rav2Off(reloff))(ths, a1, a2, a3);
		}

		template<typename TR, typename T1, typename T2, typename T3, typename T4>
		__forceinline TR thisCall(size_t reloff, const void* ths, T1 a1, T2 a2, T3 a3, T4 a4) {
			return ((TR(__fastcall*)(const void*, T1, T2, T3, T4))GlobalRelocatorPtr->Rav2Off(reloff))(ths, a1, a2, a3, a4);
		}
	}
}