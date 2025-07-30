// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdio>
#include <cstdint>
#include <string>
#include <CKPE.Common.Common.h>

namespace CKPE
{
	namespace Common
	{
		class CKPE_COMMON_API CrashHandler
		{
			CrashHandler(const CrashHandler&) = delete;
			CrashHandler& operator=(const CrashHandler&) = delete;

			void SetProcessExceptionHandlers() noexcept(true);
			void SetThreadExceptionHandlers() noexcept(true);

			void AssertWithCrashReport(const char* File, std::int32_t Line, const char* ErrorMessage, 
				void* ExceptionInfo) noexcept(true);
		public:
			typedef void (*TAnalyzeClassRefHandler)(std::uintptr_t Address, const char* RttiName, std::string& Result);
			TAnalyzeClassRefHandler OnAnalyzeClassRef{ nullptr };
			typedef void (*TOutputVersionHandler)(std::string& Result);
			TOutputVersionHandler OnOutputVersion{ nullptr };

			CrashHandler() noexcept(true) = default;
			virtual ~CrashHandler() noexcept(true) = default;

			void Install() noexcept(true);

			static CrashHandler* GetSingleton() noexcept(true);
		};
	}
}