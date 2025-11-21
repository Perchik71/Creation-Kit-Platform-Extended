// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Segment.h>
#include <CKPE.Common.Patch.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			class ReplaceBSPointerHandleAndManager : public Common::Patch
			{
				bool Install_163(Common::RelocatorDB::PatchDB* db, bool Extremly) noexcept(true);
				bool Install_980(Common::RelocatorDB::PatchDB* db, bool Extremly) noexcept(true);
				bool Install_137(Common::RelocatorDB::PatchDB* db, bool Extremly) noexcept(true);

				static std::uint32_t CheckEx(std::uintptr_t unused, std::uintptr_t refr) noexcept(true);
				static std::uint32_t Check(std::uintptr_t unused, std::uintptr_t refr) noexcept(true);
				
				static void IncRefPatch(const Segment& t) noexcept(true);
				static void DecRefPatch(const Segment& t) noexcept(true);

				ReplaceBSPointerHandleAndManager(const ReplaceBSPointerHandleAndManager&) = delete;
				ReplaceBSPointerHandleAndManager& operator=(const ReplaceBSPointerHandleAndManager&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				ReplaceBSPointerHandleAndManager();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);
			};
		}
	}
}