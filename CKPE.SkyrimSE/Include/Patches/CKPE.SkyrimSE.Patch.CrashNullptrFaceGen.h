// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.Patch.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			class CrashNullptrFaceGen : public Common::Patch
			{
				CrashNullptrFaceGen(const CrashNullptrFaceGen&) = delete;
				CrashNullptrFaceGen& operator=(const CrashNullptrFaceGen&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				CrashNullptrFaceGen();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				static bool sub(const char* path, std::uintptr_t* a2, std::uintptr_t* a3) noexcept(true);
			};
		}
	}
}