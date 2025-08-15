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
			class CrashInventoryIterators : public Common::Patch
			{
				CrashInventoryIterators(const CrashInventoryIterators&) = delete;
				CrashInventoryIterators& operator=(const CrashInventoryIterators&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				CrashInventoryIterators();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				static std::int32_t sub1(std::int64_t a1, std::int64_t a2) noexcept(true);
				static std::int32_t sub2(std::int64_t a1, std::int64_t a2) noexcept(true);
				static std::int32_t sub1_Extremly(std::int64_t a1, std::int64_t a2) noexcept(true);
				static std::int32_t sub2_Extremly(std::int64_t a1, std::int64_t a2) noexcept(true);
			};
		}
	}
}