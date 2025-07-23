// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <cstring>
#include <CKPE.Common.Common.h>

namespace CKPE
{
	namespace Common
	{
		class CKPE_COMMON_API ModernTheme
		{
			bool _caption_styles_supported{ false };

			ModernTheme(const ModernTheme&) = delete;
			ModernTheme& operator=(const ModernTheme&) = delete;
		public:
			constexpr ModernTheme() noexcept(true) = default;

			[[nodiscard]] constexpr inline bool HasCaptionStylesSupported() const noexcept(true)
			{ return _caption_styles_supported; }

			static void AddSpermanentWindowSubclass(const char* Name) noexcept(true);
			static void InitializeCurrentThread() noexcept(true);
			static void InitializeThread(uint32_t u32ThreadId) noexcept(true);
			static void ApplyDarkThemeForWindow(void* hWnd) noexcept(true);

			static ModernTheme* GetSingleton() noexcept(true);
		public:
			struct CKPE_COMMON_API Hook
			{
				static void Initialize(bool support_more_theme = true) noexcept(true);
			};
		};
	}
}