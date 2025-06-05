// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include "vmapper.h"
#include "vassert.h"

#if (defined(_WIN32) || defined(_WIN64))	
#	pragma warning (disable : 6250)
#	pragma warning (disable : 6333)
#	pragma warning (disable : 28160)
#	include <windows.h>
#endif

#include <iostream>

namespace voltek
{
	namespace core
	{
		mapper::mapper() : _mem(nullptr), _size(0), _freesize(0), _blocksize(0), _mask(nullptr)
		{
			resize(DEFAULT_SIZE, DEFAULT_BLOCKSIZE);
		}

		mapper::mapper(size_t size, size_t blocksize) : _mem(nullptr), _size(0), _freesize(0), _blocksize(0), _mask(nullptr)
		{
			resize(size * 1024 * 1024, blocksize);
		}

		mapper::mapper(const bits& ob) : _mem(nullptr), _size(0), _freesize(0), _blocksize(0), _mask(nullptr)
		{
			*this = ob;
		}

		mapper::~mapper()
		{
			clear();
		}

		mapper& mapper::operator=(const mapper& ob)
		{
			clear();
			resize(ob._size, ob._blocksize);
			if (!empty())
			{
				memcpy(_mem, ob._mem, ob._size);
				memcpy(_mask->data(), ob._mask->c_data(), ob._mask->size());
				_freesize = ob._freesize;
			}
			return *this;
		}

		void mapper::resize(size_t size, size_t blocksize)
		{
			if (_mem && (size > 0)) return;

			if (size)
			{
				_mem = (char*)VirtualAlloc(NULL, (SIZE_T)size, MEM_RESERVE, PAGE_READWRITE);
				if (_mem)
				{
					_size = size;
					_freesize = _size;
					_blocksize = blocksize;
					_mask = new bits(count_blocks());
					_vassert(_mask);
					_mask->all_set();
				}
			}
			else if (_mem)
			{
				VirtualFree(_mem, _size, MEM_RELEASE);
				_mem = nullptr;
				_size = 0;
				_freesize = 0;
				_blocksize = 0;
				if (_mask)
				{
					delete _mask;
					_mask = nullptr;
				}
			}
		}

		void* mapper::block_alloc()
		{
			if (!_freesize) 
				return nullptr;

			size_t id = 0;
			if (!_mask->find_first_set_bit(id))
				return nullptr;

			auto ret = VirtualAlloc(_mem + (id * _blocksize), _blocksize, MEM_COMMIT, PAGE_READWRITE);
			if (ret)
			{
				_mask->unset(id);
				_freesize -= _blocksize;
				return ret;
			}

			return nullptr;
		}

		bool mapper::block_free(const void* ptr)
		{
			if (!is_valid_ptr(ptr))
				return false;
			
			auto id = (size_t)((char*)ptr - _mem) / _blocksize;
			if (VirtualFree(const_cast<void*>(ptr), _blocksize, MEM_DECOMMIT))
			{
				_mask->set(id);
				_freesize += _blocksize;
				return true;
			}

			return false;
		}

		bool mapper::is_valid_ptr(const void* ptr) const
		{
			return (((char*)ptr >= _mem) && ((char*)ptr < (_mem + _size)));
		}
	}
}
