// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <cstdint>
#include <CKPE.Common.h>
#include <CKPE.Asserts.h>

namespace CKPE
{
	template<typename _Ty>
	class ISingleton
	{
	public:
		ISingleton() noexcept(true) : _singleton(nullptr) {}
		ISingleton(const _Ty* S) noexcept(true) : _singleton(nullptr) 
		{
			static _Ty* _S = const_cast<_Ty*>(S);
			_singleton = (_Ty**)&_S;
		}
		ISingleton(const _Ty** S) noexcept(true) : _singleton(S) {}
		ISingleton(std::uintptr_t addr) noexcept(true) : _singleton((_Ty**)addr) {}
		ISingleton(const ISingleton& S) noexcept(true) : _singleton(S._singleton) {}

		inline void Reset(bool destroy = false) noexcept(true)
		{ 
			if (_singleton)
			{
				if (destroy) delete *_singleton;
				_singleton = nullptr;
			}
		}

		[[nodiscard]] constexpr inline bool Empty() const noexcept(true)
		{
			return !_singleton || !(*_singleton);
		}

		inline ISingleton& operator=(const ISingleton& S) noexcept(true)
		{
			_singleton = S._singleton;
			return *this;
		}

		inline ISingleton& operator=(const _Ty** S) noexcept(true)
		{
			_singleton = const_cast<_Ty**>(S);
			return *this;
		}

		inline ISingleton& operator=(const _Ty* S) noexcept(true)
		{
			static _Ty* _S = const_cast<_Ty*>(S);
			_singleton = (_Ty**)&_S;
			return *this;
		}

		inline ISingleton& operator=(std::uintptr_t addr) noexcept(true)
		{
			_singleton = (_Ty**)addr;
			return *this;
		}
		
		[[nodiscard]] inline _Ty* GetSingleton() const noexcept(true)
		{
			CKPE_ASSERT(_singleton);
			return *_singleton;
		}

		inline _Ty* operator->() noexcept(true) { return GetSingleton(); }
		inline const _Ty* operator->() const noexcept(true) { return GetSingleton(); }
		inline _Ty& operator*() noexcept(true) { return *GetSingleton(); }
		inline const _Ty& operator*() const noexcept(true) { return *GetSingleton(); }

		CKPE_READ_PROPERTY(GetSingleton) _Ty* Singleton;
	private:
		_Ty** _singleton;
	};
}