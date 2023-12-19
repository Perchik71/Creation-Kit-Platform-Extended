#pragma once

#include "NiPoint.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		class NiPick
		{
		public:
			enum PickType
			{
				FIND_ALL = 0,
				FIND_FIRST = 1,
			};

			enum SortType
			{
				SORT = 0,
				NO_SORT = 1,
			};

			enum IntersectType
			{
				BOUND_INTERSECT = 0,
				TRIANGLE_INTERSECT = 1,
			};

			enum CoordinateType
			{
				MODEL_COORDINATES = 0,
				WORLD_COORDINATES = 1,
			};

			PickType m_ePickType;
			SortType m_eSortType;
			char _pad0[0x4];
			CoordinateType m_eCoordinateType;
			bool m_bFrontOnly;
			char _pad1[0x38];
			bool m_bReturnNormal;

			PickType GetPickType() const
			{
				return m_ePickType;
			}

			SortType GetSortType() const
			{
				return m_eSortType;
			}

			CoordinateType GetCoordinateType() const
			{
				return m_eCoordinateType;
			}

			bool GetFrontOnly() const
			{
				return m_bFrontOnly;
			}

			bool GetReturnNormal() const
			{
				return m_bReturnNormal;
			}

			class Record
			{
			public:
				void* m_Object;// NiPointer<NiAVObject *>
				NiPoint3 m_Intersect;
				NiPoint3 m_Normal;
				float m_Distance;

				inline void SetIntersection(const NiPoint3& Intersect)
				{
					m_Intersect = Intersect;
				}

				inline void SetNormal(const NiPoint3& Normal)
				{
					m_Normal = Normal;
				}

				inline void SetDistance(float Distance)
				{
					m_Distance = Distance;
				}
			};
			static_assert(sizeof(Record) == 0x28);
		};
	}
}