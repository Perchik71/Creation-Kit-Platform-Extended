// Copyright © 2026 aka CKPE team. All rights reserved.
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
			class FixAcousticSpaceMarker : public Common::Patch
			{
				static bool sub(void* form) noexcept(true);

				FixAcousticSpaceMarker(const FixAcousticSpaceMarker&) = delete;
				FixAcousticSpaceMarker& operator=(const FixAcousticSpaceMarker&) = delete;
			protected:
				virtual bool DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				FixAcousticSpaceMarker();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);
				[[nodiscard]] TEnum<Method> GetMethods() const noexcept(true) override;
			};
		}
	}
}
