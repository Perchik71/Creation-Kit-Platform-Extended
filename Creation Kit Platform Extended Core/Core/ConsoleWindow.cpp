// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Engine.h"
#include "Editor API/EditorUI.h"
#include "Patches/UIThemePatch.h"
#include "Patches/Windows/SSE/MainWindow.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		constexpr static auto FILE_NONE = L"none";
		constexpr static auto UI_LOG_CMD_ADDTEXT = 0x23000;
		constexpr static auto UI_LOG_CMD_CLEARTEXT = 0x23001;
		constexpr static auto UI_LOG_CMD_AUTOSCROLL = 0x23002;

		constexpr static size_t LINECOUNT_MAX = 50000;

		ConsoleWindow* GlobalConsoleWindowPtr = nullptr;

		ConsoleWindow::ConsoleWindow(Engine* lpEngine) : _engine(nullptr), hWindow(NULL),
			_richEditHwnd(NULL), _autoScroll(true), _outputFileHandle(nullptr), _ExternalPipeWriterHandle(NULL),
			_ExternalPipeReaderHandle(NULL), HashLastMsg(0)
		{
			Create();
		}
	
		ConsoleWindow::~ConsoleWindow()
		{
			Destroy();
		}

		void ConsoleWindow::Clear() const 
		{ 
			CHARRANGE range
			{
				.cpMin = (LONG)SendMessageA(_richEditHwnd, EM_LINEINDEX, 7, 0),
				.cpMax = LONG_MAX,
			};

			SendMessageA(_richEditHwnd, EM_EXSETSEL, 0, reinterpret_cast<LPARAM>(&range));
			SendMessageA(_richEditHwnd, EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(""));
		}

		bool ConsoleWindow::CreateStdoutListener()
		{
			SECURITY_ATTRIBUTES saAttr
			{
				.nLength = sizeof(SECURITY_ATTRIBUTES),
				.lpSecurityDescriptor = nullptr,
				.bInheritHandle = TRUE,
			};

			if (!CreatePipe(&_ExternalPipeReaderHandle, &_ExternalPipeWriterHandle, &saAttr, 0))
				return false;

			// Ensure the read handle to the pipe for STDOUT is not inherited
			if (!SetHandleInformation(_ExternalPipeReaderHandle, HANDLE_FLAG_INHERIT, 0))
				return false;

			std::thread pipeReader([](HANDLE ExternalPipeReaderHandle, HANDLE ExternalPipeWriterHandle)
				{
					char logBuffer[16384] = {};

					while (true)
					{
						char buffer[4096] = {};
						DWORD bytesRead;

						bool succeeded = ReadFile(ExternalPipeReaderHandle, buffer, ARRAYSIZE(buffer) - 1, &bytesRead, nullptr) != 0;

						// Bail if there's nothing left or the process exited
						if (!succeeded || bytesRead <= 0)
							break;

						strcat_s(logBuffer, buffer);

						// Flush on every newline and skip empty/whitespace strings
						while (char* end = strchr(logBuffer, '\n'))
						{
							*end = '\0';
							auto len = static_cast<size_t>(end - logBuffer);

							while (strchr(logBuffer, '\r'))
								*strchr(logBuffer, '\r') = ' ';

							if (len > 0 && (len > 1 || logBuffer[0] != ' '))
								GlobalConsoleWindowPtr->InputLog("%s", logBuffer);

							strcpy_s(logBuffer, end + 1);
						}
					}

					CloseHandle(ExternalPipeReaderHandle);
					CloseHandle(ExternalPipeWriterHandle);
				}, _ExternalPipeReaderHandle, _ExternalPipeWriterHandle);

			pipeReader.detach();
			return true;
		}

		LRESULT CALLBACK ConsoleWindow::WndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
		{
			static size_t LineCount = 0;

			if (WM_NCCREATE == Message)
			{
				auto info = reinterpret_cast<const CREATESTRUCT*>(lParam);
				SetWindowLongPtr(Hwnd, GWLP_USERDATA, (LONG_PTR)(info->lpCreateParams));
			}
			else
			{
				auto moduleConsole = (ConsoleWindow*)GetWindowLongPtr(Hwnd, GWLP_USERDATA);

				switch (Message)
				{
				case WM_CREATE:
				{
					auto info = reinterpret_cast<const CREATESTRUCT*>(lParam);

					// Создание rich edit control (https://docs.microsoft.com/en-us/windows/desktop/Controls/rich-edit-controls)
					uint32_t style = WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE | ES_LEFT | ES_NOHIDESEL |
						ES_AUTOVSCROLL | ES_READONLY;

					HWND richEditHwnd = CreateWindowExW(0, MSFTEDIT_CLASS, L"", style, 0, 0, info->cx, info->cy, Hwnd,
						NULL, info->hInstance, NULL);
					if (!(richEditHwnd))
						return -1;

					moduleConsole->SetRichEditHandle(richEditHwnd);
					moduleConsole->SetAutoScroll(true);

					// Установить лучший шрифт и преобразовать баллы в пункты (1 балл = 20 пункты)
					CHARFORMAT2A format = { 0 };
					format.cbSize = sizeof(format);
					format.dwMask = CFM_FACE | CFM_SIZE | CFM_WEIGHT;
					format.yHeight = _READ_OPTION_INT("Log", "nFontSize", 10) * 20;
					format.wWeight = (WORD)_READ_OPTION_UINT("Log", "uFontWeight", FW_NORMAL);
					strncpy_s(format.szFaceName, _READ_OPTION_STR("Log", "sFont", "Consolas").c_str(), _TRUNCATE);

					SendMessageA(richEditHwnd, EM_SETCHARFORMAT, SCF_ALL, reinterpret_cast<LPARAM>(&format));

					//Подписаться на EN_MSGFILTER и EN_SELCHANGE
					SendMessageA(richEditHwnd, EM_SETEVENTMASK, 0, ENM_MOUSEEVENTS | ENM_SELCHANGE);

					if (_READ_OPTION_BOOL("Log", "bShowWidow", true))
					{
						// Установить положение окна по умолчанию
						int winX = _READ_OPTION_INT("Log", "nX", info->x);
						int winY = _READ_OPTION_INT("Log", "nY", info->y);
						int winW = _READ_OPTION_INT("Log", "nWidth", info->cx);
						int winH = _READ_OPTION_INT("Log", "nHeight", info->cy);

						MoveWindow(Hwnd, winX, winY, winW, winH, FALSE);

						if (winW != 0 && winH != 0)
							ShowWindow(Hwnd, SW_SHOW);
					}
				}
				return 0;

				case WM_DESTROY:
				{
					if (DestroyWindow(moduleConsole->GetRichEditHandle()))
						moduleConsole->SetRichEditHandle(NULL);
				}
				return 0;

				case WM_SIZE:
				{
					int w = LOWORD(lParam);
					int h = HIWORD(lParam);
					MoveWindow(moduleConsole->GetRichEditHandle(), 0, 0, w, h, TRUE);
				}
				break;

				case WM_ACTIVATE:
				{
					if (wParam != WA_INACTIVE)
						SetFocus(moduleConsole->GetRichEditHandle());
				}
				return 0;

				case WM_CLOSE:
				{
					ShowWindow(Hwnd, SW_HIDE);
				}
				return 0;

				case WM_NOTIFY:
				{
					static uint64_t lastClickTime;
					auto notification = reinterpret_cast<const LPNMHDR>(lParam);

					if (notification->code == EN_MSGFILTER)
					{
						auto msgFilter = reinterpret_cast<const MSGFILTER*>(notification);

						if (msgFilter->msg == WM_LBUTTONDBLCLK)
							lastClickTime = GetTickCount64();
					}
					else if (notification->code == EN_SELCHANGE)
					{
						auto selChange = reinterpret_cast<const SELCHANGE*>(notification);
						// Двойной щелчок мыши с правильным выбором -> попробовать проанализировать идентификатор формы
						if ((GetTickCount64() - lastClickTime > 1000) || selChange->seltyp == SEL_EMPTY)
							break;

						char lineData[2048] = { 0 };
						*reinterpret_cast<uint16_t*>(&lineData[0]) = ARRAYSIZE(lineData);

						// Получить номер строки и текст из выбранного диапазона
						LRESULT lineIndex = SendMessageA(moduleConsole->GetRichEditHandle(), EM_LINEFROMCHAR,
							selChange->chrg.cpMin, 0);
						LRESULT charCount = SendMessageA(moduleConsole->GetRichEditHandle(), EM_GETLINE, lineIndex,
							reinterpret_cast<LPARAM>(&lineData));

						if (charCount > 0)
						{
							lineData[charCount - 1] = '\0';

							// Захватить шестнадцатеричный идентификатор формы в формате "(XXXXXXXX)"
							for (char* p = lineData; p[0] != '\0'; p++)
							{
								if (p[0] == '(' && strlen(p) >= 10 && p[9] == ')')
								{
									uint32_t id = strtoul(&p[1], nullptr, 16);

									if (GlobalEnginePtr->GetEditorVersion() <= EDITOR_SKYRIM_SE_LAST)
										PostMessageA(Patches::SkyrimSpectialEdition::GlobalMainWindowPtr->Handle, 
											WM_COMMAND, EditorAPI::EditorUI::UI_EDITOR_OPENFORMBYID, id);
								}
							}
						}

						lastClickTime = GetTickCount64() + 1000;
					}
				}
				break;

				case WM_TIMER:
				{
					if (wParam != UI_LOG_CMD_ADDTEXT)
						break;

					// Если очень много тысяч в лог окне, то разумно его почистить,
					// Это освободит немного памяти.
					if (LineCount >= LINECOUNT_MAX)
					{
						moduleConsole->Clear();
						auto rich = moduleConsole->GetRichEditHandle();
						LineCount = (size_t)SendMessageA(rich, EM_GETLINECOUNT, 0, 0);
					}

					if (moduleConsole->GetPendingMessages().size() <= 0)
						break;

					return WndProc(Hwnd, UI_LOG_CMD_ADDTEXT, 0, 0);
				}
				return 0;

				case UI_LOG_CMD_ADDTEXT:
				{
					auto rich = moduleConsole->GetRichEditHandle();
					SendMessageA(rich, WM_SETREDRAW, FALSE, 0);

					// Сохранить старую позицию, если нет автоскроллинга
					POINT scrollRange = { 0 };

					if (!moduleConsole->HasAutoScroll())
						SendMessageA(rich, EM_GETSCROLLPOS, 0, reinterpret_cast<LPARAM>(&scrollRange));

					// Получить копию всех элементов и очистить целевой список
					auto messages(std::move(moduleConsole->GetPendingMessages()));

					for (const char* message : messages)
					{
						// Переместить курсор в конец, затем написать
						CHARRANGE range
						{
							.cpMin = LONG_MAX,
							.cpMax = LONG_MAX,
						};

						SendMessageA(rich, EM_EXSETSEL, 0, reinterpret_cast<LPARAM>(&range));
						SendMessageA(rich, EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(message));

						voltek::scalable_free(message);
						LineCount++;
					}

					if (!moduleConsole->HasAutoScroll())
						SendMessageA(rich, EM_SETSCROLLPOS, 0, reinterpret_cast<LPARAM>(&scrollRange));

					SendMessageA(rich, WM_SETREDRAW, TRUE, 0);
					RedrawWindow(rich, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
				}
				return 0;

				case UI_LOG_CMD_CLEARTEXT:
				{
					moduleConsole->Clear();
				}
				return 0;

				case UI_LOG_CMD_AUTOSCROLL:
				{
					moduleConsole->SetAutoScroll(static_cast<bool>(wParam));
				}
				return 0;
				}
			}

			return DefWindowProc(Hwnd, Message, wParam, lParam);
		}

		bool ConsoleWindow::SaveRichTextToFile(const char* _filename) const
		{
			// Изучено отсюда
			// https://subscribe.ru/archive/comp.soft.prog.qandacpp/200507/10000511.html

			// открываю\создаю файл, полностью заменяю его содержимое
			// если файл существует
			FILE* fileStream = _fsopen(_filename, "wt", _SH_DENYRW);
			if (!fileStream)
			{
				_ERROR("I can't open the file for writing: \"%s\"", _filename);
				return false;
			}

			CreationKitPlatformExtended::Utils::ScopeFileStream file(fileStream);

			// функция обратного вызова для вывода в файл
			auto MyOutFunction = [](
				DWORD_PTR dwCookie, // то самое пользовательское значение которое
									// мы указали в EDITSTREAM::dwCookie
				LPBYTE pbBuff,		// буфер с данными которые передает RichEdit
				LONG cb,			// размер буфера в байтах
				LONG* pcb			// указатель на переменную в которую следует
									// записать сколько функция MyOutFunction
									// успешно обработала байтов из буфера pbBuff) -> DWORD 
			) -> DWORD {
					// в качестве dwCookie получаем указатель который мы
					// установили в EDITSTREAM
					FILE* stream = reinterpret_cast<FILE*>(dwCookie);
					// записываем полученный буфер в файл, размер переданного нам
					// буфера в переменной cb
					fwrite(pbBuff, 1, cb, stream);
					// говорим RichEditу сколько мы обработали байтов
					*pcb = cb;
					// возвращаем ноль (у нас всё ОК)
					return 0;
			};
			
			EDITSTREAM es = { 0 };
			// указываем функцию обратного вызова
			es.pfnCallback = static_cast<EDITSTREAMCALLBACK>(MyOutFunction);
			// сбрасываем ошибки
			es.dwError = 0;
			// в качестве Cookie передаем указатель на наш объект file
			es.dwCookie = (DWORD_PTR)fileStream;
			// посылаем сообщение окну RichEdit, WPARAM==флаги,
			// LPARAM - указатель на EDITSTREAM
			SendMessage(_richEditHwnd, EM_STREAMOUT,
				SF_TEXT /*получать обычный текст*/,
				(LPARAM)&es);

			// true - если не было ошибок, иначе что-то где-то вдруг
			// порою не в порядке.
			return !es.dwError;
		}

		void ConsoleWindow::CloseOutputFile()
		{
			if (_outputFileHandle)
			{
				fclose(_outputFileHandle);
				_outputFileHandle = nullptr;
			}
		}

		void ConsoleWindow::UpdateWindow() const
		{
			if (hWindow)
				::UpdateWindow(hWindow);
		}

		void ConsoleWindow::BringToFront() const
		{
			if (hWindow)
			{
				ShowWindow(hWindow, SW_SHOW);
				SetForegroundWindow(hWindow);
			}
		}

		void ConsoleWindow::Hide() const
		{
			if (hWindow)
				ShowWindow(hWindow, SW_HIDE);
		}

		bool ConsoleWindow::Create()
		{
			if (hWindow)
				return false;

			// Загрузка нового RichEdit
			if (!LoadLibraryA("MSFTEDIT.dll"))
				return false;

			LoadWarningBlacklist();

			auto fName = _READ_OPTION_USTR("Log", "sOutputFile", FILE_NONE);
			if (fName != FILE_NONE)
			{
				_outputFileHandle = _wfsopen(fName.c_str(), L"wt", _SH_DENYRW);
				if (!_outputFileHandle)
					_ERROR(L"Unable to open the log file '%s' for writing. To disable, "
						"set the 'OutputFile' INI option to 'none'.", fName.c_str());
			}

			// В отдельном потоке создаём окно и там же для него отдельную очередь сообщений
			// Поскольку, оно не имеет родительского окна (его ещё нет), окно живёт своей жизнью
			// и не сворачивается вместе с окном Creation Kit, к примеру.

			std::thread asyncLogThread([](HWND* window, ConsoleWindow* module)
				{
					// Окно вывода
					auto instance = static_cast<HINSTANCE>(GetModuleHandle(NULL));
					
					if (_READ_OPTION_BOOL("CreationKit", "bUIDarkTheme", false))
						Patches::UIThemePatch::InitializeThread();

					WNDCLASSEXA wc
					{
						.cbSize = sizeof(WNDCLASSEX),
						.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
						.lpfnWndProc = WndProc,
						.hInstance = instance,
						.hIcon = LoadIconA(instance, MAKEINTRESOURCE(0x13E)),				// 0x13E всегда иконка Creation Kit
						.hCursor = LoadCursor(NULL, IDC_ARROW),
						.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)),
						.lpszClassName = "RTEDITLOG",
						.hIconSm = wc.hIcon,
					};

					if (!RegisterClassExA(&wc))
						return false;

					*window = CreateWindowExA(0, "RTEDITLOG", "Log Window", WS_OVERLAPPEDWINDOW,
						64, 64, 1024, 480, NULL, NULL, instance, module);

					if (!(*window))
						return false;

					// Опрашивайть каждые 100 мс на наличие новых строк
					SetTimer(*window, UI_LOG_CMD_ADDTEXT, 100, NULL);
					::UpdateWindow(*window);

					MSG msg;
					while (GetMessageA(&msg, NULL, 0, 0) > 0)
					{
						TranslateMessage(&msg);
						DispatchMessageA(&msg);
					}

					return true;
				}, &hWindow, this);

			asyncLogThread.detach();

			return true;
		}

		void ConsoleWindow::Destroy()
		{
			if (hWindow)
				DestroyWindow(hWindow);
		}

		void ConsoleWindow::LoadWarningBlacklist()
		{
			FILE* fileStream = _wfsopen(L"CreationKitPlatformExtendedMessagesBlacklist.txt", L"rt", _SH_DENYWR);
			if (!fileStream) return;

			CreationKitPlatformExtended::Utils::ScopeFileStream file(fileStream);
			auto szBuf = std::make_unique<char[]>(2049);
			szBuf.get()[2048] = '\0';
			
			size_t nCount = 0;
			String Message;
			while (!feof(fileStream))
			{
				fgets(szBuf.get(), 2048, fileStream);
				nCount++;

				Message = CreationKitPlatformExtended::Utils::Trim(szBuf.get());
				_messageBlacklist.emplace(CreationKitPlatformExtended::Utils::MurmurHash64A(Message.c_str(), Message.length()));
			}

			_MESSAGE("Messages Blacklist: %llu", _messageBlacklist.size());
			if (nCount > _messageBlacklist.size())
				_MESSAGE("Number of messages whose hash has already been added: %llu", (nCount - _messageBlacklist.size()));
		}

		void ConsoleWindow::InputLog(const char* Format, ...)
		{
			va_list va;
			va_start(va, Format);
			InputLogVa(Format, va);
			va_end(va);
		}

		void ConsoleWindow::InputLogVa(const char* Format, va_list Va)
		{
			char buffer[2048];
			int len = _vsnprintf_s(buffer, _TRUNCATE, Format, Va);

			if (len <= 0)
				return;

			auto line = Utils::Trim(buffer);
			std::replace_if(line.begin(), line.end(), [](auto const& x) { return x == '\n' || x == '\r'; }, ' ');

			if (!line.length())
				return;

			auto HashMsg = CreationKitPlatformExtended::Utils::MurmurHash64A(line.c_str(), line.length());	
			if ((HashLastMsg == HashMsg) || (_messageBlacklist.count(HashMsg) > 0))
				return;

			HashLastMsg = HashMsg;
			line += "\n";

			if (_outputFileHandle)
			{
				fputs(line.c_str(), _outputFileHandle);
				fflush(_outputFileHandle);
			}

			if (_pendingMessages.size() < LINECOUNT_MAX)
				_pendingMessages.push_back(Utils::StrDub(line.c_str()));
		}
	}

	void _CONSOLE(const char* fmt, ...)
	{
		va_list va;
		va_start(va, fmt);
		_CONSOLEVA(fmt, va);
		va_end(va);
	}

	void _CONSOLEVA(const char* fmt, va_list va)
	{
		if (Core::GlobalConsoleWindowPtr)
			Core::GlobalConsoleWindowPtr->InputLogVa(fmt, va);
	}
}