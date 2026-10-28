// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			template<typename T>
			class BGSUniqueObjectRef
			{
			public:
				BGSUniqueObjectRef(T* Ref) noexcept(true) : _Ref(Ref) { if (_Ref) _Ref->IncRefCount(); }
				~BGSUniqueObjectRef() noexcept(true) { if (_Ref) _Ref->DecRefCount(); }

				inline T* operator->() noexcept(true) { return _Ref; }
				inline T* Get() const noexcept(true) { return _Ref; }
				inline bool Empty() const noexcept(true) { return !_Ref; }
			private:
				T* _Ref;
			};
		}
	}
}