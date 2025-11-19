// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.Patch.h>
#include <EditorAPI/BSIStream.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			class LoadShaders : public Common::Patch
			{
				static EditorAPI::OG_NG::BSIStream* CreateStream_OG_NG(EditorAPI::OG_NG::BSIStream* Stream, const char* FileName,
					std::uint32_t unk1 = 0, bool unk2 = true) noexcept(true);

				static EditorAPI::AE::BSIStream* CreateStream_AE(EditorAPI::AE::BSIStream* Stream, const char* FileName,
					std::uint32_t unk1 = 0, bool unk2 = true) noexcept(true);

				LoadShaders(const LoadShaders&) = delete;
				LoadShaders& operator=(const LoadShaders&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				LoadShaders();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);
			};
		}
	}
}