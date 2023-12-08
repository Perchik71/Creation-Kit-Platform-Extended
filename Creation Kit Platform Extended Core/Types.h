// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	template<typename _Ty>
	using BaseString = std::basic_string<_Ty, std::char_traits<_Ty>, voltek::allocator<_Ty>>;
	using String = BaseString<char>;
	using WideString = BaseString<wchar_t>;

	template<typename _Ty>
	using Array = std::vector<_Ty, voltek::allocator<_Ty>>;

	template<typename _Ty>
	using ConcurrencyArray = concurrency::concurrent_vector<_Ty, voltek::allocator<_Ty>>;

	template<typename _kTy, typename _Ty, typename _Pr = std::less<_kTy>>
	using Map = std::map<_kTy, _Ty, _Pr, voltek::allocator<std::pair<const _kTy, _Ty>>>;

	template<typename _kTy, typename _Ty, typename _Hasher = std::hash<_kTy>,
		typename _Equal = std::equal_to<_kTy>>
	using UnorderedMap = std::unordered_map<_kTy, _Ty, _Hasher, _Equal, 
		voltek::allocator<std::pair<const _kTy, _Ty>>>;

	template<typename _kTy, typename _Ty, typename _Hasher = std::hash<_kTy>, 
		typename _Equal = std::equal_to<_kTy>>
	using ConcurrencyMap = concurrency::concurrent_unordered_map<_kTy, _Ty, 
		_Hasher, _Equal, voltek::allocator<std::pair<const _kTy, _Ty>>>;

	template<typename _kTy, typename _Pr = std::less<_kTy>>
	using Set = std::set<_kTy, _Pr, voltek::allocator<_kTy>>;

	template<typename _kTy, typename _Hasher = std::hash<_kTy>, 
		typename _Equal = std::equal_to<_kTy>>
	using UnorderedSet = std::unordered_set<_kTy, _Hasher, _Equal, voltek::allocator<_kTy>>;

	template<typename _Ty>
	class SmartPointerInfo
	{
	public:
		SmartPointerInfo() : _refCount(1), _ptr(nullptr), _is_Array(false) {}
		SmartPointerInfo(const _Ty* ptr, bool is_Array = false) : _refCount(1),
			_ptr(const_cast<_Ty*>(ptr)), _is_Array(is_Array) {}
		~SmartPointerInfo() 
		{
			if (!_refCount && _ptr)
			{
				if (_is_Array)
					delete[] _ptr;
				else
					delete _ptr;
			}
		}

		inline bool operator==(const SmartPointerInfo& info) { return _ptr == info._ptr; }
		inline bool operator!=(const SmartPointerInfo& info) { return _ptr != info._ptr; }

		inline void IncRef() { _refCount++; }
		inline void DecRef() { if (_refCount > 0) _refCount--; }

		inline _Ty* Get() { return _ptr; }
		inline const _Ty* Get() const { return _ptr; }

		inline _Ty* operator->() { return Get(); }
		inline const _Ty* operator->() const { return Get(); }
		inline _Ty* operator*() { return Get(); }
		inline const _Ty* operator*() const { return Get(); }

		inline uint32_t GetRefCount() const { return _refCount; }
		inline bool Empty() const { return !_ptr; }
	private:
		SmartPointerInfo(const SmartPointerInfo& info) = default;
		SmartPointerInfo& operator=(const SmartPointerInfo& info) = default;
	private:
		_Ty* _ptr;
		uint32_t _refCount;
		bool _is_Array;
	};

	template<typename _Ty>
	class SmartPointer
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
		inline _Ty* operator*() { return Get(); }
		inline const _Ty* operator*() const { return Get(); }

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