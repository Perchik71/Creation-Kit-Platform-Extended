// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		template<typename _Ty>
		class ISingleton
		{
		public:
			ISingleton() : _singleton(nullptr)
			{}
			ISingleton(const ISingleton& S) : _singleton(S._singleton)
			{}

			inline ISingleton& operator=(const ISingleton& S)
			{
				_singleton = S._singleton;
				return *this;
			}

			inline ISingleton& operator=(const _Ty** S)
			{
				_singleton = S;
				return *this;
			}

			inline ISingleton& operator=(const uintptr_t addr)
			{
				_singleton = (_Ty**)addr;
				return *this;
			}
			
			inline _Ty* GetSingleton() const
			{
				Assert(_singleton);
				return *_singleton;
			}

			READ_PROPERTY(GetSingleton) _Ty* Singleton;
		private:
			_Ty** _singleton;
		};
	}
}