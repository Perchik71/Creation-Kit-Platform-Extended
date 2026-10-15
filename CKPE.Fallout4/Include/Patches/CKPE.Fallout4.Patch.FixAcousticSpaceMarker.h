// Copyright © 2026 aka CKPE team. All rights reserved.
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
			class FixAcousticSpaceMarker : public Common::Patch
			{
				static std::uintptr_t sub(std::uintptr_t a1, std::uintptr_t a2, std::uintptr_t a3, std::uintptr_t a4) noexcept(true);

				FixAcousticSpaceMarker(const FixAcousticSpaceMarker&) = delete;
				FixAcousticSpaceMarker& operator=(const FixAcousticSpaceMarker&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				FixAcousticSpaceMarker();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);
				[[nodiscard]] bool SupportsAddressLibrary() const noexcept(true) override { return true; }
			};
		}
	}
}
