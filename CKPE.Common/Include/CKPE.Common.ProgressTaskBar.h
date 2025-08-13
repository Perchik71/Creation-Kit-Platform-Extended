// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.Common.h>

namespace CKPE
{
	namespace Common
	{
		class CKPE_COMMON_API ProgressTaskBar
		{
			void* LinkWnd{ nullptr };
		public:
			ProgressTaskBar(void* LinkWnd, std::uint32_t Total = 95) noexcept(true);
			~ProgressTaskBar() noexcept(true);

			virtual void Begin() noexcept(true);
			virtual void End() noexcept(true);
			virtual void SetMarquee(bool Value) noexcept(true);
			virtual void SetPosition(std::uint32_t Completed) noexcept(true);
			virtual void Step() noexcept(true);
			virtual void SetTotal(std::uint32_t Total) noexcept(true);
			virtual void Reset() noexcept(true);
		};
	}
}

