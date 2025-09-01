// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.Patch.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
			class LoadMaterialsQSplash : public Common::Patch
			{
				LoadMaterialsQSplash(const LoadMaterialsQSplash&) = delete;
				LoadMaterialsQSplash& operator=(const LoadMaterialsQSplash&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);

				static void hk_finish(std::int64_t logoWin, std::int64_t mainWin) noexcept(true);
			public:
				LoadMaterialsQSplash();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);
			};
		}
	}
}