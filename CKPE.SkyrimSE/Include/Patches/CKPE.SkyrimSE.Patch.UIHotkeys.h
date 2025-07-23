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
			class UIHotkeys : public Common::Patch
			{
				UIHotkeys(const UIHotkeys&) = delete;
				UIHotkeys& operator=(const UIHotkeys&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				UIHotkeys();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				static void sub(void* Thisptr, void(*Callback)(), const char** HotkeyFunction,
					const char** DisplayText, char VirtualKey, bool Alt, bool Ctrl, bool Shift) noexcept(true);
			};
		}
	}
}