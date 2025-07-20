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
			class WeaponCriticalEffect : public Common::Patch
			{
				WeaponCriticalEffect(const WeaponCriticalEffect&) = delete;
				WeaponCriticalEffect& operator=(const WeaponCriticalEffect&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				WeaponCriticalEffect();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				static void sub1(std::int64_t DiskCRDT, std::int64_t SourceCRDT) noexcept(true);
				static void sub2(std::int64_t TESFile, std::int64_t SourceCRDT) noexcept(true);
			};
		}
	}
}