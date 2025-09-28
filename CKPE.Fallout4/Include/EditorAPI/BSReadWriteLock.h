// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/BSReadWriteLock.h

#pragma once

#include <cstdint>
#include <atomic>
#include <CKPE.Common.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSReadWriteLock
			{
			private:
				//
				// NOTE: In order to fit into 8 bytes, m_Bits is declared as int16. This means
				// a recursive read lock acquired more than 32,767 times is undefined behavior.
				//
				std::atomic<std::uint32_t> m_ThreadId = 0;	// We don't really care what other threads see
				std::atomic<std::int16_t> m_Bits = 0;		// Must be globally visible
				volatile std::int8_t m_WriteCount = 0;

				enum : std::int32_t
				{
					READER = 2,
					WRITER = 1
				};
			public:
				CKPE_DECLARE_CONSTRUCTOR_HOOK(BSReadWriteLock);

				BSReadWriteLock() = default;
				~BSReadWriteLock() noexcept(true);

				void LockForRead() noexcept(true);
				void UnlockRead() noexcept(true);
				bool TryLockForRead() noexcept(true);

				void LockForWrite() noexcept(true);
				void UnlockWrite() noexcept(true);
				bool TryLockForWrite() noexcept(true);

				void LockForReadAndWrite() const noexcept(true);
				bool IsWritingThread() const noexcept(true);
			};
			static_assert(sizeof(BSReadWriteLock) <= 0x8, "Lock must fit inside the original game structure");

			class BSAutoReadAndWriteLock
			{
			private:
				BSReadWriteLock* m_Lock;
			public:
				BSAutoReadAndWriteLock() = delete;

				// Constructor hook
				BSAutoReadAndWriteLock* Initialize(BSReadWriteLock* Child) noexcept(true);
				// Destructor hook
				void Deinitialize() const noexcept(true);
			};
			static_assert(sizeof(BSAutoReadAndWriteLock) <= 0x8, "Lock must fit inside the original game structure");
		}
	}
}