// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.htmll

#pragma once

#include <CKPE.Common.Common.h>
#include <cstdint>
#include <cstdio>

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

			bool Create();
			void Destroy();
			bool CreateStdoutListener();
			void LoadWarningBlacklist();

			LogWindow(const LogWindow&) = delete;
			LogWindow& operator=(const LogWindow&) = delete;
		public:
			LogWindow();
			virtual ~LogWindow();

			[[nodiscard]] static LogWindow* GetSingleton() noexcept(true);
			[[nodiscard]] constexpr inline void* GetHandle() const noexcept(true) { return _handle; }
			[[nodiscard]] constexpr inline void* GetRichEditHandle() const noexcept(true) { return _handle_richedit; }

			bool Initialize(const void* create_struct);	
			void Show(bool sh) const noexcept(true);
			void Update() const noexcept(true);
			void Clear() const noexcept(true);
			[[nodiscard]] constexpr inline bool HasAutoScroll() const noexcept(true) { return _auto_scroll; }
			constexpr inline void SetAutoScroll(bool v) noexcept(true) { _auto_scroll = v; }

			void ReleaseHandler() noexcept(true);
			void ResizeHandler(std::uint32_t x, std::uint32_t y) const noexcept(true);
			void ActiveHandler(bool active) const noexcept(true);
			void CloseHandler() const noexcept(true);

			/*


			bool SaveRichTextToFile(const char* _filename) const;
			void LoadWarningBlacklist();

			inline HANDLE GetStdoutListenerPipe() const { return _ExternalPipeWriterHandle; }
			bool CreateStdoutListener();

			virtual void InputLog(const char* Format, ...);
			virtual void InputLogVa(const char* Format, va_list Va);

			void CloseOutputFile();*/
		private:
			/*

			

			uint64_t HashLastMsg;
			Engine* _engine;
			HWND _richEditHwnd;
			bool _autoScroll;
			FILE* _outputFileHandle;
			HANDLE _ExternalPipeWriterHandle;
			HANDLE _ExternalPipeReaderHandle;
			*/
		};

		void _CONSOLE(const char* fmt, ...);
		void _CONSOLEVA(const char* fmt, va_list va);
	}
}