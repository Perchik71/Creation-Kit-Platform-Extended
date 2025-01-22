// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "vmmconfig.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

namespace voltek
{
	// Инициализация менеджера памяти.
	VOLTEK_MM_API void scalable_memory_manager_initialize();
	// Освобождение менеджера памяти.
	// Чисто символически, использовать её не рекомендуется.
	// Но если очень хочется, почему бы и нет.
	VOLTEK_MM_API void scalable_memory_manager_shutdown();
	// Выделение памяти нужного размера.
	// При ошибке вернёт nullptr, это если size равен более 4 гб.
	// Также вернёт nullptr если память физически кончилась.
	// Вернёт постоянный адрес при затребовании памяти равной 0.
	// Память всегда выровнена.
	VOLTEK_MM_API void* scalable_alloc(size_t size);
	// Выделение памяти нужного размера.
	// При ошибке вернёт nullptr, это если size равен более 4 гб.
	// Также вернёт nullptr если память физически кончилась.
	// Вернёт постоянный адрес при затребовании памяти равной 0.
	// Память всегда выровнена и обнулена.
	VOLTEK_MM_API void* scalable_calloc(size_t count, size_t size);
	// Выделение памяти нужного размера из прошлого указателя на память.
	// При ошибке вернёт nullptr, это если size равен 0 или более 4 гб.
	// Также вернёт nullptr если память физически кончилась.
	// Память всегда выровнена. Адрес памяти может быть изменён.
	VOLTEK_MM_API void* scalable_realloc(const void* ptr, size_t size);
	// Выделение памяти нужного размера из прошлого указателя на память.
	// При ошибке вернёт nullptr, это если size равен 0 или более 4 гб.
	// Также вернёт nullptr если память физически кончилась.
	// Память всегда выровнена. Адрес памяти может быть изменён.
	// Новый участок памяти всегда обнулён.
	VOLTEK_MM_API void* scalable_recalloc(const void* ptr, size_t count, size_t size);
	// Освобождает память выделенную под указатель.
	// Вернёт ложь, если произошла ошибка.
	VOLTEK_MM_API bool scalable_free(const void* ptr);
	// Возвращает размер памяти выделенной под указатель.
	// Вернёт 0 при ошибке, что значит, указатель на память не пренадлежит менеджеру.
	VOLTEK_MM_API size_t scalable_msize(const void* ptr);
}

#ifdef __cplusplus
} // __cplusplus defined.
#endif

// Только для внешнего использования.
// Сама библиотека это не использует.
#if !defined(VMM_EXPORTS)

#include <new>

namespace voltek
{
	// Шаблонный класс указателя на массив для аллокатора, добавлен в с++23.
	// Впервые стандарт оформлен в странном для себя стиле (вернул обратно).
	template<typename pointer, typename size_type = size_t>
	struct allocation_result {
		pointer ptr;
		size_type count;
	};
	// Шаблонный класс аллокатора, для stl библиотек, приближён к стандарту c++17,
	// возможно, будут проблемы с применением в новом стандарте, но честно, в с++23,
	// от класса мало, что осталось.
	template<class _type>
	struct allocator
	{
	public:
		typedef _type value_type;
		typedef _type* pointer;
		typedef const _type* const_pointer;
		typedef _type& reference;
		typedef const _type& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		// Конструктор по умолчанию.
		constexpr allocator() noexcept = default;
		template<class _other>
		inline constexpr allocator(const allocator<_other>&) noexcept {}
		// Конструктор копий.
		constexpr allocator(const allocator&) noexcept = default;
		// Оператор присвоения.
		//constexpr allocator& operator=(const allocator&) const = default;

		inline bool operator==(const allocator& ob) const { return true; }
		inline bool operator!=(const allocator& ob) const { return false; }

		// Деструктор.
		virtual ~allocator() = default;
		// Функция возвращает указатель на память, заданного размера.
		// Не совсем понимаю, почему сразу необъявить одну функцию как конст,
		// он не изменяет свой объект.
		// В стандарте нет упоминания, что память должна быть обнулена.
		__declspec(allocator) inline pointer allocate(size_type n) const
		{
			pointer new_ptr = (pointer)scalable_alloc(n * sizeof(value_type));
			if (!new_ptr) throw std::bad_alloc();
			return new_ptr;
		}
		// Функция возвращает результат, где есть указатель и кол-во, 
		// заданного размера, добавлен в с++23.
		// Я не совсем вкурил: 
		// Allocates count * sizeof(T) bytes of uninitialized storage, where count 
		// is an unspecified integer value not less than n.
		// Где count, не меньше, чем n. Ох уж эти молодые..., а можно я возведу n в степень 1000?
		// Фантазёры. Я реализую, где n будет подразумеваться как кол-во.
		inline allocation_result<pointer, size_type> allocate_at_least(size_type n) const
		{
			return { allocate(n), n };
		}
		// Функция освобождает память.
		inline void deallocate(const pointer ptr, size_type size) const
		{
			// На size плевать, он нас не интересует.
			// Так же всё равно на заведомо плохой указатель, для
			// Windows встроенна защита SEH.
			scalable_free((void*)ptr);
		}
		// Возвращает максимально возможный размер для одного указателя.
		inline size_type max_size() const
		{
			// 4 Гб.
			return ((size_type)4 * 1024 * 1024 * 1024) / sizeof(_type);
		}
	};
}

#endif // !defined(VMM_EXPORTS)