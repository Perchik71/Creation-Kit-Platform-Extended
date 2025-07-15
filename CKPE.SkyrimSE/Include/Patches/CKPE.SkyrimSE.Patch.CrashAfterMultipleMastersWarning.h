// Copyright © 2023-2024 aka perchik71. All rights reserved.
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
			class CrashAfterMultipleMastersWarning : public Common::Patch
			{
				CrashAfterMultipleMastersWarning(const CrashAfterMultipleMastersWarning&) = delete;
				CrashAfterMultipleMastersWarning& operator=(const CrashAfterMultipleMastersWarning&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				CrashAfterMultipleMastersWarning();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				static void sub(std::int64_t a1) noexcept(true);
			};
		}
	}
}