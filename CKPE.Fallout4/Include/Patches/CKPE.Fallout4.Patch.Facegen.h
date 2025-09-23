// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.Patch.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			class Facegen : public Common::Patch
			{
				Facegen(const Facegen&) = delete;
				Facegen& operator=(const Facegen&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				Facegen();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				static void CreateDiffuseCompressDDS(std::int64_t lpThis, std::uint32_t TextureId, const char* lpFileName,
					std::int32_t Unk1, bool Unk2) noexcept(true);
				static void CreateNormalsCompressDDS(std::int64_t lpThis, std::uint32_t TextureId, const char* lpFileName,
					std::int32_t Unk1, bool Unk2) noexcept(true);
				static void CreateSpecularCompressDDS(std::int64_t lpThis, std::uint32_t TextureId, const char* lpFileName,
					std::int32_t Unk1, bool Unk2) noexcept(true);
			};
		}
	}
}