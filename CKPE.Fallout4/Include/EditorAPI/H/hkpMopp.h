// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkpShape.h>

namespace CKPE::Fallout4::EditorAPI
{
	class hkpMoppAssembler : public hkReferencedObject
	{
	public:
		virtual void sub_04();
		virtual void sub_05();
		virtual void sub_06();
		virtual void sub_07();
	};
	static_assert(sizeof(hkpMoppAssembler) == 0x10);

	class hkpMoppDefaultAssembler : public hkpMoppAssembler
	{
	public:
		/* 0x010 */ std::uint8_t unk010[0x90];
	};
	static_assert(sizeof(hkpMoppDefaultAssembler) == 0xA0);

	class hkpMoppMediator : public hkReferencedObject
	{
	public:
		virtual void sub_04();
		virtual void sub_05();
		virtual void sub_06();
		virtual void sub_07();
		virtual void sub_08();
		virtual void sub_09();
		virtual void sub_0A();
	};
	static_assert(sizeof(hkpMoppMediator) == 0x10);

	class hkpMoppShapeMediator : public hkpMoppMediator
	{
	public:
		/* 0x010 */ std::uint8_t unk010[0x10];
	};
	static_assert(sizeof(hkpMoppShapeMediator) == 0x20);

	class hkpMoppCachedShapeMediator : public hkpMoppMediator
	{
	public:
		/* 0x010 */ std::uint8_t unk010[0x20];
	};
	static_assert(sizeof(hkpMoppCachedShapeMediator) == 0x30);

	class hkpMoppNodeMgr : public hkReferencedObject
	{
	public:
		virtual void sub_04();
		virtual void sub_05();
	};
	static_assert(sizeof(hkpMoppNodeMgr) == 0x10);

	class hkpMoppSplitter : public hkpMoppNodeMgr
	{
	public:
		virtual void sub_06();
	};
	static_assert(sizeof(hkpMoppSplitter) == 0x10);

	class hkpMoppDefaultSplitter : public hkpMoppSplitter
	{
	public:
		/* 0x010 */ std::uint8_t unk010[0x68];
	};
	static_assert(sizeof(hkpMoppDefaultSplitter) == 0x78);

	class hkpMoppCodeGenerator : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::uint8_t unk010[0x20];
	};
	static_assert(sizeof(hkpMoppCodeGenerator) == 0x30);

	class hkpMoppCostFunction : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::uint8_t unk010[0x18];
	};
	static_assert(sizeof(hkpMoppCostFunction) == 0x28);
}
