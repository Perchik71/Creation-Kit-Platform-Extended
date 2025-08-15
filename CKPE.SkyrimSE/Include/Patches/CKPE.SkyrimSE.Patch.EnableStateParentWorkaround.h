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
			class EnableStateParentWorkaround : public Common::Patch
			{
				EnableStateParentWorkaround(const EnableStateParentWorkaround&) = delete;
				EnableStateParentWorkaround& operator=(const EnableStateParentWorkaround&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				EnableStateParentWorkaround();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				static void sub(std::int64_t RenderWindowInstance,
					std::uint32_t* UntypedPointerHandle, bool Select) noexcept(true);
			};
		}
	}
}