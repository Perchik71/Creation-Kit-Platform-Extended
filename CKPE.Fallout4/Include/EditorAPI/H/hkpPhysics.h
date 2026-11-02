// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkpAction.h>
#include <EditorAPI/H/hkpCollisionAgent.h>
#include <EditorAPI/H/hkpCollisionCollector.h>
#include <EditorAPI/H/hkpCollisionFilter.h>
#include <EditorAPI/H/hkpConstraintData.h>
#include <EditorAPI/H/hkpMopp.h>
#include <EditorAPI/H/hkpMotion.h>
#include <EditorAPI/H/hkpShape.h>
#include <EditorAPI/H/hkpWorld.h>
#include <EditorAPI/H/hkInfrastructure.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE::Fallout4::EditorAPI
{
	class hkpHavokSnapshot
	{
	public:
		class ConvertListener : public hkPackfileWriter::AddObjectListener
		{
		public:
			constexpr static Common::ID RTTI = IDs::RTTI::hkpHavokSnapshot__ConvertListener;
			constexpr static Common::ID VTABLE = IDs::VTABLE::hkpHavokSnapshot__ConvertListener[0];

			/* 0x010 */ std::uint8_t unk010[0x10];
		};
	};
	static_assert(sizeof(hkpHavokSnapshot::ConvertListener) == 0x20);
}
