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
			class CrashMHDTMoreThan70 : public Common::Patch
			{
				CrashMHDTMoreThan70(const CrashMHDTMoreThan70&) = delete;
				CrashMHDTMoreThan70& operator=(const CrashMHDTMoreThan70&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				CrashMHDTMoreThan70();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				static void* sub1(void* world, int x, int y) noexcept(true);
				static void sub2(void* shape, void* vec, void* point, void* unk1, int unk2) noexcept(true);
			};
		}
	}
}