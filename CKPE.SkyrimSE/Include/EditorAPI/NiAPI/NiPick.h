#pragma once

#include <cstdint>
#include <EditorAPI/NiAPI/NiPoint.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiPick
				{
				public:
					enum PickType : std::uint32_t
					{
						FIND_ALL = 0,
						FIND_FIRST = 1,
					};

					enum SortType : std::uint32_t
					{
						SORT = 0,
						NO_SORT = 1,
					};

					enum IntersectType : std::uint32_t
					{
						BOUND_INTERSECT = 0,
						TRIANGLE_INTERSECT = 1,
					};

					enum CoordinateType : std::uint32_t
					{
						MODEL_COORDINATES = 0,
						WORLD_COORDINATES = 1,
					};
				private:
					PickType m_ePickType;
					SortType m_eSortType;
					char _pad0[0x4];
					CoordinateType m_eCoordinateType;
					bool m_bFrontOnly;
					char _pad1[0x38];
					bool m_bReturnNormal;
				public:
					[[nodiscard]] inline PickType GetPickType() const noexcept(true) { return m_ePickType; }
					[[nodiscard]] inline SortType GetSortType() const noexcept(true) { return m_eSortType; }
					[[nodiscard]] inline CoordinateType GetCoordinateType() const noexcept(true) { return m_eCoordinateType; }
					[[nodiscard]] inline bool GetFrontOnly() const noexcept(true) { return m_bFrontOnly; }
					[[nodiscard]] inline bool GetReturnNormal() const noexcept(true) { return m_bReturnNormal; }

					class Record
					{
					public:
						void* m_Object;// NiPointer<NiAVObject*>
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
	}
}