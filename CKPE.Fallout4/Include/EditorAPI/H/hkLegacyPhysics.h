// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkpAction.h>
#include <EditorAPI/H/hkpShape.h>
#include <EditorAPI/N/NiObject.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE::Fallout4::EditorAPI
{
	class hkLiquidAction : public hkpUnaryAction
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkLiquidAction;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkLiquidAction[0];

		/* 0x038 */ std::byte pad038[0x18];
	};
	static_assert(sizeof(hkLiquidAction) == 0x50);

	class hkNormalTriangleShape : public hkpTriangleShape
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkNormalTriangleShape;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkNormalTriangleShape[0];

		/* 0x070 */ std::byte pad070[0x10];
	};
	static_assert(sizeof(hkNormalTriangleShape) == 0x80);

	class hkOrientHingedBodyAction : public hkpUnaryAction
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkOrientHingedBodyAction;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkOrientHingedBodyAction[0];

		/* 0x038 */ std::byte pad038[0x48];
	};
	static_assert(sizeof(hkOrientHingedBodyAction) == 0x80);

	class hkScaledMoppBvTreeShape : public hkpMoppBvTreeShape
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkScaledMoppBvTreeShape;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkScaledMoppBvTreeShape[0];

		/* 0x070 */ std::byte pad070[0x10];
	};
	static_assert(sizeof(hkScaledMoppBvTreeShape) == 0x80);

	class hkNiTriStripsShape : public hkpShapeCollection
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkNiTriStripsShape;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkNiTriStripsShape[0];

		virtual void hkNiTriStripsShape_Unk14();
		virtual void hkNiTriStripsShape_Unk15();

		/* 0x030 */ std::byte pad030[0xA0];
	};
	static_assert(sizeof(hkNiTriStripsShape) == 0xD0);

	class hkPackedNiTriStripsShape : public hkpShapeCollection
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkPackedNiTriStripsShape;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkPackedNiTriStripsShape[0];

		virtual void hkPackedNiTriStripsShape_Unk14();
		virtual void hkPackedNiTriStripsShape_Unk15();

		/* 0x030 */ std::byte pad030[0x90];
	};
	static_assert(sizeof(hkPackedNiTriStripsShape) == 0xC0);

	class hkPackedNiTriStripsData : public NiAPI::NiObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkPackedNiTriStripsData;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkPackedNiTriStripsData[0];

		/* 02 */ virtual NiAPI::NiRTTI* GetRTTI() const noexcept(true) override;
		/* 1B */ virtual void LoadBinary(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1C */ virtual void LinkObject(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1D */ virtual bool RegisterStreamables(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1E */ virtual void SaveBinary(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1F */ virtual bool IsEqual(NiAPI::NiObject* Object) noexcept(true) override;
		/* 0x010 */ std::byte pad010[0x30];
	};
	static_assert(sizeof(hkPackedNiTriStripsData) == 0x40);
}
