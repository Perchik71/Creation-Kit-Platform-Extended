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
			class FixCrashMakeXYZCircles : public Common::Patch
			{
				FixCrashMakeXYZCircles(const FixCrashMakeXYZCircles&) = delete;
				FixCrashMakeXYZCircles& operator=(const FixCrashMakeXYZCircles&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				FixCrashMakeXYZCircles();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				static void* sub(std::int64_t ResourceManager, std::uint32_t IndexCount, std::int64_t a3, std::int64_t a4,
					std::int64_t a5, std::int64_t a6, std::int64_t a7) noexcept(true);
			};
		}
	}
}