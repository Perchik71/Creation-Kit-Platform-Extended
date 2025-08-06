// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <type_traits>

namespace CKPE
{
	template<typename _Ty>
	class CKPE_API SmartPointerInfo
	{
	public:
		SmartPointerInfo() noexcept(true) : _refCount(1), _ptr(nullptr), _is_Array(false) {}
		SmartPointerInfo(const _Ty* ptr, bool is_Array = false) noexcept(true) : _refCount(1),
			_ptr(const_cast<_Ty*>(ptr)), _is_Array(is_Array) {}
		~SmartPointerInfo() noexcept(true)
		{
			if (!_refCount && _ptr)
			{
				if (_is_Array)
					delete[] _ptr;
				else
					delete _ptr;
			}
		}

		inline bool operator==(const SmartPointerInfo& info) noexcept(true) { return _ptr == info._ptr; }
		inline bool operator!=(const SmartPointerInfo& info) noexcept(true) { return _ptr != info._ptr; }

		inline void IncRef() noexcept(true) { _refCount++; }
		inline void DecRef() noexcept(true) { if (_refCount > 0) _refCount--; }

		[[nodiscard]] inline _Ty* Get() noexcept(true) { return _ptr; }
		[[nodiscard]] inline const _Ty* Get() const noexcept(true) { return _ptr; }

		inline _Ty* operator->() noexcept(true) { return Get(); }
		inline const _Ty* operator->() const noexcept(true) { return Get(); }
		inline _Ty& operator*() noexcept(true) { return *Get(); }
		inline const _Ty& operator*() const noexcept(true) { return *Get(); }

		[[nodiscard]] inline uint32_t GetRefCount() const noexcept(true) { return _refCount; }
		[[nodiscard]] inline bool Empty() const noexcept(true) { return !_ptr; }
	private:
		SmartPointerInfo(const SmartPointerInfo& info) = delete;
		SmartPointerInfo& operator=(const SmartPointerInfo& info) = delete;
	private:
		_Ty* _ptr;
		uint32_t _refCount;
		bool _is_Array;
	};

	template<typename _Ty>
	class CKPE_API SmartPointer
	{
	public:
		SmartPointer() : _info(new SmartPointerInfo<_Ty>()) {}
		SmartPointer(const _Ty* ptr, bool is_Array = false) : _info(new SmartPointerInfo<_Ty>(ptr, is_Array)) {}
		SmartPointer(const SmartPointer& smart_ptr) : _info(smart_ptr._info) { _info->IncRef(); }
		SmartPointer& operator=(const SmartPointer& smart_ptr)
		{
			if (!_info->Empty())
				_info->DecRef();

			if (!_info->GetRefCount())
				delete _info;

			_info = smart_ptr._info;
			_info->IncRef();
			return *this;
		}
		virtual ~SmartPointer()
		{
			_info->DecRef();

			if (!_info->GetRefCount())
				delete _info;
		}

		inline _Ty* Get() { return _info->Get(); }
		inline const _Ty* Get() const { return _info->Get(); }

		inline _Ty* operator->() { return Get(); }
		inline const _Ty* operator->() const { return Get(); }
		inline _Ty& operator*() { return *Get(); }
		inline const _Ty& operator*() const { return *Get(); }

		inline uint32_t GetRefCount() const { return _info->GetRefCount(); }
		inline bool Empty() const { return _info->Empty(); }
	private:
		SmartPointerInfo<_Ty>* _info;
	};

	template<typename _Ty, class... _Types, std::enable_if_t<!std::is_array_v<_Ty>, int> = 0>
	SmartPointer<_Ty> MakeSmartPointer(_Types&&... _Args) noexcept
	{
		return SmartPointer<_Ty>(new _Ty(std::forward<_Types>(_Args)...), false);
	}

	template<typename _Ty, std::enable_if_t<std::is_array_v<_Ty>&& std::extent_v<_Ty> == 0, int> = 0>
	SmartPointer<_Ty> MakeSmartPointer(const size_t _Size) noexcept
	{
		using _Elem = std::remove_extent_t<_Ty>;
		return SmartPointer<_Ty>(new _Elem[_Size], true);
	}
}