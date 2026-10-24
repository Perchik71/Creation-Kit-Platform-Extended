// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkpAction.h>
#include <EditorAPI/H/hkpShape.h>
#include <EditorAPI/N/NiObject.h>

namespace CKPE::Fallout4::EditorAPI
{
	class hkLiquidAction : public hkpUnaryAction
	{
	public:
		/* 0x038 */ std::byte pad038[0x18];
	};
	static_assert(sizeof(hkLiquidAction) == 0x50);

	class hkNormalTriangleShape : public hkpTriangleShape
	{
	public:
		/* 0x070 */ std::byte pad070[0x10];
	};
	static_assert(sizeof(hkNormalTriangleShape) == 0x80);

	class hkOrientHingedBodyAction : public hkpUnaryAction
	{
	public:
		/* 0x038 */ std::byte pad038[0x48];
	};
	static_assert(sizeof(hkOrientHingedBodyAction) == 0x80);

	class hkScaledMoppBvTreeShape : public hkpMoppBvTreeShape
	{
	public:
		/* 0x070 */ std::byte pad070[0x10];
	};
	static_assert(sizeof(hkScaledMoppBvTreeShape) == 0x80);

	class hkNiTriStripsShape : public hkpShapeCollection
	{
	public:
		virtual void hkNiTriStripsShape_Unk14();
		virtual void hkNiTriStripsShape_Unk15();

		/* 0x030 */ std::byte pad030[0xA0];
	};
	static_assert(sizeof(hkNiTriStripsShape) == 0xD0);

	class hkPackedNiTriStripsShape : public hkpShapeCollection
	{
	public:
		virtual void hkPackedNiTriStripsShape_Unk14();
		virtual void hkPackedNiTriStripsShape_Unk15();

		/* 0x030 */ std::byte pad030[0x90];
	};
	static_assert(sizeof(hkPackedNiTriStripsShape) == 0xC0);

	class hkPackedNiTriStripsData : public NiAPI::NiObject
	{
	public:
		/* 0x010 */ std::byte pad010[0x30];
	};
	static_assert(sizeof(hkPackedNiTriStripsData) == 0x40);
}
