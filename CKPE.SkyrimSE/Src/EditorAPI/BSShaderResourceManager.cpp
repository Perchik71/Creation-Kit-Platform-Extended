// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/CKSSE/BSShaderResourceManager_CK.cpp

#include <CKPE.Asserts.h>
#include <EditorAPI/BSTriShape.h>
#include <EditorAPI/BSDynamicTriShape.h>
#include <EditorAPI/BSShaderResourceManager.h>
#include <EditorAPI/NiAPI/NiCollisionUtils.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			using namespace NiAPI;

			std::uintptr_t pointer_PickGetRecord_sub = 0;

			bool BSShaderResourceManager::FindIntersectionsTriShapeFastPath(NiPoint3& kOrigin, const NiPoint3& kDir, 
				NiPick& kPick, BSTriShape* pkTriShape)
			{
				if (pkTriShape->m_TriangleCount <= 0 || !pkTriShape->QRendererData())
					return false;

				const std::uint16_t* indices = nullptr;
				std::uintptr_t vertexData[2] = {};
				std::uint32_t vertexStrides[2] = {};

				if (pkTriShape->QType() == GEOMETRY_TYPE_TRISHAPE || pkTriShape->QType() == GEOMETRY_TYPE_MULTIINDEX_TRISHAPE)
				{
					auto shape = static_cast<const BSTriShape*>(pkTriShape);
					auto rendererData = static_cast<const BSGraphics::TriShape*>(shape->QRendererData());

					CKPE_ASSERT_MSG(rendererData->m_RawVertexData, "Trying to select a triangle with no vertex data!?");

					indices = reinterpret_cast<const std::uint16_t*>(rendererData->m_RawIndexData);

					if (BSGeometry::HasVertexAttribute(shape->GetVertexDesc(), 0))
						vertexData[0] = reinterpret_cast<std::uintptr_t>(rendererData->m_RawVertexData) + shape->GetVertexAttributeOffset(0);

					if (BSGeometry::HasVertexAttribute(shape->GetVertexDesc(), 3))
						vertexData[1] = reinterpret_cast<std::uintptr_t>(rendererData->m_RawVertexData) + shape->GetVertexAttributeOffset(3);

					vertexStrides[0] = shape->GetVertexSize();
					vertexStrides[1] = shape->GetVertexSize();
				}
				else if (pkTriShape->QType() == GEOMETRY_TYPE_DYNAMIC_TRISHAPE)
				{
					auto shape = static_cast<BSDynamicTriShape*>(pkTriShape);
					auto rendererData = static_cast<const BSGraphics::DynamicTriShape*>(shape->QRendererData());

					indices = reinterpret_cast<const std::uint16_t*>(rendererData->m_RawIndexData);
					auto dynamicData = reinterpret_cast<std::uintptr_t>(shape->LockDynamicDataForRead());// NOTE: SPINLOCK IS HELD

					// Position
					if (BSGeometry::HasVertexAttribute(shape->GetVertexDesc(), 0))
					{
						if (shape->GetVertexAttributeStream(0) == 1)
						{
							CKPE_ASSERT(dynamicData);

							vertexData[0] = dynamicData + shape->GetVertexAttributeOffset(0);
							vertexStrides[0] = shape->GetDynamicVertexSize();
						}
						else
						{
							if (rendererData->m_RawVertexData)
								vertexData[0] = reinterpret_cast<std::uintptr_t>(rendererData->m_RawVertexData) + shape->GetVertexAttributeOffset(0);

							vertexStrides[0] = shape->GetVertexSize();
						}
					}

					// Annoying hack for broken vertex layouts (LOD land). Vertex descriptions are incorrect.
					if (!vertexData[0])
					{
						vertexData[0] = dynamicData;
						vertexStrides[0] = shape->GetDynamicVertexSize();
					}

					// Normals
					if (BSGeometry::HasVertexAttribute(shape->GetVertexDesc(), 3))
					{
						if (shape->GetVertexAttributeStream(3) == 1)
						{
							CKPE_ASSERT(dynamicData);

							vertexData[1] = dynamicData + shape->GetVertexAttributeOffset(3);
							vertexStrides[1] = shape->GetDynamicVertexSize();
						}
						else
						{
							if (rendererData->m_RawVertexData)
								vertexData[1] = reinterpret_cast<std::uintptr_t>(rendererData->m_RawVertexData) + shape->GetVertexAttributeOffset(3);

							vertexStrides[1] = shape->GetVertexSize();
						}
					}
				}
				else
				{
					// All other geometry types are unsupported
					return false;
				}

				// Vertex positions are required, but normals are not
				bool intersectionFound = false;

				if (indices && vertexData[0])
				{
					auto fetchStream = [vertexData, vertexStrides](std::uint32_t Stream, std::uintptr_t VertexIndex)
					{
						// 0 = position, 1 = normal
						if (Stream == 1)
						{
							auto data = reinterpret_cast<const std::uint8_t*>(vertexData[Stream] + (VertexIndex * vertexStrides[Stream]));
							return NiPoint3(((float)data[0] / 255.0f) * 2.0f - 1.0f, ((float)data[1] / 255.0f) * 2.0f - 1.0f, ((float)data[2] / 255.0f) * 2.0f - 1.0f);
						}

						auto data = reinterpret_cast<const float*>(vertexData[Stream] + (VertexIndex * vertexStrides[Stream]));
						return NiPoint3(data[0], data[1], data[2]);
					};

					// Find ray's model space origin and direction
					const NiTransform& kWorld = pkTriShape->GetWorldTransform();
					const float fInvWorldScale = 1.0f / kWorld.m_fScale;

					NiPoint3 kDiff = kOrigin - kWorld.m_Translate;
					NiPoint3 kModelOrigin = (kDiff * kWorld.m_Rotate) * fInvWorldScale;
					NiPoint3 kModelDir = kDir * kWorld.m_Rotate;

					for (std::uint32_t i = 0; i < (pkTriShape->m_TriangleCount * 3u); i += 3)
					{
						NiPoint3 v[3] =
						{
							fetchStream(0, indices[i + 0]),
							fetchStream(0, indices[i + 1]),
							fetchStream(0, indices[i + 2]),
						};

						NiPoint3 intersect;
						float lineParam;
						float triParam1;
						float triParam2;

						if (NiCollisionUtils::IntersectTriangle(kModelOrigin, kModelDir, v[0], v[1], v[2], kPick.GetFrontOnly(), intersect, lineParam, triParam1, triParam2))
						{
							auto record = ((NiPick::Record* (__fastcall*)(std::uintptr_t, BSTriShape*))pointer_PickGetRecord_sub)((std::uintptr_t)&kPick + 0x20, pkTriShape);
							intersectionFound = true;

							// Ray intersection point
							if (kPick.GetCoordinateType() == NiPick::WORLD_COORDINATES)
								record->SetIntersection(kWorld.m_fScale * (kWorld.m_Rotate * intersect) + kWorld.m_Translate);
							else
								record->SetIntersection(intersect);

							// Distance from origin (NOTE: Bethesda code never does this scale)
							record->SetDistance(lineParam * kWorld.m_fScale);

							// Normal vector
							if (kPick.GetReturnNormal())
							{
								NiPoint3 normal;

								// if (kPick.GetReturnSmoothNormal()) was also eliminated in their code
								if (vertexData[1])
								{
									float triParam0 = 1.0f - (triParam1 + triParam2);

									normal =
										triParam0 * fetchStream(1, indices[i + 0]) +
										triParam1 * fetchStream(1, indices[i + 1]) +
										triParam2 * fetchStream(1, indices[i + 2]);
								}
								else
								{
									NiPoint3 v0 = v[1] - v[0];
									NiPoint3 v1 = v[2] - v[0];

									normal = v0.Cross(v1);
								}

								normal.Normalize();

								if (kPick.GetCoordinateType() == NiPick::WORLD_COORDINATES)
									normal = kWorld.m_Rotate * normal;

								record->SetNormal(normal);
							}
							else
							{
								// Zero-initialize if someone tries to access it anyway
								record->SetNormal(ZERO_P3);
							}

							if (kPick.GetPickType() == NiPick::FIND_FIRST && kPick.GetSortType() == NiPick::NO_SORT)
								break;
						}
					}
				}

				if (pkTriShape->QType() == GEOMETRY_TYPE_DYNAMIC_TRISHAPE)
					static_cast<BSDynamicTriShape*>(pkTriShape)->UnlockDynamicData();

				return intersectionFound;
			}

			bool BSShaderResourceManager::FindIntersectionsTriShapeFastPathEx(NiPoint3& kOrigin, const NiPoint3& kDir, 
				NiPick& kPick, class BSTriShape* pkTriShape) {
				//
				// This function is for the new version of CK. 
				// It is no different from the old one, except that there is no more offset 0x20 to get the desired value.
				//\

				if (pkTriShape->m_TriangleCount <= 0 || !pkTriShape->QRendererData())
					return false;

				const std::uint16_t* indices = nullptr;
				std::uintptr_t vertexData[2] = {};
				std::uint32_t vertexStrides[2] = {};

				if (pkTriShape->QType() == GEOMETRY_TYPE_TRISHAPE || pkTriShape->QType() == GEOMETRY_TYPE_MULTIINDEX_TRISHAPE)
				{
					auto shape = static_cast<const BSTriShape*>(pkTriShape);
					auto rendererData = static_cast<const BSGraphics::TriShape*>(shape->QRendererData());

					CKPE_ASSERT_MSG(rendererData->m_RawVertexData, "Trying to select a triangle with no vertex data!?");

					indices = reinterpret_cast<const std::uint16_t*>(rendererData->m_RawIndexData);

					if (BSGeometry::HasVertexAttribute(shape->GetVertexDesc(), 0))
						vertexData[0] = reinterpret_cast<std::uintptr_t>(rendererData->m_RawVertexData) + shape->GetVertexAttributeOffset(0);

					if (BSGeometry::HasVertexAttribute(shape->GetVertexDesc(), 3))
						vertexData[1] = reinterpret_cast<std::uintptr_t>(rendererData->m_RawVertexData) + shape->GetVertexAttributeOffset(3);

					vertexStrides[0] = shape->GetVertexSize();
					vertexStrides[1] = shape->GetVertexSize();
				}
				else if (pkTriShape->QType() == GEOMETRY_TYPE_DYNAMIC_TRISHAPE)
				{
					auto shape = static_cast<BSDynamicTriShape*>(pkTriShape);
					auto rendererData = static_cast<const BSGraphics::DynamicTriShape*>(shape->QRendererData());

					indices = reinterpret_cast<const std::uint16_t*>(rendererData->m_RawIndexData);
					auto dynamicData = reinterpret_cast<std::uintptr_t>(shape->LockDynamicDataForRead());// NOTE: SPINLOCK IS HELD

					// Position
					if (BSGeometry::HasVertexAttribute(shape->GetVertexDesc(), 0))
					{
						if (shape->GetVertexAttributeStream(0) == 1)
						{
							CKPE_ASSERT(dynamicData);

							vertexData[0] = dynamicData + shape->GetVertexAttributeOffset(0);
							vertexStrides[0] = shape->GetDynamicVertexSize();
						}
						else
						{
							if (rendererData->m_RawVertexData)
								vertexData[0] = reinterpret_cast<std::uintptr_t>(rendererData->m_RawVertexData) + shape->GetVertexAttributeOffset(0);

							vertexStrides[0] = shape->GetVertexSize();
						}
					}

					// Annoying hack for broken vertex layouts (LOD land). Vertex descriptions are incorrect.
					if (!vertexData[0])
					{
						vertexData[0] = dynamicData;
						vertexStrides[0] = shape->GetDynamicVertexSize();
					}

					// Normals
					if (BSGeometry::HasVertexAttribute(shape->GetVertexDesc(), 3))
					{
						if (shape->GetVertexAttributeStream(3) == 1)
						{
							CKPE_ASSERT(dynamicData);

							vertexData[1] = dynamicData + shape->GetVertexAttributeOffset(3);
							vertexStrides[1] = shape->GetDynamicVertexSize();
						}
						else
						{
							if (rendererData->m_RawVertexData)
								vertexData[1] = reinterpret_cast<std::uintptr_t>(rendererData->m_RawVertexData) + shape->GetVertexAttributeOffset(3);

							vertexStrides[1] = shape->GetVertexSize();
						}
					}
				}
				else
				{
					// All other geometry types are unsupported
					return false;
				}

				// Vertex positions are required, but normals are not
				bool intersectionFound = false;

				if (indices && vertexData[0])
				{
					auto fetchStream = [vertexData, vertexStrides](std::uint32_t Stream, std::uintptr_t VertexIndex)
					{
						// 0 = position, 1 = normal
						if (Stream == 1)
						{
							auto data = reinterpret_cast<const std::uint8_t*>(vertexData[Stream] + (VertexIndex * vertexStrides[Stream]));
							return NiPoint3(((float)data[0] / 255.0f) * 2.0f - 1.0f, ((float)data[1] / 255.0f) * 2.0f - 1.0f, ((float)data[2] / 255.0f) * 2.0f - 1.0f);
						}

						auto data = reinterpret_cast<const float*>(vertexData[Stream] + (VertexIndex * vertexStrides[Stream]));
						return NiPoint3(data[0], data[1], data[2]);
					};

					// Find ray's model space origin and direction
					const NiTransform& kWorld = pkTriShape->GetWorldTransform();
					const float fInvWorldScale = 1.0f / kWorld.m_fScale;

					NiPoint3 kDiff = kOrigin - kWorld.m_Translate;
					NiPoint3 kModelOrigin = (kDiff * kWorld.m_Rotate) * fInvWorldScale;
					NiPoint3 kModelDir = kDir * kWorld.m_Rotate;

					for (std::uint32_t i = 0; i < (pkTriShape->m_TriangleCount * 3u); i += 3)
					{
						NiPoint3 v[3] =
						{
							fetchStream(0, indices[i + 0]),
							fetchStream(0, indices[i + 1]),
							fetchStream(0, indices[i + 2]),
						};

						NiPoint3 intersect;
						float lineParam;
						float triParam1;
						float triParam2;

						if (NiCollisionUtils::IntersectTriangle(kModelOrigin, kModelDir, v[0], v[1], v[2], kPick.GetFrontOnly(), intersect, lineParam, triParam1, triParam2))
						{
							auto record = ((NiPick::Record* (__fastcall*)(std::uintptr_t, BSTriShape*))pointer_PickGetRecord_sub)((std::uintptr_t)&kPick, pkTriShape);
							intersectionFound = true;

							// Ray intersection point
							if (kPick.GetCoordinateType() == NiPick::WORLD_COORDINATES)
								record->SetIntersection(kWorld.m_fScale * (kWorld.m_Rotate * intersect) + kWorld.m_Translate);
							else
								record->SetIntersection(intersect);

							// Distance from origin (NOTE: Bethesda code never does this scale)
							record->SetDistance(lineParam * kWorld.m_fScale);

							// Normal vector
							if (kPick.GetReturnNormal())
							{
								NiPoint3 normal;

								// if (kPick.GetReturnSmoothNormal()) was also eliminated in their code
								if (vertexData[1])
								{
									float triParam0 = 1.0f - (triParam1 + triParam2);

									normal =
										triParam0 * fetchStream(1, indices[i + 0]) +
										triParam1 * fetchStream(1, indices[i + 1]) +
										triParam2 * fetchStream(1, indices[i + 2]);
								}
								else
								{
									NiPoint3 v0 = v[1] - v[0];
									NiPoint3 v1 = v[2] - v[0];

									normal = v0.Cross(v1);
								}

								normal.Normalize();

								if (kPick.GetCoordinateType() == NiPick::WORLD_COORDINATES)
									normal = kWorld.m_Rotate * normal;

								record->SetNormal(normal);
							}
							else
							{
								// Zero-initialize if someone tries to access it anyway
								record->SetNormal(ZERO_P3);
							}

							if (kPick.GetPickType() == NiPick::FIND_FIRST && kPick.GetSortType() == NiPick::NO_SORT)
								break;
						}
					}
				}

				if (pkTriShape->QType() == GEOMETRY_TYPE_DYNAMIC_TRISHAPE)
					static_cast<BSDynamicTriShape*>(pkTriShape)->UnlockDynamicData();

				return intersectionFound;
			}
		}
	}
}