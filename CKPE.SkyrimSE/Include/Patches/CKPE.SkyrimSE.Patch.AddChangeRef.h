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
			class AddChangeRef : public Common::Patch
			{
				AddChangeRef(const AddChangeRef&) = delete;
				AddChangeRef& operator=(const AddChangeRef&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				AddChangeRef();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				static bool HKInsertMenuA(void* hMenu, std::uint32_t uPosition, std::uint32_t uFlags,
					std::uintptr_t uIDNewItem, const char* lpNewItem);
				static bool HKDeleteMenu(void* hMenu, std::uint32_t uPosition, std::uint32_t uFlags);
			};
		}
	}
}