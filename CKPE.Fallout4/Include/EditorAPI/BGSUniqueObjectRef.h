// Copyright © 2020-2025 aka perchik71. All rights reserved.
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
				BGSUniqueObjectRef(T* Ref) : _Ref(Ref) { if (_Ref) _Ref->IncRefCount(); }
				~BGSUniqueObjectRef() { if (_Ref) _Ref->DecRefCount(); }

				inline T* operator->() { return _Ref; }
				inline T* Get() const { return _Ref; }
				inline bool Empty() const { return !_Ref; }
			private:
				T* _Ref;
			};
		}
	}
}