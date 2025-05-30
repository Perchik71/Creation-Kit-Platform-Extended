// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <CKPE.Module.h>

namespace CKPE
{
	class CKPE_API Process : public Module
	{
		void* _process{ nullptr };
		std::uint32_t _processid{ (std::uint32_t)-1 };

		Process(const Process&) = delete;
		Process& operator=(const Process&) = delete;
	public:
		Process() noexcept(true);

		[[nodiscard]] inline constexpr virtual const void* GetHandle() const noexcept(true) { return _process; }
		[[nodiscard]] inline constexpr virtual std::uint32_t GetID() const noexcept(true) { return _processid; }

		virtual void Terminate() const noexcept(true);
	};
}