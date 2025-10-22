// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.Patch.h>
#include <EditorAPI/Forms/TESObjectREFR.h>
#include <EditorAPI/NiAPI/NiPoint.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			class FakeMoveLight : public Common::Patch
			{
				static void SetPosition(EditorAPI::Forms::TESObjectREFR* refr,
					EditorAPI::NiAPI::NiPoint3* pos, EditorAPI::NiAPI::NiPoint3* dir) noexcept(true);

				FakeMoveLight(const FakeMoveLight&) = delete;
				FakeMoveLight& operator=(const FakeMoveLight&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				FakeMoveLight();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);
			};
		}
	}
}