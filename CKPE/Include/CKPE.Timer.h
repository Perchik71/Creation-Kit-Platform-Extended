// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <CKPE.Common.h>

namespace CKPE
{
	class CKPE_API Timer
	{
		std::int64_t perfCounter{ 0 };
		double perfFreq{ 0.0 };

		Timer(const Timer&) = delete;
		Timer& operator=(const Timer&) = delete;
	public:
		Timer() noexcept(true);
		virtual ~Timer() noexcept(true) = default;

		virtual void Start() noexcept(true);
		virtual void Reset() noexcept(true);
		virtual double Get() const noexcept(true);
	};
}