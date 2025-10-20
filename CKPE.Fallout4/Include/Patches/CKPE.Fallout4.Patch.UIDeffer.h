// Copyright © 2024-2025 aka perchik71. All rights reserved.
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
			class UIDeffer : public Common::Patch
			{
				static void sub1(void* Thisptr, void* ControlHandle, std::int64_t Unknown) noexcept(true);
				static void sub2(void* Thisptr, void* ControlHandle, std::int64_t Unknown) noexcept(true);
				static void sub3(void* Thisptr, void** ControlHandle) noexcept(true);

				UIDeffer(const UIDeffer&) = delete;
				UIDeffer& operator=(const UIDeffer&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				UIDeffer();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);
			};
		}
	}
}