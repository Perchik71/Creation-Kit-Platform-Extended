// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

namespace voltek
{
	namespace core
	{
		namespace _internal
		{
			// Класс блокировки для других потоков.
			// Защищённый код, гарантировано, выполняется одним потоком.
			// Поддержка рекурсивной блокировки.
			// Для Windows используется критическая секция, а для остальных std::recursive_mutex.
			// Используется свой malloc.
			class simple_lock
			{
			public:
				// Конструктор по умолчанию.
				simple_lock() noexcept;
				// Деструктор.
				virtual ~simple_lock() noexcept;
				// Заблокирует  участок кода, для других потоков.
				// Внимание, последующие вызовы приводят к неочевидной
				// реакции, чаще CTD.
				// Один раз заблокировали, далее снимайте, не пригодно,
				// для объектной реализации.
				// Для Windows это не касается, работает отлично.
				void lock() const noexcept;
				// Разблокирует участок кода, для других потоков.
				void unlock() const noexcept;
				// Возвращает если есть возможность заблокировать и уже заблокирует для текущего потока.
				[[nodiscard]] bool try_lock() const noexcept;
			private:
				inline simple_lock(const simple_lock&) = delete;
				inline simple_lock(simple_lock&&) = delete;
				inline simple_lock& operator=(const simple_lock&) = delete;
				inline simple_lock& operator=(simple_lock&&) = delete;
			private:
				// Указатель на мьютекс или что-то другое.
				void* handle{ nullptr };
			};

			// Класс блокировки для других потоков.
			// Класс удобно использовать внутри методов класса.
			// Конструктор из simple_lock позволяет сразу же установить блокировку.
			// Деструктор блокировку снимет.
			class simple_scope_lock
			{
			public:
				// Конструктор из simple_lock.
				simple_scope_lock(const simple_lock& ob) noexcept;
				// Деструктор.
				virtual ~simple_scope_lock() noexcept;
			private:
				simple_scope_lock(const simple_scope_lock&) = delete;
				simple_scope_lock(simple_scope_lock&&) = delete;
				simple_scope_lock& operator=(const simple_scope_lock&) = delete;
				simple_scope_lock& operator=(simple_scope_lock&&) = delete;
			private:
				// Указатель на простой блокировщик
				simple_lock* _handle{ nullptr };
			};

			// Класс блокировки для других потоков.
			// Класс удобно использовать внутри методов класса.
			// Конструктор из simple_lock позволяет сразу же установить блокировку, если это возможно.
			// Деструктор блокировку снимет.
			class simple_scope_try_lock
			{
			public:
				// Конструктор из simple_lock.
				simple_scope_try_lock(const simple_lock& ob) noexcept;
				// Деструктор.
				virtual ~simple_scope_try_lock() noexcept;
				// Возвращает если есть возможность заблокировать и уже заблокирует для текущего потока. 
				[[nodiscard]] bool try_lock() const noexcept;
			private:
				simple_scope_try_lock(const simple_scope_try_lock&) = delete;
				simple_scope_try_lock(simple_scope_try_lock&&) = delete;
				simple_scope_try_lock& operator=(const simple_scope_try_lock&) = delete;
				simple_scope_try_lock& operator=(simple_scope_try_lock&&) = delete;
			private:
				// Указатель на простой блокировщик
				simple_lock* _handle{ nullptr };
				bool _locked{ false };
			};
		}
	}
}