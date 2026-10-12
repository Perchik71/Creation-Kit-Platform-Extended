// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <algorithm>
#include <unordered_set>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/B/BGSRenderWindow.h>
#include <EditorAPI/B/BSLightingShaderProperty.h>
#include <EditorAPI/B/BSRenderPass.h>
#include <Patches/CKPE.SkyrimSE.Patch.TintSelectedObjects.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			namespace
			{
				using SetupGeometryFn = std::uint64_t(*)(void*, void*, std::uint64_t, std::uint64_t);
				using GetRefFromGeometryFn = EditorAPI::Forms::TESObjectREFR* (*)(void*);

				SetupGeometryFn g_OrigSetupGeometry = nullptr;
				GetRefFromGeometryFn g_GetRefFromGeometry = nullptr;

				// Colour added to the emissive term of every selected geometry (emissive colour * multiplier)
				EditorAPI::NiAPI::NiColor g_SelectionColor{ 0.0f, 0.35f, 0.0f };

				// Selected references are cached; the pick list is re-read at most every kRefreshMs.
				constexpr ULONGLONG kRefreshMs = 30;
				std::unordered_set<const void*> g_SelectedRefs;
				ULONGLONG g_LastRefresh = 0;

				void RefreshSelectedRefs(EditorAPI::BGSRenderWindow* Renderer) noexcept(true)
				{
					auto Now = GetTickCount64();
					if (g_LastRefresh && ((Now - g_LastRefresh) < kRefreshMs))
						return;

					g_LastRefresh = Now;
					g_SelectedRefs.clear();

					auto Pick = Renderer->GetPickHandler();
					if (!Pick || !Pick->GetCount() || !Pick->GetItems())
						return;

					// The list is a circular chain of Count nodes, same walk as BGSPickHandler::Has
					auto It = Pick->GetItems()->GetFirst();
					for (std::uint32_t i = 0; It && (i < Pick->GetCount()); i++, It = It->GetNext())
					{
						// BGSPickNode::GetRef does not tolerate a handle that no longer resolves
						if (!EditorAPI::GetRefFormByUniqueId(It->GetIndex()))
							continue;

						auto Ref = EditorAPI::BGSRenderWindow::GetRef(It->GetIndex());
						if (Ref)
							g_SelectedRefs.insert(Ref);
					}
				}

				bool IsSelectedGeometry(void* Geometry) noexcept(true)
				{
					if (!Geometry)
						return false;

					auto Renderer = EditorAPI::BGSRenderWindow::Singleton.Singleton;
					if (!Renderer)
						return false;

					auto Pick = Renderer->GetPickHandler();
					if (!Pick || !Pick->GetCount())
					{
						g_LastRefresh = 0;
						return false;
					}

					RefreshSelectedRefs(Renderer);
					if (g_SelectedRefs.empty())
						return false;

					auto Ref = g_GetRefFromGeometry(Geometry);
					return Ref && g_SelectedRefs.contains(Ref);
				}
			}

			TintSelectedObjects::TintSelectedObjects() : Common::Patch()
			{
				SetName("Tint Selected Objects");
			}

			bool TintSelectedObjects::HasOption() const noexcept(true)
			{
				return true;
			}

			const char* TintSelectedObjects::GetOptionName() const noexcept(true)
			{
				return "CreationKit:bTintSelectedObjects";
			}

			bool TintSelectedObjects::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> TintSelectedObjects::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool TintSelectedObjects::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool TintSelectedObjects::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool TintSelectedObjects::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Highlights the objects selected in the Render Window (the Fallout 4 Creation Kit does this with a green
				// tint, "bTintSelectedObjects"). The Skyrim lighting pixel shader has no selection effect, so the highlight
				// is the shader's own emissive term: BSLightingShader::SetupGeometry writes
				// "PropertyColor = *BSLightingShaderProperty::pEmissiveColor * fEmissiveMultiple" to the per-geometry pixel
				// shader constant buffer, and for the geometry of a selected reference the property's emissive colour is
				// swapped for (original + selection colour) around the original call.
				//
				// BSShader virtual 6 is SetupGeometry in every supported build (the vtable of BSLightingShader is ID 150860).
				//
				g_SelectionColor.r = std::clamp(_READ_OPTION_FLOAT("CreationKit", "fSelectionTintColorR", 0.0f), 0.0f, 1.0f);
				g_SelectionColor.g = std::clamp(_READ_OPTION_FLOAT("CreationKit", "fSelectionTintColorG", 1.0f), 0.0f, 1.0f);
				g_SelectionColor.b = std::clamp(_READ_OPTION_FLOAT("CreationKit", "fSelectionTintColorB", 0.0f), 0.0f, 1.0f);

				auto Intensity = std::clamp(_READ_OPTION_FLOAT("CreationKit", "fSelectionTintIntensity", 0.35f), 0.0f, 4.0f);
				g_SelectionColor.r *= Intensity;
				g_SelectionColor.g *= Intensity;
				g_SelectionColor.b *= Intensity;

				g_GetRefFromGeometry = (GetRefFromGeometryFn)ID(111020).Address();
				g_OrigSetupGeometry = (SetupGeometryFn)Relocation(ID(150860)).WriteVFunc(6, &TintSelectedObjects::HKSetupGeometry);

				return true;
			}

			std::uint64_t TintSelectedObjects::HKSetupGeometry(void* Shader, void* Pass, std::uint64_t Arg3,
				std::uint64_t Arg4) noexcept(true)
			{
				auto RenderPass = static_cast<EditorAPI::BSRenderPass*>(Pass);
				if (!RenderPass || !RenderPass->pShaderProperty || !IsSelectedGeometry(RenderPass->pGeometry))
					return g_OrigSetupGeometry(Shader, Pass, Arg3, Arg4);

				auto Property = static_cast<EditorAPI::BSLightingShaderProperty*>(RenderPass->pShaderProperty);
				auto OrigColor = Property->pEmissiveColor;
				auto OrigMultiple = Property->fEmissiveMultiple;
				if (!OrigColor)
					return g_OrigSetupGeometry(Shader, Pass, Arg3, Arg4);

				// Never write through the original colour, it may be shared between properties
				EditorAPI::NiAPI::NiColor Combined
				{
					OrigColor->r * OrigMultiple + g_SelectionColor.r,
					OrigColor->g * OrigMultiple + g_SelectionColor.g,
					OrigColor->b * OrigMultiple + g_SelectionColor.b
				};

				Property->pEmissiveColor = &Combined;
				Property->fEmissiveMultiple = 1.0f;
				auto Result = g_OrigSetupGeometry(Shader, Pass, Arg3, Arg4);
				Property->pEmissiveColor = OrigColor;
				Property->fEmissiveMultiple = OrigMultiple;

				return Result;
			}
		}
	}
}
