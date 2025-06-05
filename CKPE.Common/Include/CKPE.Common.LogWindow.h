// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.htmll

#pragma once

#include <CKPE.Common.Common.h>
#include <cstdint>
#include <cstdio>
#include <string_view>
#include <string>

namespace CKPE
{
	namespace Common
	{
		class CKPE_COMMON_API LogWindow
		{
			bool _auto_scroll{ true };
			void* _handle{ nullptr };
			void* _handle_richedit{ nullptr };
			void* _external_pipereader_handler{ nullptr };
			void* _external_pipewriter_handler{ nullptr };
			FILE* _output_file{ nullptr };
			std::uint64_t _last_hash{ 0 };

			bool Create();
			void Destroy();
			void LoadWarningBlacklist() noexcept(true);

			LogWindow(const LogWindow&) = delete;
			LogWindow& operator=(const LogWindow&) = delete;
		public:
			typedef void(__stdcall* OnOpenFormByIdHandler)(std::uint32_t id);

			LogWindow();
			virtual ~LogWindow();

			static void Shutdown() noexcept(true);

			[[nodiscard]] static LogWindow* GetSingleton() noexcept(true);
			[[nodiscard]] constexpr inline void* GetHandle() const noexcept(true) { return _handle; }
			[[nodiscard]] constexpr inline void* GetRichEditHandle() const noexcept(true) { return _handle_richedit; }

			bool Initialize(void* handle, const void* create_struct);
			void Show(bool sh) const noexcept(true);
			void Update() const noexcept(true);
			void Clear() const noexcept(true);
			[[nodiscard]] constexpr inline bool HasAutoScroll() const noexcept(true) { return _auto_scroll; }
			constexpr inline void SetAutoScroll(bool v) noexcept(true) { _auto_scroll = v; }

			void ReleaseHandler() noexcept(true);
			void ResizeHandler(std::uint32_t x, std::uint32_t y) const noexcept(true);
			void ActiveHandler(bool active) const noexcept(true);
			void CloseHandler() const noexcept(true);

			bool CreateStdoutListener() noexcept(true);
			void CloseOutputFile() noexcept(true);

			bool SaveRichTextToFile(const std::string& fname) const noexcept(true);
			bool SaveRichTextToFile(const std::wstring& fname) const noexcept(true);
			
			[[nodiscard]] constexpr inline void* GetStdoutListenerPipe() const noexcept(true) 
			{ return _external_pipewriter_handler; }

			virtual void InputLog(const std::string_view& formatted_message, ...);
			virtual void InputLogVa(const std::string_view& formatted_message, va_list va);
			virtual void InputLog(const std::wstring_view& formatted_message, ...);
			virtual void InputLogVa(const std::wstring_view& formatted_message, va_list va);

			OnOpenFormByIdHandler OnOpenFormById{ nullptr };
		};
	}

	CKPE_COMMON_API void _CONSOLE(const std::string_view& formatted_message, ...) noexcept(true);
	CKPE_COMMON_API void _CONSOLEVA(const std::string_view& formatted_message, va_list va) noexcept(true);
	CKPE_COMMON_API void _CONSOLE(const std::wstring_view& formatted_message, ...) noexcept(true);
	CKPE_COMMON_API void _CONSOLEVA(const std::wstring_view& formatted_message, va_list va) noexcept(true);
}