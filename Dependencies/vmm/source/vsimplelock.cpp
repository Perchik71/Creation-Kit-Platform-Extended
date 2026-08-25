// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#if defined(_MSC_VER)
#	pragma warning(disable : 26110)
#endif

#include "vsimplelock.h"
#include "valloc.h"

#if (defined(_WIN32) || defined(_WIN64))	
#	include <windows.h>
#else
#	include <mutex>
#endif

namespace voltek
{
	namespace core
	{
		namespace _internal
		{
#if (defined(_WIN32) || defined(_WIN64))
			// Конструктор по умолчанию.
			simple_lock::simple_lock() noexcept
			{
				auto section =
					(PCRITICAL_SECTION)aligned_malloc(sizeof(CRITICAL_SECTION), 0x10);
				if (section)
				{
					InitializeCriticalSection(section);
					handle = (void*)section;
				}
			}
			// Деструктор.
			simple_lock::~simple_lock() noexcept
			{
				if (handle)
				{
					DeleteCriticalSection((PCRITICAL_SECTION)handle);
					// Освобождаем указатель.
					aligned_free(handle);
					handle = nullptr;
				}
			}
			// Заблокирует  участок кода, для других потоков.
			// Внимание, последующие вызовы приводят к неочевидной
			// реакции, чаще CTD.
			// Один раз заблокировали, далее снимайте, не пригодно,
			// для объектной реализации.
			// Для Windows это не касается, работает отлично.
			void simple_lock::lock() const noexcept
			{
				if (handle)
					EnterCriticalSection((PCRITICAL_SECTION)handle);
			}
			// Разблокирует участок кода, для других потоков.
			void simple_lock::unlock() const noexcept
			{
				if (handle)
					LeaveCriticalSection((PCRITICAL_SECTION)handle);
			}
			// Возвращает если есть возможность заблокировать.
			bool simple_lock::try_lock() const noexcept
			{
				if (handle)
					return TryEnterCriticalSection((PCRITICAL_SECTION)handle);
				return false;
			}
#else
			// Конструктор по умолчанию.
			simple_lock::simple_lock() noexcept
			{
				std::recursive_mutex* section = 
					(std::recursive_mutex*)aligned_malloc(sizeof(std::recursive_mutex), 0x10);
				if (section)
				{
					new(section) std::recursive_mutex();
					handle = (void*)section;
				}
			}
			// Деструктор.
			simple_lock::~simple_lock() noexcept
			{
				if (handle)
				{
					((std::recursive_mutex*)handle)->~recursive_mutex();
					// Освобождаем указатель.
					aligned_free(handle);
					handle = nullptr;
				}
			}
			// Заблокирует  участок кода, для других потоков.
			// Внимание, последующие вызовы приводят к неочевидной
			// реакции, чаще CTD.
			// Один раз заблокировали, далее снимайте, не пригодно,
			// для объектной реализации.
			// Для Windows это не касается, работает отлично.
			void simple_lock::lock() const noexcept
			{
				if (handle)
					((std::recursive_mutex*)handle)->lock();
			}
			// Разблокирует участок кода, для других потоков.
			void simple_lock::unlock() const noexcept
			{
				if (handle)
					((std::recursive_mutex*)handle)->unlock();
			}
			// Возвращает если есть возможность заблокировать.
			bool simple_lock::try_lock() const noexcept
			{
				if (handle)
					return ((std::recursive_mutex*)handle)->try_lock();
				return false;
			}
#endif
			
			simple_scope_lock::simple_scope_lock(const simple_lock& ob) noexcept :
				_handle(const_cast<simple_lock*>(&ob))
			{
				_handle->lock();
			}
			
			simple_scope_lock::~simple_scope_lock() noexcept
			{
				_handle->unlock();
			}

			simple_scope_try_lock::simple_scope_try_lock(const simple_lock& ob) noexcept :
				_handle(const_cast<simple_lock*>(&ob))
			{}

			simple_scope_try_lock::~simple_scope_try_lock()
			{
				if (_locked)
				{
					_locked = false;
					_handle->unlock();
				}
			}

			bool simple_scope_try_lock::try_lock() const noexcept
			{
				return const_cast<simple_scope_try_lock*>(this)->_locked = _handle->try_lock();
			}
}
	}
}

#if defined(_MSC_VER)
#	pragma warning(default : 26110)
#endif