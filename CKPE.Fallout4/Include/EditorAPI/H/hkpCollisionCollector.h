// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkBaseTypes.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE::Fallout4::EditorAPI
{
	class hkpAabbCastCollector
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpAabbCastCollector;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpAabbCastCollector[0];

		virtual void addBroadPhaseHandle(const void* Handle, std::int32_t CastIndex); // hkpBroadPhaseHandle, declared before the destructor
		virtual ~hkpAabbCastCollector();
		/* 0x008 */ std::uint8_t unk008[0x18];
	};
	static_assert(sizeof(hkpAabbCastCollector) == 0x20);

	class hkpBroadPhaseCastCollector
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpBroadPhaseCastCollector;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpBroadPhaseCastCollector[0];

		virtual ~hkpBroadPhaseCastCollector();
		virtual void addBroadPhaseHandle(const void* Handle, std::int32_t CastIndex);
	};
	static_assert(sizeof(hkpBroadPhaseCastCollector) == 0x8);

	class hkpRayHitCollector
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpRayHitCollector;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpRayHitCollector[0];

		virtual void addRayHit(const void* CdBody, const void* HitInfo); // declared before the destructor; the closest collector keeps the hit with the smallest fraction
		virtual ~hkpRayHitCollector();
		/* 0x008 */ std::uint8_t unk008[8];
	};
	static_assert(sizeof(hkpRayHitCollector) == 0x10);

	class hkpCdBodyPairCollector
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpCdBodyPairCollector;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpCdBodyPairCollector[0];

		virtual ~hkpCdBodyPairCollector();
		virtual void addCdBodyPair(const void* BodyA, const void* BodyB);
		virtual void reset();
		/* 0x008 */ std::uint8_t unk008[8];
	};
	static_assert(sizeof(hkpCdBodyPairCollector) == 0x10);

	class hkpCdPointCollector
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpCdPointCollector;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpCdPointCollector[0];

		virtual ~hkpCdPointCollector();
		virtual void addCdPoint(const void* Point); // hkpCdPoint; the closest collector keeps the point with the smallest distance
		virtual void reset();
		/* 0x008 */ std::uint8_t unk008[8];
	};
	static_assert(sizeof(hkpCdPointCollector) == 0x10);

	class hkpFlagCdBodyPairCollector : public hkpCdBodyPairCollector
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpFlagCdBodyPairCollector;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpFlagCdBodyPairCollector[0];
	};
	static_assert(sizeof(hkpFlagCdBodyPairCollector) == 0x10);

	class alignas(16) hkpClosestCdPointCollector : public hkpCdPointCollector
	{
	public:
		/* 0x010 */ std::uint8_t unk010[0x50];
	};
	static_assert(sizeof(hkpClosestCdPointCollector) == 0x60);

	class alignas(16) hkpClosestRayHitCollector : public hkpRayHitCollector
	{
	public:
		/* 0x010 */ std::uint8_t unk010[0x60];
	};
	static_assert(sizeof(hkpClosestRayHitCollector) == 0x70);

	class hkpSimpleClosestContactCollector : public hkpCdPointCollector
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpSimpleClosestContactCollector;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpSimpleClosestContactCollector[0];

		/* 0x010 */ std::uint8_t unk010[0x30];
	};
	static_assert(sizeof(hkpSimpleClosestContactCollector) == 0x40);

	class hkpSymmetricAgentFlipBodyCollector : public hkpCdBodyPairCollector
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpSymmetricAgentFlipBodyCollector;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpSymmetricAgentFlipBodyCollector[0];

		/* 0x010 */ std::uint8_t unk010[8];
	};
	static_assert(sizeof(hkpSymmetricAgentFlipBodyCollector) == 0x18);

	class hkpSymmetricAgentFlipCastCollector : public hkpCdPointCollector
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpSymmetricAgentFlipCastCollector;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpSymmetricAgentFlipCastCollector[0];

		/* 0x010 */ std::uint8_t unk010[0x20];
	};
	static_assert(sizeof(hkpSymmetricAgentFlipCastCollector) == 0x30);

	class hkpSymmetricAgentFlipCollector : public hkpCdPointCollector
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpSymmetricAgentFlipCollector;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpSymmetricAgentFlipCollector[0];
	};
	static_assert(sizeof(hkpSymmetricAgentFlipCollector) == 0x10);

	class hkpSimpleWorldRayCaster : public hkpBroadPhaseCastCollector
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpSimpleWorldRayCaster;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpSimpleWorldRayCaster[0];

		/* 0x008 */ std::uint8_t unk008[0x58];
	};
	static_assert(sizeof(hkpSimpleWorldRayCaster) == 0x60);

	class hkpWorldRayCaster : public hkpBroadPhaseCastCollector
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpWorldRayCaster;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpWorldRayCaster[0];

		/* 0x008 */ std::uint8_t unk008[0x68];
	};
	static_assert(sizeof(hkpWorldRayCaster) == 0x70);

	class hkpWorldLinearCaster : public hkpBroadPhaseCastCollector
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpWorldLinearCaster;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpWorldLinearCaster[0];

		/* 0x008 */ std::uint8_t unk008[0xB8];
	};
	static_assert(sizeof(hkpWorldLinearCaster) == 0xC0);

	namespace hkpStaticCompoundShape_Internals
	{
		class AabbCastQuery
		{
		public:
			class AabbCastCollectorWrapper : public hkpAabbCastCollector
			{
			public:
				constexpr static Common::ID RTTI = IDs::RTTI::hkpStaticCompoundShape_Internals__AabbCastQuery__AabbCastCollectorWrapper;
				constexpr static Common::ID VTABLE = IDs::VTABLE::hkpStaticCompoundShape_Internals__AabbCastQuery__AabbCastCollectorWrapper[0];

				/* 0x020 */ std::uint8_t unk020[0x30];
			};
		};
		static_assert(sizeof(hkpStaticCompoundShape_Internals::AabbCastQuery::AabbCastCollectorWrapper) == 0x50);
	}

	class hkpSaveContactPointsUtil
	{
	public:
		class EntitySelector
		{
		public:
			constexpr static Common::ID RTTI = IDs::RTTI::hkpSaveContactPointsUtil__EntitySelector;
			constexpr static Common::ID VTABLE = IDs::VTABLE::hkpSaveContactPointsUtil__EntitySelector[0];

			virtual ~EntitySelector();
			virtual void sub_EntitySelector_01();
		};
	};
	static_assert(sizeof(hkpSaveContactPointsUtil::EntitySelector) == 0x8);
}
