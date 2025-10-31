// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BSReadWriteLock.h"
#include "BSSpinLock.h"
#include <algorithm>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSNonReentrantSpinLock
			{
			protected:
				std::uint32_t lock{ 0 };
			public:
				constexpr BSNonReentrantSpinLock() noexcept(true) {}
			};
			static_assert(sizeof(BSNonReentrantSpinLock) == 0x4);

			template<class Mutex>
			struct BSAutoLockDefaultPolicy
			{
			public:
				static void Lock(Mutex& Mutex) { Mutex.Acquire(); }
				static void Unlock(Mutex& Mutex) { Mutex.Release(); }
			};

			extern template struct BSAutoLockDefaultPolicy<BSSpinLock>;

			template<class Mutex>
			struct BSAutoLockReadLockPolicy
			{
			public:
				static void Lock(Mutex& Mutex) { Mutex.LockForRead(); }
				static void Unlock(Mutex& Mutex) { Mutex.UnlockRead(); }
			};

			extern template struct BSAutoLockReadLockPolicy<BSReadWriteLock>;

			template<class Mutex>
			struct BSAutoLockWriteLockPolicy
			{
			public:
				static void Lock(Mutex& Mutex) { Mutex.LockForWrite(); }
				static void Unlock(Mutex& Mutex) { Mutex.UnlockWrite(); }
			};

			extern template struct BSAutoLockWriteLockPolicy<BSReadWriteLock>;

			template<class Mutex, template <class> class Policy = BSAutoLockDefaultPolicy>
			class BSAutoLock
			{
			public:
				using mutex_type = Mutex;
				using policy_type = Policy<mutex_type>;
			private:
				mutex_type* _lock{ nullptr };
			public:
				explicit BSAutoLock(mutex_type& Mutex) : _lock(std::addressof(Mutex)) { policy_type::Lock(*_lock); }
				explicit BSAutoLock(mutex_type* Mutex) : _lock(Mutex) { if (_lock) policy_type::Lock(*_lock); }
				~BSAutoLock() { if (_lock) policy_type::Unlock(*_lock); }
			};

			template<class Mutex>
			BSAutoLock(Mutex&) -> BSAutoLock<Mutex>;

			template<class Mutex>
			BSAutoLock(Mutex*) -> BSAutoLock<Mutex>;

			extern template class BSAutoLock<BSSpinLock, BSAutoLockDefaultPolicy>;
			extern template class BSAutoLock<BSReadWriteLock, BSAutoLockReadLockPolicy>;
			extern template class BSAutoLock<BSReadWriteLock, BSAutoLockWriteLockPolicy>;

			using BSAutoReadLock = BSAutoLock<BSReadWriteLock, BSAutoLockReadLockPolicy>;
			using BSAutoWriteLock = BSAutoLock<BSReadWriteLock, BSAutoLockWriteLockPolicy>;
		}
	}
}
