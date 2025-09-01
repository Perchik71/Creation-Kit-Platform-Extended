// Copyright © 2023-2024 aka perchik71. All rights reserved.
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
			class RTDynamicCastCrash : public Common::Patch
			{
				RTDynamicCastCrash(const RTDynamicCastCrash&) = delete;
				RTDynamicCastCrash& operator=(const RTDynamicCastCrash&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);

				static bool sub(std::int64_t a1, std::int64_t a2, std::int64_t a3, std::int64_t a4);
			public:
				RTDynamicCastCrash();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);
			};
		}
	}
}