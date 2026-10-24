// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>

namespace CKPE::Fallout4::EditorAPI
{
	class hkClass;

	class hkBaseObject
	{
	public:
		virtual ~hkBaseObject();
		virtual void __first_virtual_table_function__();
	};
	static_assert(sizeof(hkBaseObject) == 0x8);

	class hkReferencedObject : public hkBaseObject
	{
	public:
		virtual const hkClass* GetClassType() const;
		virtual void DeleteThisReferencedObject() const;

		/* 0x008 */ std::uint32_t m_memSizeAndRefCount;
		/* 0x00C */ std::uint32_t pad00C;
	};
	static_assert(sizeof(hkReferencedObject) == 0x10);
	static_assert(offsetof(hkReferencedObject, m_memSizeAndRefCount) == 0x8);
}
