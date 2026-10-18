// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <CKPE.Common.Relocation.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// Object Window helpers.
			namespace ObjectWindow
			{
				// 1.10.162.0 only: inserts a form into the object list, as called by the patch with (ObjectListInsertData, Form).
				// Not verified against the 1.10.162.0 database.
				inline std::int32_t InsertForm(std::int64_t ListInsertData, void* Form) noexcept(true)
				{
					using func_t = decltype(&InsertForm);
					static Common::Relocation<func_t> func{ Common::ID{ 636375 } };
					return func(ListInsertData, Form);
				}

				// 1.10.982.3 and later: filter text match (concatenates Text and Filter and runs the matcher; the patch passes Mode 0).
				// CK 1.11.246.0 0x14243A610.
				inline std::int32_t MatchFilterText(const char* Text, const char* Filter, std::uint32_t Mode = 0) noexcept(true)
				{
					using func_t = decltype(&MatchFilterText);
					static Common::Relocation<func_t> func{ Common::ID{ 1613291 } };
					return func(Text, Filter, Mode);
				}
			}
		}
	}
}
