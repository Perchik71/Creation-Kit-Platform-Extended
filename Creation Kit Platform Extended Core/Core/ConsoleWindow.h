// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		class Engine;

		class ConsoleWindow
		{
		public:
			ConsoleWindow(Engine* lpEngine);
			~ConsoleWindow();

			static LRESULT CALLBACK WndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

			inline ConcurrencyArray<const char*>& GetPendingMessages() { return _pendingMessages; }
			inline bool HasAutoScroll() const { return _autoScroll; }
			inline void SetAutoScroll(bool value) { _autoScroll = value; }
			inline void Clear() const { SendMessageA(_richEditHwnd, WM_SETTEXT, 0, reinterpret_cast<LPARAM>("")); }
			inline void SetRichEditHandle(HWND richEditHwnd) { _richEditHwnd = richEditHwnd; }
			inline HWND GetRichEditHandle() const { return _richEditHwnd; }
			
			bool SaveRichTextToFile(const char* _filename) const;

			void InputLog(const char* Format, ...);
			void InputLogVa(const char* Format, va_list Va);

			void CloseOutputFile();
			void UpdateWindow() const;
			void BringToFront() const;
			void Hide() const;
		private:
			ConsoleWindow(const ConsoleWindow&) = default;
			ConsoleWindow& operator=(const ConsoleWindow&) = default;

			bool Create();
			void Destroy();

			HWND hWindow;
			Engine* _engine;
			HWND _richEditHwnd;
			bool _autoScroll;
			FILE* _outputFileHandle;
			ConcurrencyArray<const char*> _pendingMessages;
		};

		extern ConsoleWindow* GlobalConsoleWindowPtr;
	}

	void _CONSOLE(const char* fmt, ...);
	void _CONSOLE(const char* fmt, va_list va);
}