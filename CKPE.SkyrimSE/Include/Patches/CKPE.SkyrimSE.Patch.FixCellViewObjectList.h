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
			class FixCellViewObjectList : public Common::Patch
			{
				FixCellViewObjectList(const FixCellViewObjectList&) = delete;
				FixCellViewObjectList& operator=(const FixCellViewObjectList&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				FixCellViewObjectList();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				static void ListViewSelectItem(void* ListViewHandle, std::int32_t ItemIndex, 
					bool KeepOtherSelections) noexcept(true);
				static void ListViewFindAndSelectItem(void* ListViewHandle, void* Parameter, 
					bool KeepOtherSelections) noexcept(true);
				static void ListViewDeselectItem(void* ListViewHandle, void* Parameter) noexcept(true);
				static void sub1(std::uint64_t a1, std::uint64_t a2, std::uint64_t a3) noexcept(true);
			};
		}
	}
}