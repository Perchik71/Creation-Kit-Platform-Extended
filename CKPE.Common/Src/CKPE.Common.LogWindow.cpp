// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <richedit.h>
#include <CKPE.Application.h>
#include <CKPE.Exception.h>
#include <CKPE.HashUtils.h>
#include <CKPE.Stream.h>
#include <CKPE.PathUtils.h>
#include <CKPE.MessageBox.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.MemoryManager.h>
#include <concurrent_vector.h>
#include <unordered_set>
#include <thread>
#include <stdexcept>
#include <memory>
#include <string>

namespace CKPE
{
	namespace Common
	{
		constexpr static auto FILE_NONE = L"none";
		constexpr static auto FILE_BLACKLIST = L"CreationKitPlatformExtendedMessagesBlacklist.txt";
		constexpr static auto UI_LOG_CMD_ADDTEXT = 0x23000;
		constexpr static auto UI_LOG_CMD_CLEARTEXT = 0x23001;
		constexpr static auto UI_LOG_CMD_AUTOSCROLL = 0x23002;

		constexpr static size_t LINECOUNT_MAX = 50000;

		static LogWindow* slogwindow = nullptr;
		concurrency::concurrent_vector<const char*> _pendingMessages;
		std::unordered_set<uint64_t> _messageBlacklist;

		static LRESULT CALLBACK LogWindowProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
		{
			static size_t LineCount = 0;

			if (WM_NCCREATE == Message)
			{
				auto info = reinterpret_cast<const CREATESTRUCT*>(lParam);
				SetWindowLongPtr(Hwnd, GWLP_USERDATA, (LONG_PTR)(info->lpCreateParams));
			}
			else
			{
				auto log = (LogWindow*)GetWindowLongPtr(Hwnd, GWLP_USERDATA);

				switch (Message)
				{
				case WM_CREATE:
				{
					try
					{
						if (!log->Initialize(Hwnd, (void*)lParam))
							throw SystemError(GetLastError(), "LogWindow_Initialize");
					}
					catch (const std::exception& e)
					{
						ErrorHandler::Trigger(e.what());
					}					
				}
				return 0;

				case WM_DESTROY:
					log->ReleaseHandler();
					return 0;

				case WM_SIZE:
					log->ResizeHandler(LOWORD(lParam), HIWORD(lParam));
					break;

				case WM_ACTIVATE:
					log->ActiveHandler(wParam != WA_INACTIVE);
					return 0;

				case WM_CLOSE:
					log->CloseHandler();
					return 0;

				case WM_TIMER:
				{
					if (wParam != UI_LOG_CMD_ADDTEXT)
						break;

					// Если очень много тысяч в лог окне, то разумно его почистить,
					// Это освободит немного памяти.
					if (LineCount >= LINECOUNT_MAX)
					{
						log->Clear();
						auto rich = log->GetRichEditHandle();
						LineCount = (size_t)SendMessageA((HWND)rich, EM_GETLINECOUNT, 0, 0);
					}

					if (_pendingMessages.size() <= 0)
						break;

					return LogWindowProc(Hwnd, UI_LOG_CMD_ADDTEXT, 0, 0);
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

						if (!log->OnOpenFormById)
							break;

						char lineData[2048] = { 0 };
						*reinterpret_cast<uint16_t*>(&lineData[0]) = ARRAYSIZE(lineData);

						// Получить номер строки и текст из выбранного диапазона
						LRESULT lineIndex = SendMessageA((HWND)log->GetRichEditHandle(), EM_LINEFROMCHAR,
							selChange->chrg.cpMin, 0);
						LRESULT charCount = SendMessageA((HWND)log->GetRichEditHandle(), EM_GETLINE, lineIndex,
							reinterpret_cast<LPARAM>(&lineData));

						if (charCount > 0)
						{
							lineData[charCount - 1] = '\0';

							// Захватить шестнадцатеричный идентификатор формы в формате "(XXXXXXXX)"
							for (char* p = lineData; p[0] != '\0'; p++)
								if (p[0] == '(' && strlen(p) >= 10 && p[9] == ')')
									log->OnOpenFormById(strtoul(&p[1], nullptr, 16));
						}

						lastClickTime = GetTickCount64() + 1000;
					}
				}
				break;

				case UI_LOG_CMD_CLEARTEXT:
					log->Clear();
					return 0;

				case UI_LOG_CMD_AUTOSCROLL:
					log->SetAutoScroll(static_cast<bool>(wParam));
					return 0;

				case UI_LOG_CMD_ADDTEXT:
				{
					auto rich = (HWND)log->GetRichEditHandle();
					SendMessageA(rich, WM_SETREDRAW, FALSE, 0);

					// Сохранить старую позицию, если нет автоскроллинга
					POINT scrollRange = { 0 };

					if (!log->HasAutoScroll())
						SendMessageA(rich, EM_GETSCROLLPOS, 0, reinterpret_cast<LPARAM>(&scrollRange));

					// Получить копию всех элементов и очистить целевой список
					auto messages(std::move(_pendingMessages));

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

						CKPE::free((void*)message);
						LineCount++;
					}

					if (!log->HasAutoScroll())
						SendMessageA(rich, EM_SETSCROLLPOS, 0, reinterpret_cast<LPARAM>(&scrollRange));

					SendMessageA(rich, WM_SETREDRAW, TRUE, 0);
					RedrawWindow(rich, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
				}
				return 0;
				}
			}

			return DefWindowProc(Hwnd, Message, wParam, lParam);
		}
		
		bool LogWindow::Create()
		{
			if (slogwindow || _handle)
				return false;

			slogwindow = this;

			// Загрузка нового RichEdit
			if (!LoadLibraryA("MSFTEDIT.dll"))
				return false;

			LoadWarningBlacklist();

			auto fName = _READ_OPTION_USTR("Log", "sOutputFile", FILE_NONE);
			if ((fName != FILE_NONE) && !fName.empty())
			{
				_output_file = _wfsopen(fName.c_str(), L"wt", _SH_DENYRW);
				if (!_output_file)
					_ERROR(L"Unable to open the log file '%s' for writing. To disable, "
						"set the 'OutputFile' INI option to 'none'.", fName.c_str());
			}

			// В отдельном потоке создаём окно и там же для него отдельную очередь сообщений
			// Поскольку, оно не имеет родительского окна (его ещё нет), окно живёт своей жизнью
			// и не сворачивается вместе с окном Creation Kit, к примеру.

			std::thread asyncLogThread([](HWND* window, LogWindow* module)
				{
					// Окно вывода
					auto instance = static_cast<HINSTANCE>(GetModuleHandle(NULL));
					/*auto darkmode = _READ_OPTION_BOOL("CreationKit", "bUIDarkTheme", false);
					auto classicmode = _READ_OPTION_BOOL("CreationKit", "bUIClassicTheme", false);*/

					/*if (darkmode && classicmode)
						_WARNING("You cannot use classic mode and dark mode together.");
					else
					{
						if (darkmode)
							Patches::UIThemePatch::InitializeCurrentThread();
						else if (classicmode)
							Patches::UIThemeClassicPatch::InitializeCurrentThread();
					}*/

					WNDCLASSEXA wc
					{
						.cbSize = sizeof(WNDCLASSEXA),
						.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
						.lpfnWndProc = LogWindowProc,
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
				}, (HWND*)&_handle, this);

			asyncLogThread.detach();

			return true;
		}

		void LogWindow::Destroy()
		{
			if (_handle)
			{
				DestroyWindow((HWND)_handle);
				_handle = nullptr;
			}
		}

		bool LogWindow::CreateStdoutListener() noexcept(true)
		{
			SECURITY_ATTRIBUTES saAttr
			{
				.nLength = sizeof(SECURITY_ATTRIBUTES),
				.lpSecurityDescriptor = nullptr,
				.bInheritHandle = TRUE,
			};

			if (!CreatePipe((PHANDLE)&_external_pipereader_handler, (PHANDLE)&_external_pipewriter_handler, &saAttr, 0))
				return false;

			// Ensure the read handle to the pipe for STDOUT is not inherited
			if (!SetHandleInformation((HANDLE)_external_pipereader_handler, HANDLE_FLAG_INHERIT, 0))
				return false;

			std::thread pipeReader([](HANDLE ExternalPipeReaderHandle, HANDLE ExternalPipeWriterHandle)
				{
					auto logBuffer = std::make_unique<char[]>(16384);
					auto buffer = std::make_unique<char[]>(4096);

					while (true)
					{
						DWORD bytesRead;
						bool succeeded = ReadFile(ExternalPipeReaderHandle, buffer.get(), 4095, &bytesRead, nullptr) != 0;

						// Bail if there's nothing left or the process exited
						if (!succeeded || bytesRead <= 0)
							break;

						buffer.get()[bytesRead] = '\0';
						strcat_s(logBuffer.get(), 16384, buffer.get());

						// Flush on every newline and skip empty/whitespace strings
						while (char* end = strchr(logBuffer.get(), '\n'))
						{
							*end = '\0';
							auto len = static_cast<size_t>(end - logBuffer.get());

							while (strchr(logBuffer.get(), '\r'))
								*strchr(logBuffer.get(), '\r') = ' ';

							if (len > 0 && (len > 1 || logBuffer[0] != ' '))
								slogwindow->InputLog("%s", logBuffer.get());

							strcpy_s(logBuffer.get(), 16384, end + 1);
						}
					}

					CloseHandle(ExternalPipeReaderHandle);
					CloseHandle(ExternalPipeWriterHandle);
				}, (HANDLE)_external_pipereader_handler, (HANDLE)_external_pipewriter_handler);

			pipeReader.detach();
			return true;
		}

		void LogWindow::CloseOutputFile() noexcept(true)
		{
			if (_output_file)
			{
				fclose(_output_file);
				_output_file = nullptr;
			}
		}

		void LogWindow::LoadWarningBlacklist() noexcept(true)
		{
			auto app = Interface::GetSingleton()->GetApplication();
			auto spath = std::wstring(app->GetPath());
			
			if (!PathUtils::FileExists(spath + FILE_BLACKLIST))
				return;
			
			try
			{
				TextFileStream stream(spath + FILE_BLACKLIST, FileStream::fmOpenRead);

				std::string sbuffer;
				sbuffer.resize(2048);
				if (sbuffer.empty())
					throw RuntimeError("LogWindow: Out of memory");

				std::size_t nCount = 0;
				std::string Message;
				while (!stream.Eof())
				{
					if (!stream.ReadLine(sbuffer, 2048))
						break;

					nCount++;
					Message = StringUtils::Trim(sbuffer);
					_messageBlacklist.emplace(HashUtils::MurmurHash64A(Message.c_str(), Message.length()));
				}

				_MESSAGE("Messages Blacklist: %llu", _messageBlacklist.size());
				if (nCount > _messageBlacklist.size())
					_MESSAGE("Number of messages whose hash has already been added: %llu", (nCount - _messageBlacklist.size()));
			}
			catch (const std::exception& e)
			{
				_ERROR(e.what());
			}
		}

		LogWindow::LogWindow()
		{
			Create();
		}

		LogWindow::~LogWindow()
		{
			Destroy();
		}

		void LogWindow::Shutdown() noexcept(true)
		{
			if (slogwindow)
			{
				delete slogwindow;
				slogwindow = nullptr;
			}
		}

		LogWindow* LogWindow::GetSingleton() noexcept(true)
		{
			return slogwindow;
		}

		bool LogWindow::Initialize(void* handle, const void* create_struct)
		{
			auto cs = reinterpret_cast<const CREATESTRUCTA*>(create_struct);
			_handle = handle;

			// Создание rich edit control (https://docs.microsoft.com/en-us/windows/desktop/Controls/rich-edit-controls)
			uint32_t style = WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE | ES_LEFT | ES_NOHIDESEL |
				ES_AUTOVSCROLL | ES_READONLY;

			_handle_richedit = (void*)CreateWindowExW(0, MSFTEDIT_CLASS, L"", style, 0, 0, (int)cs->cx, (int)cs->cy, (HWND)_handle,
				NULL, cs->hInstance, NULL);
			if (!_handle_richedit)
				return false;

			// Установить лучший шрифт и преобразовать баллы в пункты (1 балл = 20 пункты)
			CHARFORMAT2A format = { 0 };
			format.cbSize = sizeof(format);
			format.dwMask = CFM_FACE | CFM_SIZE | CFM_WEIGHT;
			format.yHeight = _READ_OPTION_INT("Log", "nFontSize", 10) * 20;
			format.wWeight = (WORD)_READ_OPTION_UINT("Log", "uFontWeight", FW_NORMAL);
			strncpy_s(format.szFaceName, _READ_OPTION_STR("Log", "sFont", "Consolas").c_str(), _TRUNCATE);

			SendMessageA((HWND)_handle_richedit, EM_SETCHARFORMAT, SCF_ALL, reinterpret_cast<LPARAM>(&format));
			//Подписаться на EN_MSGFILTER и EN_SELCHANGE
			SendMessageA((HWND)_handle_richedit, EM_SETEVENTMASK, 0, ENM_MOUSEEVENTS | ENM_SELCHANGE);

			if (_READ_OPTION_BOOL("Log", "bShowWindow", true))
			{
				// Установить положение окна по умолчанию
				int winX = _READ_OPTION_INT("Log", "nX", cs->x);
				int winY = _READ_OPTION_INT("Log", "nY", cs->y);
				int winW = _READ_OPTION_INT("Log", "nWidth", cs->cx);
				int winH = _READ_OPTION_INT("Log", "nHeight", cs->cy);

				MoveWindow((HWND)_handle, winX, winY, winW, winH, FALSE);

				if (winW != 0 && winH != 0)
					Show(true);
			}

			return true;
		}

		void LogWindow::Show(bool sh) const noexcept(true)
		{
			if ((HWND)_handle)
			{
				ShowWindow((HWND)_handle, sh ? SW_SHOW : SW_HIDE);
				if (sh) SetForegroundWindow((HWND)_handle);
			}
		}

		void LogWindow::Update() const noexcept(true)
		{
			if ((HWND)_handle)
				::UpdateWindow((HWND)_handle);
		}

		void LogWindow::Clear() const noexcept(true)
		{
			if ((HWND)_handle_richedit)
			{
				CHARRANGE range
				{
					.cpMin = (LONG)SendMessageA((HWND)_handle_richedit, EM_LINEINDEX, 7, 0),
					.cpMax = LONG_MAX,
				};

				SendMessageA((HWND)_handle_richedit, EM_EXSETSEL, 0, reinterpret_cast<LPARAM>(&range));
				SendMessageA((HWND)_handle_richedit, EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(""));
			}
		}

		void LogWindow::ReleaseHandler() noexcept(true)
		{
			if (DestroyWindow((HWND)_handle_richedit))
				_handle_richedit = nullptr;
		}

		void LogWindow::ResizeHandler(std::uint32_t x, std::uint32_t y) const noexcept(true)
		{
			MoveWindow((HWND)_handle_richedit, 0, 0, x, y, TRUE);
		}

		void LogWindow::ActiveHandler(bool active) const noexcept(true)
		{
			if (active)
				SetFocus((HWND)_handle_richedit);
		}

		void LogWindow::CloseHandler() const noexcept(true)
		{
			Show(false);
		}

		bool LogWindow::SaveRichTextToFile(const std::string& fname) const noexcept(true)
		{
			return SaveRichTextToFile(StringUtils::Utf8ToUtf16(fname));
		}

		bool LogWindow::SaveRichTextToFile(const std::wstring& fname) const noexcept(true)
		{
			// Изучено отсюда
			// https://subscribe.ru/archive/comp.soft.prog.qandacpp/200507/10000511.html

			try
			{
				// открываю\создаю файл, полностью заменяю его содержимое
				// если файл существует
				FileStream stream(fname, FileStream::fmCreate);

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
						auto stream = reinterpret_cast<FileStream*>(dwCookie);
						// записываем полученный буфер в файл, размер переданного нам
						// буфера в переменной cb
						stream->Write(pbBuff, cb);
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
				es.dwCookie = (DWORD_PTR)&stream;
				// посылаем сообщение окну RichEdit, WPARAM==флаги,
				// LPARAM - указатель на EDITSTREAM
				SendMessageA((HWND)_handle_richedit, EM_STREAMOUT,
					SF_TEXT /*получать обычный текст*/,
					(LPARAM)&es);

				// true - если не было ошибок, иначе что-то где-то вдруг
				// порою не в порядке.
				return !es.dwError;
			}
			catch (const std::exception& e)
			{
				_ERROR(e.what());
				return false;
			}
		}

		void LogWindow::InputLog(const std::string_view& formatted_message, ...)
		{
			va_list va;
			va_start(va, &formatted_message);
			InputLogVa(formatted_message, va);
			va_end(va);
		}

		void LogWindow::InputLogVa(const std::string_view& formatted_message, va_list va)
		{
			int len = _vscprintf(formatted_message.data(), va);
			if (len <= 0)
				return;

			std::string sbuffer;
			sbuffer.resize(len);
			if (!sbuffer.empty())
			{
				_vsnprintf_s(sbuffer.data(), (std::size_t)len + 1, _TRUNCATE, formatted_message.data(), va);

				sbuffer = StringUtils::Trim(sbuffer);
				std::replace_if(sbuffer.begin(), sbuffer.end(), [](auto const& x) { return x == '\n' || x == '\r'; }, ' ');
				if (!sbuffer.length())
					return;

				auto HashMsg = HashUtils::MurmurHash64A(sbuffer.c_str(), sbuffer.length());
				if ((_last_hash == HashMsg) || (_messageBlacklist.count(HashMsg) > 0))
					return;

				_last_hash = HashMsg;
				// fix length string sbuffer
				sbuffer = std::string(sbuffer.c_str()).append("\n");

				if (_output_file)
				{
					fputs(sbuffer.c_str(), _output_file);
					fflush(_output_file);
				}

				if (_pendingMessages.size() < LINECOUNT_MAX)
					_pendingMessages.push_back(CKPE::strdup(sbuffer.c_str()));
			}
		}

		void LogWindow::InputLog(const std::wstring_view& formatted_message, ...)
		{
			va_list va;
			va_start(va, &formatted_message);
			InputLogVa(formatted_message, va);
			va_end(va);
		}

		void LogWindow::InputLogVa(const std::wstring_view& formatted_message, va_list va)
		{
			int len = _vscwprintf(formatted_message.data(), va);
			if (len <= 0)
				return;

			std::wstring sbuffer;
			sbuffer.resize(len);
			if (!sbuffer.empty())
			{
				_vsnwprintf_s(sbuffer.data(), (std::size_t)len, _TRUNCATE, formatted_message.data(), va);
				InputLog(StringUtils::Utf16ToWinCP(sbuffer));
			}
		}
	}

	CKPE_COMMON_API void _CONSOLE(const std::string_view& formatted_message, ...) noexcept(true)
	{
		va_list va;
		va_start(va, &formatted_message);
		_CONSOLEVA(formatted_message, va);
		va_end(va);
	}

	CKPE_COMMON_API void _CONSOLEVA(const std::string_view& formatted_message, va_list va) noexcept(true)
	{
		if (Common::slogwindow)
			Common::slogwindow->InputLogVa(formatted_message, va);
	}

	CKPE_COMMON_API void _CONSOLE(const std::wstring_view& formatted_message, ...) noexcept(true)
	{
		va_list va;
		va_start(va, &formatted_message);
		_CONSOLEVA(formatted_message, va);
		va_end(va);
	}

	CKPE_COMMON_API void _CONSOLEVA(const std::wstring_view& formatted_message, va_list va) noexcept(true)
	{
		if (Common::slogwindow)
			Common::slogwindow->InputLogVa(formatted_message, va);
	}
}