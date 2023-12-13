// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/BSReadWriteLock.cpp

#include "BSReadWriteLock.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		BSReadWriteLock::~BSReadWriteLock()
		{
			AssertMsg(m_Bits == 0 && m_WriteCount == 0, "Destructing a lock that is still in use");
		}

		void BSReadWriteLock::LockForRead()
		{
			for (uint32_t count = 0; !TryLockForRead();)
			{
				if (++count > 1000)
					YieldProcessor();
			}
		}

		void BSReadWriteLock::UnlockRead()
		{
			if (IsWritingThread())
				return;

			m_Bits.fetch_add(-READER, std::memory_order_release);
		}

		bool BSReadWriteLock::TryLockForRead()
		{
			if (IsWritingThread())
				return true;

			// fetch_add is considerably (100%) faster than compare_exchange,
			// so here we are optimizing for the common (lock success) case.
			int16_t value = m_Bits.fetch_add(READER, std::memory_order_acquire);

			if (value & WRITER)
			{
				m_Bits.fetch_add(-READER, std::memory_order_release);
				return false;
			}

			return true;
		}

		void BSReadWriteLock::LockForWrite()
		{
			for (uint32_t count = 0; !TryLockForWrite();)
			{
				if (++count > 1000)
					YieldProcessor();
			}
		}

		void BSReadWriteLock::UnlockWrite()
		{
			if (--m_WriteCount > 0)
				return;

			m_ThreadId.store(0, std::memory_order_release);
			m_Bits.fetch_and(~WRITER, std::memory_order_release);
		}

		bool BSReadWriteLock::TryLockForWrite()
		{
			if (IsWritingThread())
			{
				m_WriteCount++;
				return true;
			}

			int16_t expect = 0;
			if (m_Bits.compare_exchange_strong(expect, WRITER, std::memory_order_acq_rel))
			{
				m_WriteCount = 1;
				m_ThreadId.store(GetCurrentThreadId(), std::memory_order_release);
				return true;
			}

			return false;
		}

		void BSReadWriteLock::LockForReadAndWrite() const
		{
			// This is only called from BSAutoReadAndWriteLock (no-op, it's always a write lock now)
		}

		bool BSReadWriteLock::IsWritingThread() const
		{
			return m_ThreadId == GetCurrentThreadId();
		}

		BSAutoReadAndWriteLock* BSAutoReadAndWriteLock::Initialize(BSReadWriteLock* Child)
		{
			m_Lock = Child;
			m_Lock->LockForWrite();

			return this;
		}

		void BSAutoReadAndWriteLock::Deinitialize() const
		{
			m_Lock->UnlockWrite();
		}
	}
}