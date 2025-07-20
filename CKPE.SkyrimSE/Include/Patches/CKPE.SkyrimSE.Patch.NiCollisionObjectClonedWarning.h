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
			class NiCollisionObjectClonedWarning : public Common::Patch
			{
				NiCollisionObjectClonedWarning(const NiCollisionObjectClonedWarning&) = delete;
				NiCollisionObjectClonedWarning& operator=(const NiCollisionObjectClonedWarning&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				NiCollisionObjectClonedWarning();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				static void sub(std::int64_t SourceNode, std::int64_t DestNode, 
					std::int64_t CloningProcess) noexcept(true);
			};
		}
	}
}