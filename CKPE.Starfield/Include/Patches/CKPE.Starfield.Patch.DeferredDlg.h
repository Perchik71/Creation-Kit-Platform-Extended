// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.Patch.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
			class DeferredDlg : public Common::Patch
			{
				static void sub1(void* Thisptr, void* ControlHandle, std::int64_t Unknown1, std::int64_t Unknown2) noexcept(true);
				static void sub2(void* Thisptr, void* ControlHandle, std::int64_t Unknown1, std::int64_t Unknown2) noexcept(true);
				static void sub3(void* Thisptr, void** ControlHandle) noexcept(true);
				/*static void deferredCombobox(std::int64_t a1, std::int64_t a2, std::int64_t a3, std::int64_t a4, 
					std::int64_t a5, std::int64_t a6, std::int64_t a7, std::int64_t a8) noexcept(true);*/

				DeferredDlg(const DeferredDlg&) = delete;
				DeferredDlg& operator=(const DeferredDlg&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				DeferredDlg();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);
			};
		}
	}
}