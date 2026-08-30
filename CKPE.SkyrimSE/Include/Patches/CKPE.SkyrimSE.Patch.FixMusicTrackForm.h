// Copyright © 2023-2025 aka CKPE team. All rights reserved.
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
			class FixMusicTrackForm : public Common::Patch
			{
				FixMusicTrackForm(const FixMusicTrackForm&) = delete;
				FixMusicTrackForm& operator=(const FixMusicTrackForm&) = delete;
			protected:
				virtual bool DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				FixMusicTrackForm();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);
				bool SupportsAddressLibrary() const noexcept(true) override;

				static std::size_t sub(const char* string) noexcept(true);
			};
		}
	}
}