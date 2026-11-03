// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.Patch.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			class BGSRenderWindow;
			class BGSRenderWindowCamera;
			namespace NiAPI
			{
				class NiCamera;
				class NiFrustum;
			}
		}

		namespace Patch
		{
			class FixWaterOrtho : public Common::Patch
			{
				FixWaterOrtho(const FixWaterOrtho&) = delete;
				FixWaterOrtho& operator=(const FixWaterOrtho&) = delete;
			protected:
				virtual bool DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				FixWaterOrtho();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);
				bool SupportsAddressLibrary() const noexcept(true) override;

				static void InitOrthoFrustum(EditorAPI::NiAPI::NiFrustum* a1, bool a2) noexcept(true);
				static void RestorePerspective(EditorAPI::NiAPI::NiCamera* a1, const EditorAPI::NiAPI::NiFrustum* a2) noexcept(true);
				static EditorAPI::BGSRenderWindowCamera* SetViewModeAndUpdateTitle(EditorAPI::BGSRenderWindowCamera* a1, std::uint32_t a2) noexcept(true);
				static void ResizeRenderWindow(EditorAPI::BGSRenderWindow* a1, std::int32_t a2, std::int32_t a3) noexcept(true);
			};
		}
	}
}