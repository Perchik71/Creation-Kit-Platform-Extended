// Copyright © 2025 aka perchik71. All rights reserved.
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
			class CrashInvalidStrings : public Common::Patch
			{
				static std::uint32_t GetLocalizeStringLengthSafe(void* This);
				static const char* GetLocalizeStringSafe(void* This);

				CrashInvalidStrings(const CrashInvalidStrings&) = delete;
				CrashInvalidStrings& operator=(const CrashInvalidStrings&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				CrashInvalidStrings();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);
			};
		}
	}
}