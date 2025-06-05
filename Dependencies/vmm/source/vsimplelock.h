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
				simple_lock();
				// Деструктор.
				virtual ~simple_lock();
				// Заблокирует  участок кода, для других потоков.
				// Внимание, последующие вызовы приводят к неочевидной
				// реакции, чаще CTD.
				// Один раз заблокировали, далее снимайте, не пригодно,
				// для объектной реализации.
				// Для Windows это не касается, работает отлично.
				void lock() const;
				// Разблокирует участок кода, для других потоков.
				void unlock() const;
				// Возвращает если есть возможность заблокировать.
				bool try_lock() const;
			private:
				// Критическая секция уникальна.
				inline simple_lock(const simple_lock& ob) : handle(nullptr) {}
				// Критическая секция уникальна.
				inline simple_lock& operator=(const simple_lock& ob) {}
			private:
				// Указатель на мьютекс или что-то другое.
				void* handle;
			};

			// Класс блокировки для других потоков.
			// Класс удобно использовать внутри методов класса.
			// Конструктор из simple_lock позволяет сразу же установить блокировку.
			// Деструктор блокировку снимет.
			class simple_scope_lock
			{
			public:
				// Конструктор из simple_lock.
				simple_scope_lock(const simple_lock& ob);
				// Деструктор.
				virtual ~simple_scope_lock();
			private:
				// Уникален.
				simple_scope_lock(const simple_scope_lock& ob);
				// Уникален.
				simple_scope_lock& operator=(const simple_scope_lock& ob);
			private:
				// Указатель на простой блокировщик
				simple_lock* _handle;
			};
		}
	}
}