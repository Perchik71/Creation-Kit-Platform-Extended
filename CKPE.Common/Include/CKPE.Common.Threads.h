// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <CKPE.Common.Common.h>

namespace CKPE
{
	namespace Common
	{
		class CKPE_COMMON_API Threads
		{
			Threads(const Threads&) = delete;
			Threads& operator=(const Threads&) = delete;
		public:
			constexpr Threads() noexcept(true) = default;

			static void SetThreadName(std::uint32_t ThreadID, const char* ThreadName);
			static bool SetThreadDesc(void* thread, const char* ThreadName);
		public:
			struct CKPE_COMMON_API Hook
			{
				static bool HKSetThreadPriority(void* thread, std::int32_t priority) noexcept(true);
				static std::uintptr_t HKSetThreadAffinityMask(void* thread, std::uintptr_t affinity_mask) noexcept(true);
				static std::uint32_t HKSetThreadIdealProcessor(void* thread, std::uint32_t mask) noexcept(true);
				static void* HKCreateThread(void* attributes, std::uint64_t stack_size, void* start_address,
					void* parameter, std::uint32_t flags, std::uint32_t* id) noexcept(true);
				static void HKSleep(std::uint32_t ms) noexcept(true);
				static void HKSleepEx(std::uint32_t ms, bool alertable) noexcept(true);

				static void Initialize() noexcept(true);
			};
		};
	}
}