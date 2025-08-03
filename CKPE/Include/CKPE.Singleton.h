// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <cstdint>
#include "CKPE.Common.h"

namespace CKPE
{
	template<typename _Ty>
	class Singleton
	{
	public:
		Singleton() : _singleton(nullptr)
		{}
		Singleton(const Singleton& S) : _singleton(S._singleton)
		{}

		inline Singleton& operator=(const Singleton& S)
		{
			_singleton = S._singleton;
			return *this;
		}

		inline Singleton& operator=(const _Ty** S)
		{
			_singleton = S;
			return *this;
		}

		inline Singleton& operator=(const std::uintptr_t addr)
		{
			_singleton = (_Ty**)addr;
			return *this;
		}
		
		inline _Ty* GetSingleton() const
		{
			Assert(_singleton);
			return *_singleton;
		}

		CKPE_READ_PROPERTY(GetSingleton) _Ty* Singleton;
	private:
		_Ty** _singleton;
	};
}