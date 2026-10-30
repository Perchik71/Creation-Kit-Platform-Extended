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
			class PreviewRenderTargets : public Common::Patch
			{
				PreviewRenderTargets(const PreviewRenderTargets&) = delete;
				PreviewRenderTargets& operator=(const PreviewRenderTargets&) = delete;
			protected:
				bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true) override;
				bool DoQuery() const noexcept(true) override;
			public:
				PreviewRenderTargets();

				bool HasOption() const noexcept(true) override;
				const char* GetOptionName() const noexcept(true) override;
				bool HasDependencies() const noexcept(true) override;
				std::vector<std::string> GetDependencies() const noexcept(true) override;
				[[nodiscard]] bool SupportsAddressLibrary() const noexcept(true) override
				{
					return true;
				}
			};
		}
	}
}
