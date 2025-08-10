// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <unordered_map>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Asserts.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/BSTArray.h>
#include <Patches/CKPE.SkyrimSE.Patch.NavMeshPseudoDelete.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			std::uintptr_t pointer_NavMeshPseudoDelete_sub1 = 0;
			std::uintptr_t pointer_NavMeshPseudoDelete_sub2 = 0;

			class BSNavmesh
			{
			public:
				constexpr static std::uint16_t BAD_NAVMESH_TRIANGLE = 0xFFFF;
				constexpr static std::uint16_t BAD_NAVMESH_VERTEX = 0xFFFF;

				constexpr static std::uint32_t CUSTOM_NAVMESH_PSEUDODELTE_FLAG = 0x8;
				constexpr static std::uint32_t NAVMESH_FOUND_FLAG = 0x800;

				class VertexData
				{
				public:
					float m_Values[3];
				};

				class BSNavmeshTriangle
				{
				public:
					std::uint16_t m_Vertices[3];	// Triangle vertices pointing into the mesh vertex array
					std::uint16_t m_Edges[3];	// Index into the triangle array of the navmesh OR an index into the edge links array
					std::uint32_t m_ExtraInfo;

					std::uint16_t GetVertexIndex(std::uint32_t Vertex) const noexcept(true)
					{
						return m_Vertices[Vertex];
					}

					std::uint16_t GetEdgeIndex(std::uint32_t Edge) const noexcept(true)
					{
						return m_Edges[Edge];
					}

					std::uint16_t GetEdgeLinkIndex(std::uint32_t Edge) const noexcept(true)
					{
						if (HasExtraInfo(Edge))
							return m_Edges[Edge];

						CKPE_ASSERT(false);
						return BAD_NAVMESH_TRIANGLE;
					}

					bool HasExtraInfo(std::uint32_t Edge) const noexcept(true)
					{
						// If set, m_Edge[Edge] points in to m_ExtraInfo[]
						// If not set, triangle is in the same mesh
						return (m_ExtraInfo & (1 << Edge)) != 0;
					}

					void ClearEdge(std::uint32_t Edge) noexcept(true)
					{
						m_ExtraInfo &= ~(1 << Edge);
						m_Edges[Edge] = BAD_NAVMESH_TRIANGLE;
					}

					std::uint16_t HKGetVertexIndex_DegenerateCheck(std::uint32_t Vertex) noexcept(true)
					{
						// If special flag is set: return an invalid value to make the == comparison fail
						if (m_ExtraInfo & CUSTOM_NAVMESH_PSEUDODELTE_FLAG)
							return BAD_NAVMESH_VERTEX;

						return GetVertexIndex(Vertex);
					}

					std::uint16_t HKGetVertexIndex_VertexCheck(std::uint32_t Vertex) noexcept(true)
					{
						// Same as degenerate check, but can't return 2 identical values
						static std::uint16_t fakeCounter = 0;

						if (m_ExtraInfo & CUSTOM_NAVMESH_PSEUDODELTE_FLAG)
						{
							fakeCounter = (fakeCounter + 1) % 0xFFFF;
							return fakeCounter;
						}

						return GetVertexIndex(Vertex);
					}
				};

				class EdgeExtraInfo
				{
				public:
					std::uint32_t m_Type;
					std::uint32_t m_NavmeshID;
					std::uint16_t m_TriangleIndex;
				};

				char _pad0[0x10];
				EditorAPI::BSTArray<VertexData> m_Vertices;
				EditorAPI::BSTArray<BSNavmeshTriangle> m_Triangles;
				EditorAPI::BSTArray<EdgeExtraInfo> m_ExtraInfo;

				EdgeExtraInfo* GetEdgeExtraInfo(uint16_t TriangleIndex, std::uint32_t EdgeIndex) noexcept(true)
				{
					CKPE_ASSERT_MSG(TriangleIndex < m_Triangles.QSize(), "Invalid triangle index");
					CKPE_ASSERT_MSG(EdgeIndex < 3, "Invalid edge index");

					if (m_Triangles[TriangleIndex].HasExtraInfo(EdgeIndex))
					{
						uint16_t index = m_Triangles[TriangleIndex].GetEdgeLinkIndex(EdgeIndex);

						if (index < m_ExtraInfo.QSize())
							return &m_ExtraInfo.at(index);
					}

					return nullptr;
				}
			};

			static_assert(sizeof(BSNavmesh::VertexData) == 0xC);
			static_assert(sizeof(BSNavmesh::BSNavmeshTriangle) == 0x10);
			static_assert(sizeof(BSNavmesh::EdgeExtraInfo) == 0xC);

			class NavMesh
			{
			public:
				char _pad0[0x30];
				BSNavmesh m_Data;

				void HKDeleteTriangle(uint16_t TriangleIndex) noexcept(true)
				{
					if (false)
					{
						// Proceed with normal deletion
						(this->*DeleteTriangle)(TriangleIndex);
					}
					else
					{
						auto PathingGetSingleton = (void* (__fastcall*)())pointer_NavMeshPseudoDelete_sub1;
						auto PathingGetForm = (NavMesh* (__fastcall*)(void*, std::uint32_t))pointer_NavMeshPseudoDelete_sub2;

						BSNavmesh::BSNavmeshTriangle& tri = m_Data.m_Triangles.at(TriangleIndex);

						// Mark with custom flag
						tri.m_ExtraInfo |= BSNavmesh::CUSTOM_NAVMESH_PSEUDODELTE_FLAG;

						// Kill all edges referencing this triangle & kill edges this triangle references
						auto removeEdgeReferences = [this, TriangleIndex](BSNavmesh::BSNavmeshTriangle& Triangle)
							{
								for (std::uint32_t i = 0; i < 3; i++)
								{
									if (!Triangle.HasExtraInfo(i) && Triangle.GetEdgeIndex(i) == TriangleIndex)
										Triangle.ClearEdge(i);
								}
							};

						for (int edge = 0; edge < 3; edge++)
						{
							BSNavmesh::EdgeExtraInfo* extraInfo = m_Data.GetEdgeExtraInfo(TriangleIndex, edge);
							std::uint16_t index = (extraInfo) ? extraInfo->m_TriangleIndex : tri.GetEdgeIndex(edge);

							if (index != BSNavmesh::BAD_NAVMESH_TRIANGLE)
							{
								if (extraInfo)
								{
									// Separated mesh link
									CKPE_ASSERT(extraInfo->m_NavmeshID != 0);
									NavMesh* externalMesh = PathingGetForm(PathingGetSingleton(), extraInfo->m_NavmeshID);

									CKPE_ASSERT(externalMesh);
									removeEdgeReferences(externalMesh->m_Data.m_Triangles.at(index));

									extraInfo->m_NavmeshID = 0;
									extraInfo->m_TriangleIndex = BSNavmesh::BAD_NAVMESH_TRIANGLE;
								}
								else
								{
									removeEdgeReferences(m_Data.m_Triangles.at(index));
								}
							}

							tri.ClearEdge(edge);
						}

						// If possible, avoid orphaning vertices when making it degenerate
						std::unordered_map<std::uint16_t, bool> orphans
						{
							{ tri.GetVertexIndex(0), true },
							{ tri.GetVertexIndex(1), true },
							{ tri.GetVertexIndex(2), true },
						};

						for (std::uint32_t i = 0; i < m_Data.m_Triangles.QSize(); i++)
						{
							if (i == TriangleIndex)
								continue;

							orphans[m_Data.m_Triangles[i].GetVertexIndex(0)] = false;
							orphans[m_Data.m_Triangles[i].GetVertexIndex(1)] = false;
							orphans[m_Data.m_Triangles[i].GetVertexIndex(2)] = false;
						}

						std::uint16_t vert = tri.GetVertexIndex(0);

						for (auto [orphanVert, orphaned] : orphans)
						{
							if (!orphaned)
								continue;

							vert = orphanVert;
							break;
						}

						tri.m_Vertices[0] = vert;
						tri.m_Vertices[1] = vert;
						tri.m_Vertices[2] = vert;
					}
				}

				static inline decltype(&HKDeleteTriangle) DeleteTriangle;
			};

			NavMeshPseudoDelete::NavMeshPseudoDelete() : Common::Patch()
			{
				SetName("NavMesh Pseudo Delete");
			}

			bool NavMeshPseudoDelete::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* NavMeshPseudoDelete::GetOptionName() const noexcept(true)
			{
				return "CreationKit:bNavMeshPseudoDelete";
			}

			bool NavMeshPseudoDelete::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> NavMeshPseudoDelete::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool NavMeshPseudoDelete::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool NavMeshPseudoDelete::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				pointer_NavMeshPseudoDelete_sub1 = __CKPE_OFFSET(0);
				pointer_NavMeshPseudoDelete_sub2 = __CKPE_OFFSET(1);

				*(uintptr_t*)&NavMesh::DeleteTriangle =
					Detours::DetourClassJump(__CKPE_OFFSET(2), &NavMesh::HKDeleteTriangle);

				Detours::DetourClassCall(__CKPE_OFFSET(3), &BSNavmesh::BSNavmeshTriangle::HKGetVertexIndex_DegenerateCheck);
				Detours::DetourClassCall(__CKPE_OFFSET(4), &BSNavmesh::BSNavmeshTriangle::HKGetVertexIndex_DegenerateCheck);
				Detours::DetourClassCall(__CKPE_OFFSET(5), &BSNavmesh::BSNavmeshTriangle::HKGetVertexIndex_DegenerateCheck);
				Detours::DetourClassCall(__CKPE_OFFSET(6), &BSNavmesh::BSNavmeshTriangle::HKGetVertexIndex_VertexCheck);
				Detours::DetourClassCall(__CKPE_OFFSET(7), &BSNavmesh::BSNavmeshTriangle::HKGetVertexIndex_VertexCheck);

				// Prevent vertices from being deleted separately
				SafeWrite::Write(__CKPE_OFFSET(8), { 0xE9, 0xA1, 0x01, 0x00, 0x00 });

				return true;
			}
		}
	}
}