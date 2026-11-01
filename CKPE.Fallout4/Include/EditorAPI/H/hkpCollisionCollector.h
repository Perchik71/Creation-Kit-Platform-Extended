// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkBaseTypes.h>

namespace CKPE::Fallout4::EditorAPI
{
	class hkpAabbCastCollector
	{
	public:
		virtual void addBroadPhaseHandle(const void* Handle, std::int32_t CastIndex); // hkpBroadPhaseHandle, declared before the destructor
		virtual ~hkpAabbCastCollector();
		/* 0x008 */ std::uint8_t unk008[0x18];
	};
	static_assert(sizeof(hkpAabbCastCollector) == 0x20);

	class hkpBroadPhaseCastCollector
	{
	public:
		virtual ~hkpBroadPhaseCastCollector();
		virtual void addBroadPhaseHandle(const void* Handle, std::int32_t CastIndex);
	};
	static_assert(sizeof(hkpBroadPhaseCastCollector) == 0x8);

	class hkpRayHitCollector
	{
	public:
		virtual void addRayHit(const void* CdBody, const void* HitInfo); // declared before the destructor; the closest collector keeps the hit with the smallest fraction
		virtual ~hkpRayHitCollector();
		/* 0x008 */ std::uint8_t unk008[8];
	};
	static_assert(sizeof(hkpRayHitCollector) == 0x10);

	class hkpCdBodyPairCollector
	{
	public:
		virtual ~hkpCdBodyPairCollector();
		virtual void addCdBodyPair(const void* BodyA, const void* BodyB);
		virtual void reset();
		/* 0x008 */ std::uint8_t unk008[8];
	};
	static_assert(sizeof(hkpCdBodyPairCollector) == 0x10);

	class hkpCdPointCollector
	{
	public:
		virtual ~hkpCdPointCollector();
		virtual void addCdPoint(const void* Point); // hkpCdPoint; the closest collector keeps the point with the smallest distance
		virtual void reset();
		/* 0x008 */ std::uint8_t unk008[8];
	};
	static_assert(sizeof(hkpCdPointCollector) == 0x10);

	class hkpFlagCdBodyPairCollector : public hkpCdBodyPairCollector {};
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
		/* 0x010 */ std::uint8_t unk010[0x30];
	};
	static_assert(sizeof(hkpSimpleClosestContactCollector) == 0x40);

	class hkpSymmetricAgentFlipBodyCollector : public hkpCdBodyPairCollector
	{
	public:
		/* 0x010 */ std::uint8_t unk010[8];
	};
	static_assert(sizeof(hkpSymmetricAgentFlipBodyCollector) == 0x18);

	class hkpSymmetricAgentFlipCastCollector : public hkpCdPointCollector
	{
	public:
		/* 0x010 */ std::uint8_t unk010[0x20];
	};
	static_assert(sizeof(hkpSymmetricAgentFlipCastCollector) == 0x30);

	class hkpSymmetricAgentFlipCollector : public hkpCdPointCollector {};
	static_assert(sizeof(hkpSymmetricAgentFlipCollector) == 0x10);

	class hkpSimpleWorldRayCaster : public hkpBroadPhaseCastCollector
	{
	public:
		/* 0x008 */ std::uint8_t unk008[0x58];
	};
	static_assert(sizeof(hkpSimpleWorldRayCaster) == 0x60);

	class hkpWorldRayCaster : public hkpBroadPhaseCastCollector
	{
	public:
		/* 0x008 */ std::uint8_t unk008[0x68];
	};
	static_assert(sizeof(hkpWorldRayCaster) == 0x70);

	class hkpWorldLinearCaster : public hkpBroadPhaseCastCollector
	{
	public:
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
			virtual ~EntitySelector();
			virtual void sub_EntitySelector_01();
		};
	};
	static_assert(sizeof(hkpSaveContactPointsUtil::EntitySelector) == 0x8);
}
