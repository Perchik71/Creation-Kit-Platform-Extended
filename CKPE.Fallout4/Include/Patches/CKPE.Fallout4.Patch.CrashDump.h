// Copyright © 2023-2025 aka perchik71. All rights reserved.
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
			class CrashDump : public Common::Patch
			{
				CrashDump(const CrashDump&) = delete;
				CrashDump& operator=(const CrashDump&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				CrashDump();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				static void DoOutputVersion(std::string& Result) noexcept(true);
				static void DoAnalyzeClassRef(std::uintptr_t Address, const char* RttiName, std::string& Result);
				static void DoOutputCKVersion(std::string& Result) noexcept(true);
			};
		}
	}
}