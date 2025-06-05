// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#if defined(_MSC_VER)
#pragma warning(disable : 26495)
#endif

#include "vbits.h"
#include "vassert.h"
#include "valloc.h"
#include "vintrin.h"
#include <string.h>
#include "vio.h"

#ifndef VMMDLL_EXPORTS
#	include <stdio.h>
#endif

#include <bit>

namespace voltek
{
	namespace core
	{
		// Конструктор по умолчанию.
		bits::bits() : base(), _mem(nullptr), _count(0), _sets(0)
		{
			//initialize();
		}
		// Конструктор.
		// В качестве параметра указывается кол-во желаемых битов.
		bits::bits(size_t count) : base(), _mem(nullptr), _count(0), _sets(0)
		{
			//initialize();
			resize(count);
		}
		// Конструктор копий.
		bits::bits(const bits& ob) : base(), _mem(nullptr), _count(0), _sets(0)
		{
			//initialize();
			// Типо вызов присвоения
			*this = ob;
		}
		// Деструктор.
		bits::~bits() 
		{ 
			clear();
		}
		// Оператор присвоения
		bits& bits::operator=(const bits& ob)
		{
			resize(ob._count);
			if (_mem)
			{
				memcpy(_mem, ob._mem, size());
				_sets = ob._sets;
			}
			return *this;
		}
		// Установить все биты равно 1.
		inline void bits::all_set() 
		{ 
			if (_mem)
			{
				memset(_mem, -1, size());
				_sets = _count;
			}
		}
		// Установить все биты равно 0.
		inline void bits::all_unset() 
		{ 
			if (_mem)
			{
				memset(_mem, 0, size());
				_sets = 0;
			}
		}
		// Изменяет кол-во допустимых битов.
		// В качестве параметра указывается кол-во желаемых битов.
		// Если указать 0, то будет освобождена память
		// При уменьшении объёма вызовет перерасчёт установленных битов, это очень медленно.
		void bits::resize(size_t count)
		{
			if (count > 0)
			{
				size_t need_size = count >> 3;
				// Проверка кратности кол-ва к 8 (в байте 8 бит, а память выделяется только в байтах)
				if ((need_size << 3) != count)
					need_size++;

				if (_mem)
				{
					size_t old_count = _count;
					_mem = (char*)_internal::aligned_recalloc(_mem, need_size, 1, 0x10);
					if (_mem && (old_count > count))
						// Ситуация неопределённая, требуется перерасчёт всех битов
						update_sets();
				}
				else
					_mem = (char*)_internal::aligned_calloc(need_size, 1, 0x10);

				_vassert(_mem != nullptr);

				if (_mem)
					_count = count;
			}
			else
			{
				if (_mem)
				{
					_internal::aligned_free(_mem);
					_mem = nullptr;
					_count = 0;
					_sets = 0;
				}
			}
		}
		// Возвращает размер выделенной памяти под этот объект класса.
		size_t bits::size() const
		{ 
			return _internal::aligned_msize(_mem); 
		}
		// Устанавливает бит за заданным индексом "bit_index" в 0, если он был равен 1.
		// Возвращает истину, если операция прошла успешно.
		// Успех будет только в том случаи, если функция "is_set" вернёт истину, что
		// значит бит был равен 1.
		bool bits::unset(size_t bit_index)
		{
			_vassert(_count > bit_index);
			if (is_set(bit_index))
			{
				_vassert(_sets > 0);
				_mem[index_from_bit_index(bit_index)] &= ~((char)1 << (bit_index - ((bit_index >> 3) << 3)));
				_sets--;
				return true;
			}

			return false;
		}
		// Устанавливает бит за заданным индексом "bit_index" в 1, если он был равен 0.
		// Возвращает истину, если операция прошла успешно.
		// Успех будет только в том случаи, если функция "is_unset" вернёт истину, что
		// значит бит был равен 0.
		bool bits::set(size_t bit_index)
		{
			_vassert(_count > bit_index);
			if (is_unset(bit_index))
			{
				_vassert(_sets != _count);
				_mem[index_from_bit_index(bit_index)] |= ((char)1 << (bit_index - ((bit_index >> 3) << 3)));
				_sets++;
				return true;
			}

			return false;
		}

		// Перерасчёт установленных битов.
		// Очень неоптимизированный способ восстановления кол-ва установленных бит.
		// Не рекомендуется.
		void bits::update_sets()
		{
			_sets = 0;
			for (size_t i = 0; i < _count; i++)
			{
				if (is_set(i)) _sets++;
			}
		}

		// Вывод дампа битовой карты
		void bits::dump(const char* file_name) const
		{
#ifndef VMMDLL_EXPORTS
			_internal::memory_to_file(file_name, (void*)_mem, size());
#endif //!VMMDLL_EXPORTS
		}

		// Поиск первого установленного бита в памяти
		// Возвращает истину, тогда "index" содержит индекс бита, который установлен как 1.
		// Если ложь, то значит все биты установлены как 0.
		bool bits::find_first_set_bit(size_t& index) const
		{
			if (is_all_unsets())
				return false;

			if (_count >= 32)
			{
				if (_count >= 2048)
				{
					if (avx2_supported)
						// AVX2
						return find_first_set_bit_avx2(index);
					if (sse41_supported)
						// SSE4.1
						return find_first_set_bit_sse41(index);
					else
						return find_first_set_bit_none(index);
				}

				uint32_t* u32p = (uint32_t*)_mem;
				size_t cnt = (_count >> 5) << 5;
				size_t end_cnt = cnt >> 5;

				for (size_t i = 0; i < end_cnt; i++)
				{
					// 0 - ничего нет
					if (!u32p[i]) continue;

					index = (i << 5) + voltek::ctzl(u32p[i]);
					goto quit_label_find_bit;
				}

				for (size_t i = cnt; i < _count; i++)
				{
					if (is_set(i))
					{
						index = i;
						break;
					}
				}
			}
			else
			{
				// Самое простое и медленное, но на малых данных самое то.

				for (size_t i = 0; i < _count; i++)
				{
					if (is_set(i))
					{
						index = i;
						break;
					}
				}
			}
		quit_label_find_bit:
			return true;
		}

		// Поиск первого установленного бита AVX2 инструкциями
		bool bits::find_first_set_bit_avx2(size_t& index) const
		{
			uint64_t* u64p = (uint64_t*)_mem;
			size_t cnt = (_count >> 6) << 6;
			size_t end_cnt = cnt >> 6;

			int mask = 0;

			__m256i zero = { 0 };	// Всратая студия хочет инициализации локальной переменной, окей
			zero = _mm256_xor_si256(zero, zero);

			// Сравниваем по 2048 бит или 256 байт за итерацию
			for (size_t i = 0; end_cnt > i; i += 32)
			{
				__m256i mask1 = _mm256_cmpeq_epi64(_mm256_loadu_si256((__m256i*)&u64p[i]), zero);
				__m256i mask2 = _mm256_cmpeq_epi64(_mm256_loadu_si256((__m256i*)&u64p[i + 4]), zero);
				__m256i mask3 = _mm256_cmpeq_epi64(_mm256_loadu_si256((__m256i*)&u64p[i + 8]), zero);
				__m256i mask4 = _mm256_cmpeq_epi64(_mm256_loadu_si256((__m256i*)&u64p[i + 12]), zero);

				mask  = _mm256_movemask_pd(*(reinterpret_cast<__m256d*>(&mask1)));
				mask |= _mm256_movemask_pd(*(reinterpret_cast<__m256d*>(&mask2))) << 4;
				mask |= _mm256_movemask_pd(*(reinterpret_cast<__m256d*>(&mask3))) << 8;
				mask |= _mm256_movemask_pd(*(reinterpret_cast<__m256d*>(&mask4))) << 12;

				// Для производительности
				// Если маска != 65535, то значит в первых 1024 битах есть поднятый бит
				// Оказывается это даёт буст в 10%, скорее всего, загрузка дополнительных данных
				// существенно затратно по времени.
				if (mask == 65535)
				{
					__m256i mask5 = _mm256_cmpeq_epi64(_mm256_loadu_si256((__m256i*)&u64p[i + 16]), zero);
					__m256i mask6 = _mm256_cmpeq_epi64(_mm256_loadu_si256((__m256i*)&u64p[i + 20]), zero);
					__m256i mask7 = _mm256_cmpeq_epi64(_mm256_loadu_si256((__m256i*)&u64p[i + 24]), zero);
					__m256i mask8 = _mm256_cmpeq_epi64(_mm256_loadu_si256((__m256i*)&u64p[i + 28]), zero);

					mask |= _mm256_movemask_pd(*(reinterpret_cast<__m256d*>(&mask5))) << 16;
					mask |= _mm256_movemask_pd(*(reinterpret_cast<__m256d*>(&mask6))) << 20;
					mask |= _mm256_movemask_pd(*(reinterpret_cast<__m256d*>(&mask7))) << 24;
					mask |= _mm256_movemask_pd(*(reinterpret_cast<__m256d*>(&mask8))) << 28;
				}

				mask = ~mask;
				// 0 - ничего нет
				if (!mask) continue;

				size_t array_index = (size_t)i + voltek::ctzl(mask);
				index = (array_index << 6) + voltek::ctzll((unsigned long long)(u64p[array_index]));

				// Сбросить, чтобы не было пиналити
				_mm256_zeroupper();

				return true;
			}

			for (size_t i = cnt; i < _count; i++)
			{
				if (is_set(i))
				{
					index = i;
					return true;
				}
			}

			return false;
		}

		// Поиск первого установленного бита SSE4.1 инструкциями
		bool bits::find_first_set_bit_sse41(size_t& index) const
		{
			uint64_t* u64p = (uint64_t*)_mem;
			size_t cnt = (_count >> 6) << 6;
			size_t end_cnt = cnt >> 6;
			int mask = 0;

			__m128i zero = { 0 };	// Всратая студия хочет инициализации локальной переменной, окей
			zero = _mm_xor_si128(zero, zero);

			// Сравниваем по 1024 бит или 128 байт за итерацию
			for (size_t i = 0; end_cnt > i; i += 16)
			{
				__m128i mask1 = _mm_cmpeq_epi64(_mm_loadu_si128((__m128i*)&u64p[i]), zero);
				__m128i mask2 = _mm_cmpeq_epi64(_mm_loadu_si128((__m128i*)&u64p[i + 2]), zero);
				__m128i mask3 = _mm_cmpeq_epi64(_mm_loadu_si128((__m128i*)&u64p[i + 4]), zero);
				__m128i mask4 = _mm_cmpeq_epi64(_mm_loadu_si128((__m128i*)&u64p[i + 6]), zero);

				mask = _mm_movemask_pd(*(reinterpret_cast<__m128d*>(&mask1)));
				mask |= _mm_movemask_pd(*(reinterpret_cast<__m128d*>(&mask2))) << 2;
				mask |= _mm_movemask_pd(*(reinterpret_cast<__m128d*>(&mask3))) << 4;
				mask |= _mm_movemask_pd(*(reinterpret_cast<__m128d*>(&mask4))) << 6;

				// Для производительности
				// Если маска != 255, то значит в первых 512 битах есть поднятый бит
				// Оказывается это даёт буст в 7%, скорее всего, загрузка дополнительных данных
				// существенно затратно по времени.
				if (mask == 255)
				{
					__m128i mask5 = _mm_cmpeq_epi64(_mm_loadu_si128((__m128i*)&u64p[i + 8]), zero);
					__m128i mask6 = _mm_cmpeq_epi64(_mm_loadu_si128((__m128i*)&u64p[i + 10]), zero);
					__m128i mask7 = _mm_cmpeq_epi64(_mm_loadu_si128((__m128i*)&u64p[i + 12]), zero);
					__m128i mask8 = _mm_cmpeq_epi64(_mm_loadu_si128((__m128i*)&u64p[i + 14]), zero);

					mask |= _mm_movemask_pd(*(reinterpret_cast<__m128d*>(&mask5))) << 8;
					mask |= _mm_movemask_pd(*(reinterpret_cast<__m128d*>(&mask6))) << 10;
					mask |= _mm_movemask_pd(*(reinterpret_cast<__m128d*>(&mask7))) << 12;
					mask |= _mm_movemask_pd(*(reinterpret_cast<__m128d*>(&mask8))) << 14;
				}

				mask = ~mask & 65535;
				// 0 - ничего нет
				if (!mask) continue;

				size_t array_index = i + voltek::ctzl(mask);
				index = (array_index << 6) + voltek::ctzll(u64p[array_index]);
				return true;
			}

			for (size_t i = cnt; i < _count; i++)
			{
				if (is_set(i))
				{
					index = i;
					return true;
				}
			}

			return false;
		}

		bool bits::find_first_set_bit_none(size_t& index, size_t start) const
		{
			uint64_t* u64p = (uint64_t*)_mem;
			size_t cnt = (_count >> 6) << 6;
			size_t end_cnt = cnt >> 6;

			for (size_t i = start; end_cnt > i; i++)
			{
				size_t search_index = voltek::ctzll(u64p[i]);
				if (search_index != 64)
				{
					index = (i << 6) + search_index;
					return true;
				}
			}

			return false;
		}

		/////////////////////////////////////

		// Конструктор по умолчанию.
		bits_regions::bits_regions() : base(), _region_map(0), _count(0), _sets(0), _distance(0)
		{}
		// Конструктор.
		// В качестве параметра указывается кол-во желаемых битов.
		bits_regions::bits_regions(size_t count) : base(), _region_map(0), _count(0), _sets(0), _distance(0)
		{
			resize(count);
		}
		// Конструктор копий.
		bits_regions::bits_regions(const bits_regions& ob) : base(), _region_map(0), _count(0), _sets(0), _distance(0)
		{
			*this = ob;
		}
		// Деструктор.
		bits_regions::~bits_regions()
		{
			clear();
		}
		// Оператор присвоения
		bits_regions& bits_regions::operator=(const bits_regions& ob)
		{
			for (size_t i = 0; i < 16; i++)
				_region_bits[i] = ob._region_bits[i];

			memcpy(_regions, ob._regions, sizeof(region) << 4);
			_region_map = ob._region_map;
			_count = ob._count;
			_sets = ob._sets;
			_distance = ob._distance;

			return *this;
		}
		// Изменяет кол-во допустимых битов.
		// В качестве параметра указывается кол-во желаемых битов.
		// Если указать 0, то будет освобождена память
		// При уменьшении объёма вызовет перерасчёт установленных битов, это очень медленно.
		// Меньше 65536 нельзя.
		void bits_regions::resize(size_t count)
		{
			if (count < 65536)
			{
				_vassert(count < 65536);
				return;
			}

			if (!count)
			{
				for (size_t i = 0; i < 16; i++)
					_region_bits[i].clear();

				memset(_regions, 0, sizeof(region) << 4);
				_distance = 0;
				_region_map = 0;
				_count = 0;
				_sets = 0;
				return;
			}

			if (_count)
			{
				// TODO: не реализовано
				return;
			}

			// Делим на 16 масок
			_distance = count >> 4;
			// Настроим кол-во регионов (фактически делим огромное на 16 секций)
			for (size_t i = 0; i < 16; i++)
			{
				_regions[i].start = _distance * i;
				_regions[i].end = _distance * (i + 1);
			}
			_regions[15].end = count;
			// Все регионы обнулены
			_region_map = 0;
			// Инициализация битовых масок
			for (size_t i = 0; i < 16; i++)
				_region_bits[i].resize(_regions[i].end - _regions[i].start);
			// Кол-во допустимых битов
			_count = count;
			_sets = 0;
		}
		// Возвращает размер выделенной памяти под этот объект класса.
		size_t bits_regions::size() const
		{
			size_t ret = 0;
			for (size_t i = 0; i < 16; i++)
				ret += _region_bits[i].size();
			return ret;
		}
		// Установить все биты равно 1.
		void bits_regions::all_set()
		{
			for (size_t i = 0; i < 16; i++)
				_region_bits[i].all_set();
			_sets = _count;
			_region_map = (uint16_t)-1;
		}
		// Установить все биты равно 0.
		void bits_regions::all_unset()
		{
			for (size_t i = 0; i < 16; i++)
				_region_bits[i].all_unset();
			_sets = 0;
			_region_map = 0;
		}
		// Возвращает истину, если бит за заданным индексом "bit_index" бит равен 1.
		bool bits_regions::is_set(size_t bit_index) const
		{
			_vassert(_count > bit_index);
			size_t region_id = bit_index / _distance;
			return _region_bits[region_id].is_set(bit_index - _regions[region_id].start);
		}
		// Устанавливает бит за заданным индексом "bit_index" в 0, если он был равен 1.
		// Возвращает истину, если операция прошла успешно.
		// Успех будет только в том случаи, если функция "is_set" вернёт истину, что
		// значит бит был равен 1.
		bool bits_regions::unset(size_t bit_index)
		{
			_vassert(_count > bit_index);
			size_t region_id = bit_index / _distance;
			bool ret = _region_bits[region_id].unset(bit_index - _regions[region_id].start);
			if (ret)
			{
				_vassert(_sets > 0);
				if (_region_bits[region_id].is_all_unsets())
					_region_map &= ~((region_id > 0) ? (uint16_t)1 << region_id : (uint16_t)1);
				_sets--;
			}
			return ret;
		}
		// Устанавливает бит за заданным индексом "bit_index" в 1, если он был равен 0.
		// Возвращает истину, если операция прошла успешно.
		// Успех будет только в том случаи, если функция "is_unset" вернёт истину, что
		// значит бит был равен 0.
		bool bits_regions::set(size_t bit_index)
		{
			_vassert(_count > bit_index);
			size_t region_id = bit_index / _distance;
			bool ret = _region_bits[region_id].set(bit_index - _regions[region_id].start);
			if (ret)
			{
				_vassert(_sets != _count);
				_region_map |= ((region_id > 0) ? (uint16_t)1 << region_id : (uint16_t)1);
				_sets++;
			}
			return ret;
		}
		// Вывод дампа битовой карты
		void bits_regions::dump(const char* file_name) const
		{
#ifndef VMMDLL_EXPORTS
			if (!_count) return;
			void* f = _internal::open_file(file_name, "wb");
			if (!f) return;

			// Проходим по всем кешам
			for (size_t i = 0; i < 16; i++)
			{
				size_t size = _region_bits[i].size();
				const char* data = (const char*)_region_bits[i].c_data();

				// Записываем в файл по 4096 байт за итерацию

				size_t wsize = 4096 > size ? size : 4096;
				size_t write = 0;

				while (write < size)
				{
					_internal::write_file(f, (const void*)data, write, wsize);
					write += wsize;
					wsize = 4096 > (size - write) ? size - write : 4096;
				}
			}

			// Закрытие файла
			_internal::close_file(f);
#endif // !VMMDLL_EXPORTS
		}
		// Поиск первого установленного бита в памяти
		// Возвращает истину, тогда "index" содержит индекс бита, который установлен как 1.
		// Если ложь, то значит все биты установлены как 0.
		bool bits_regions::find_first_set_bit(size_t& index) const
		{
			if (is_all_unsets())
				return false;

			size_t region_id = (size_t)voltek::ctzll((unsigned long)_region_map);
			_vassert(!_region_bits[region_id].is_all_unsets());
			
			bool ret = _region_bits[region_id].find_first_set_bit(index);
			if (ret) index = _regions[region_id].start + index;
			return ret;
		}
	}
}

#if defined(_MSC_VER)
#pragma warning(default : 26495)
#pragma warning(default : 4996)
#endif