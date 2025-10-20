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
			class FixQuoteCmdLine : public Common::Patch
			{
				static char* sub(char* str, const char* delim, char** next_token) noexcept(true);
				static void sub2(char* lpCmdLine, char* arg2) noexcept(true);

				FixQuoteCmdLine(const FixQuoteCmdLine&) = delete;
				FixQuoteCmdLine& operator=(const FixQuoteCmdLine&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				FixQuoteCmdLine();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);
			};
		}
	}
}