// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <cstdarg>
#include <CKPE.PathUtils.h>
#include <CKPE.StringUtils.h>
#include <CKPE.Application.h>
#include <CKPE.FileUtils.h>
#include <CKPE.Graphics.h>
#include <CKPE.Common.Include.h>
#include <CKPE.Common.DialogManager.h>
#include <CKPE.Common.PatchManager.h>
#include <CKPE.Common.Relocator.h>
#if 0
#include <CKPE.Common.GenerateTableID.h>
#endif
#include <CKPE.Common.Registry.h>
#include <CKPE.Common.RTTI.h>
#include <CKPE.Exception.h>
#include <algorithm>
#include <time.h>

namespace CKPE
{
	namespace Common
	{
		static Interface _sinterface;

		constexpr static auto _dllName = L"CKPE.Common.dll";
		constexpr static auto _ssettings_fname = L"CreationKitPlatformExtended.toml";
		constexpr static auto _stheme_settings_fname = L"CreationKitPlatformExtendedCustomTheme.toml";

		Interface::~Interface() noexcept(true)
		{
			LogWindow::Shutdown();

			if (_settings)
			{
				delete _settings;
				_settings = nullptr;
			}

			if (_theme_settings)
			{
				delete _theme_settings;
				_theme_settings = nullptr;
			}

			if (_cmdline)
			{
				delete _cmdline;
				_cmdline = nullptr;
			}
		}

		void Interface::Initialize(const CKPEGameLibraryInterface* a_interface, std::uint64_t a_version,
			const std::wstring& a_dialogs_fn, const std::wstring& a_databases_fn, const std::wstring& a_database_fn,
			const std::wstring& a_resources_fn, bool support_more_theme) noexcept(true)
		{
			if (_cmdline) return;

			std::srand(time(nullptr));

			{
				// IMPORTANT OBJECTS
				_instDll = (std::uintptr_t)GetModuleHandleW(_dllName);
				_interface = a_interface;
				std::wstring spath = _interface->application->GetPath();
				_cmdline = new CommandLineParser;
				_settings = new TOMLSettingCollection(spath + _ssettings_fname);
				if (PathUtils::FileExists(spath + _stheme_settings_fname))
					_theme_settings = new TOMLSettingCollection(spath + _stheme_settings_fname);
				else
					_theme_settings = nullptr;
				_version = FileUtils::GetFileVersion(spath + _dllName);
				Common::PatchManager::GetSingleton()->OpenBlackList();

				// IMPORTANT SYSTEM
				RTTI::GetSingleton()->Initialize();

				CommandLineParser cmd;
				if (cmd.HasCommandRun())
				{
					_MESSAGE("\tAccessing the console...");

					// Detect command
					auto Command = cmd.GetCommand();
					_MESSAGE(L"\tCommand: \"%s\"", Command.c_str());

					if (!_wcsicmp(Command.c_str(), L"-PECreateDatabase"))
					{
						auto reldb = Relocator::GetSingleton();

						// Создаём базу
						reldb->Clear();
						reldb->Save(a_databases_fn, a_database_fn);

						// Close Creation Kit				
						_interface->application->Terminate();
					}
				}

				// LOAD DATAS
				if (!DialogManager::Initialize(a_dialogs_fn))
					ErrorHandler::Trigger(StringUtils::Utf16ToWinCP(
						StringUtils::FormatString(L"No found dialogs pak \"%s\"."
							"\nMore detailed to log.", a_dialogs_fn.c_str())));

				if (!Relocator::GetSingleton()->Open(a_databases_fn, a_database_fn))
					ErrorHandler::Trigger(StringUtils::Utf16ToWinCP(
						StringUtils::FormatString(L"Couldn't open the database \"%s\" in \"%s\""
							"\nMore detailed to log.", a_database_fn.c_str(), a_databases_fn.c_str())));

				// CMD LINE HANDLER

				if (cmd.HasCommandRun())
				{
					auto Command = cmd.GetCommand();
					if (!_wcsicmp(Command.c_str(), L"-PEUpdateDatabase"))
					{
						if (cmd.Count() != 2)
						{
							_ERROR("Invalid number of command arguments: %u", cmd.Count());
							_MESSAGE("Example: CreationKit -PEUpdateDatabase \"test.relb\"");
						}
						else if (!PathUtils::FileExists(cmd[1].c_str()))
							_ERROR(L"The file does not exist: \"%s\"", cmd[1].c_str());
						else
						{
							auto Patch = new RelocatorDB::PatchDB();
							if (Patch && Patch->LoadDevFromFile(cmd[1]))
							{
								auto reldb = Relocator::GetSingleton();

								reldb->Delete(Patch->GetName());
								reldb->Add(Patch);
								reldb->Save(a_databases_fn, a_database_fn);
							}
						}

						// Close Creation Kit				
						_interface->application->Terminate();
					}
					else if (!_wcsicmp(Command.c_str(), L"-PERemoveFromDatabase"))
					{
						if (cmd.Count() != 2)
						{
							_ERROR("Invalid number of command arguments: %u", cmd.Count());
							_MESSAGE("Example: CreationKit -PERemoveFromDatabase \"test\"");
						}
						else
						{
							if (Relocator::GetSingleton()->Delete(StringUtils::Utf16ToWinCP(cmd[1])))
							{
								if (Relocator::GetSingleton()->Save(a_databases_fn, a_database_fn))
									_ERROR(L"The database can't save: \"%s\"", a_databases_fn.c_str());
							}
							else
								_ERROR(L"Couldn't remove the patch from the database: \"%s\"", cmd[1].c_str());
						}

						// Close Creation Kit				
						_interface->application->Terminate();
					}
					else if (!_wcsicmp(Command.c_str(), L"-PEExtractFromDatabase"))
					{
						if ((cmd.Count() < 3) || (cmd.Count() > 4))
						{
							_ERROR("Invalid number of command arguments: %u", cmd.Count());
							_MESSAGE("Example: CreationKit -PEExtractFromDatabase \"test\" \"test.relb\" <-regen-sign>");
						}
						else
						{
							auto Patch = Relocator::GetSingleton()->GetByNameConst(StringUtils::Utf16ToWinCP(cmd[1]));
							// Патча есть?
							if (Patch)
							{
								if (cmd.Count() == 4)
									Patch->SaveDevToFile(cmd[2], !_wcsicmp(cmd[3].c_str(), L"-regen-sign"));
								else
									Patch->SaveDevToFile(cmd[2]);
							}
						}

						// Close Creation Kit				
						_interface->application->Terminate();
					}
					else if (!_wcsicmp(Command.c_str(), L"-PEExtractAllFromDatabase"))
					{
						if ((cmd.Count() < 2) || (cmd.Count() > 3))
						{
							_ERROR("Invalid number of command arguments: %u", cmd.Count());
							_MESSAGE("Example: CreationKit -PEExtractAllFromDatabase \"dir\" <-regen-sign>");
						}
						else
						{
							auto spath = cmd[1];
							PathUtils::CreateFolder(PathUtils::IncludeTrailingPathDelimiter(PathUtils::Normalize(spath)));
							
							bool regen_sign = false;
							if (cmd.Count() == 3)
								regen_sign = !_wcsicmp(cmd[2].c_str(), L"-regen-sign");

							auto rldb = Relocator::GetSingleton()->GetDB();
							_MESSAGE("\tTotal patches: %u", rldb->GetCount());

							for (std::uint32_t i = 0; i < rldb->GetCount(); i++)
							{
								auto Patch = rldb->AtByIndex(i);
								// Патча есть?
								if (Patch)
								{
									auto sname = StringUtils::FormatString(L"%s%s.relb",
										spath.c_str(), StringUtils::Utf8ToUtf16(Patch->GetName()).c_str());

									_MESSAGE(L"\tExtract: \"%s\"", sname.c_str());

									Patch->SaveDevToFile(sname, regen_sign);
								}
							}
						}

						// Close Creation Kit				
						_interface->application->Terminate();
					}
					else if (!_wcsicmp(Command.c_str(), L"-PEExportRTTI"))
					{
						if (cmd.Count() != 2)
						{
							_ERROR("Invalid number of command arguments: %u", cmd.Count());
							_MESSAGE("Example: CreationKit -PEExportRTTI \"rtti.txt\"");
						}
						else
						{
							try
							{
								RTTI::GetSingleton()->Dump(cmd[1]);
							}
							catch (const std::exception&)
							{
								_ERROR("It was not possible to create a file and write data there.");
							}
						}

						// Close Creation Kit				
						_interface->application->Terminate();
					} 
#if 0
					// Create RELIB for the current process
					else if (!_wcsicmp(Command.c_str(), L"-PECreateRL"))
					{
						if (cmd.Count() != 2)
						{
							_ERROR("Invalid number of command arguments: %u", cmd.Count());
							_MESSAGE("Example: CreationKit -PECreateAL \"version-lib.relib\"");
						}
						else
						{
							try
							{
								GenerateTableID GenRL;
								GenRL.Rebase();			// analize current .exe
								GenRL.SaveToFile(PathUtils::ChangeFileExt(cmd[1], L".relib").c_str());
							}
							catch (const std::exception&)
							{
								_ERROR("Couldn't create .relib file the current process");
							}
						}

						// Close Creation Kit				
						_interface->application->Terminate();
					}
					// Merging RELIB with the current process 
					else if (!_wcsicmp(Command.c_str(), L"-PEMergeRL"))
					{
						if (cmd.Count() != 2)
						{
							_ERROR("Invalid number of command arguments: %u", cmd.Count());
							_MESSAGE("Example: CreationKit -PEMergeRL \"version-lib.relib\"");
						}
						else
						{
							try
							{
								GenerateTableID GenRL;
								GenerateTableID OpenRL;
								GenRL.Rebase();			// analize current .exe
								OpenRL.LoadFromFile(PathUtils::ChangeFileExt(cmd[1], L".relib").c_str());
								OpenRL.Merging(GenRL);
							}
							catch (const std::exception&)
							{
								_ERROR("Couldn't merging .relib file the current process");
							}
						}

						// Close Creation Kit				
						_interface->application->Terminate();
					}
#endif
				}

				// INSTALL RUN
				// 
				// Check registry	
				Registry reg;
				if (reg.OpenKey(L"Software\\CKPE\\Installer", false))
				{
					if (reg.ValueExists("NoHi") && reg.ReadBool("NoHi"))
						goto SkipsInstaller;
					else
						goto StartInstaller;
				}
				else
				{
				StartInstaller:
					
					STARTUPINFOW si = { 0 };
					PROCESS_INFORMATION pi = { 0 };
					si.cb = sizeof(STARTUPINFOW);

					if (!CreateProcessW(NULL, (spath + L"CKPE.Installer.exe").data(), NULL, NULL, FALSE,
						0, NULL, NULL, &si, &pi))
					{
						_ERROR("Can't launch CKPE Installer");
						goto SkipsInstaller;
					}

					if (WaitForSingleObject(pi.hProcess, INFINITE) != WAIT_OBJECT_0)
						_ERROR("Error wait closes CKPE Installer");
					
					CloseHandle(pi.hThread);
					CloseHandle(pi.hProcess);
				}

			SkipsInstaller:			
				// IMPORTANT HOOKS
				EditorUI::Hook::Initialize();	// Init UI patch (Dialogs)
				SafeExit::Hook::Initialize();	// Init fast quit
				Threads::Hook::Initialize();	// Init threads

				if (_READ_OPTION_BOOL("CreationKit", "bUIClassicTheme", false))
					ClassicTheme::Hook::Initialize();

				else if (_READ_OPTION_BOOL("CreationKit", "bUIDarkTheme", false))
					ModernTheme::Hook::Initialize();

				// LOG WINDOW
				/* call constructor */ new LogWindow();		
			}

			char timeBuffer[80];
			struct tm* timeInfo;
			time_t rawtime;
			time(&rawtime);
			timeInfo = localtime(&rawtime);
			strftime(timeBuffer, sizeof(timeBuffer), "%A %d %b %Y %r %Z", timeInfo);

			auto v = _interface->ckpeVersion;
			
			_CONSOLE("##########################################################");
			_CONSOLE("Hi, I'm CKPE! Now: %s", timeBuffer); 
			// TODO: version CK
			_CONSOLE("CKPE Runtime: %u.%u build %u rev:%u", GET_EXE_VERSION_EX_MAJOR(v), GET_EXE_VERSION_EX_MINOR(v), 
				GET_EXE_VERSION_EX_BUILD(v), GET_EXE_VERSION_EX_REVISION(v));
			_CONSOLE("CKPE Common Library: %u.%u build %u rev:%u", GET_EXE_VERSION_EX_MAJOR(_version), 
				GET_EXE_VERSION_EX_MINOR(_version), GET_EXE_VERSION_EX_BUILD(_version), GET_EXE_VERSION_EX_REVISION(_version));
			_CONSOLE("CKPE Game Library: %u.%u build %u rev:%u", GET_EXE_VERSION_EX_MAJOR(a_version),
				GET_EXE_VERSION_EX_MINOR(a_version), GET_EXE_VERSION_EX_BUILD(a_version), GET_EXE_VERSION_EX_REVISION(a_version));
			_CONSOLE("I have created a log file: \"%s\"", StringUtils::Utf16ToWinCP(_interface->logger->GetFileName()).c_str());

			auto log = LogWindow::GetSingleton();
			if (log && log->HasOutputFile())
			{
				auto fName = _READ_OPTION_USTR("Log", "sOutputFile", L"");
				if (!fName.empty())
					_CONSOLE("As you wanted, I will duplicate my entries in a log file: \"%s\"", 
						StringUtils::Utf16ToWinCP(fName).c_str());
			}

			_CONSOLE("##########################################################");
		}

		void Interface::CmdLineHandler()
		{
			CommandLineParser cmd;
			if (cmd.HasCommandRun() && _interface)
			{
				auto scmd = StringUtils::ToLower(cmd.GetCommand());
				if (!scmd.compare(L"-peexportrtti"))
				{
					if (cmd.Count() != 2)
					{
						_ERROR("Invalid number of command arguments: %u", cmd.Count());
						_MESSAGE("Example: CreationKit -PEExportRTTI \"rtti.txt\"");
					}
					else
						RTTI::GetSingleton()->Dump(cmd[1]);

					// Закрываем Creation Kit
					_interface->application->Terminate();
				}
			}
		}

		bool Interface::HasCustomThemeSetting() const noexcept(true)
		{
			return _theme_settings != nullptr;
		}

		Interface* Interface::GetSingleton() noexcept(true)
		{
			return &_sinterface;
		}
	}

#ifdef CKPE_NO_LOGGER_FUNCTION
	CKPE_COMMON_API void _FATALERROR(const std::string_view& formatted_message, ...)
	{
		va_list ap;
		va_start(ap, &formatted_message);
		Common::Interface::GetSingleton()->GetLogger()->WriteString(Logger::tFatalError,
			StringUtils::FormatStringVa(formatted_message.data(), ap));
		va_end(ap);
	}

	CKPE_COMMON_API void _ERROR(const std::string_view& formatted_message, ...)
	{
		va_list ap;
		va_start(ap, &formatted_message);
		Common::Interface::GetSingleton()->GetLogger()->WriteString(Logger::tError,
			StringUtils::FormatStringVa(formatted_message.data(), ap));
		va_end(ap);
	}

	CKPE_COMMON_API void _WARNING(const std::string_view& formatted_message, ...)
	{
		va_list ap;
		va_start(ap, &formatted_message);
		Common::Interface::GetSingleton()->GetLogger()->WriteString(Logger::tWarning,
			StringUtils::FormatStringVa(formatted_message.data(), ap));
		va_end(ap);
	}

	CKPE_COMMON_API void _MESSAGE(const std::string_view& formatted_message, ...)
	{
		va_list ap;
		va_start(ap, &formatted_message);
		Common::Interface::GetSingleton()->GetLogger()->WriteString(Logger::tMessage,
			StringUtils::FormatStringVa(formatted_message.data(), ap));
		va_end(ap);
	}

	CKPE_COMMON_API void _FATALERROR(const std::wstring_view& formatted_message, ...)
	{
		va_list ap;
		va_start(ap, &formatted_message);
		Common::Interface::GetSingleton()->GetLogger()->WriteString(Logger::tFatalError,
			StringUtils::Utf16ToUtf8(StringUtils::FormatStringVa(formatted_message.data(), ap)));
		va_end(ap);
	}

	CKPE_COMMON_API void _ERROR(const std::wstring_view& formatted_message, ...)
	{
		va_list ap;
		va_start(ap, &formatted_message);
		Common::Interface::GetSingleton()->GetLogger()->WriteString(Logger::tError,
			StringUtils::Utf16ToUtf8(StringUtils::FormatStringVa(formatted_message.data(), ap)));
		va_end(ap);
	}

	CKPE_COMMON_API void _WARNING(const std::wstring_view& formatted_message, ...)
	{
		va_list ap;
		va_start(ap, &formatted_message);
		Common::Interface::GetSingleton()->GetLogger()->WriteString(Logger::tWarning,
			StringUtils::Utf16ToUtf8(StringUtils::FormatStringVa(formatted_message.data(), ap)));
		va_end(ap);
	}

	CKPE_COMMON_API void _MESSAGE(const std::wstring_view& formatted_message, ...)
	{
		va_list ap;
		va_start(ap, &formatted_message);
		Common::Interface::GetSingleton()->GetLogger()->WriteString(Logger::tMessage,
			StringUtils::Utf16ToUtf8(StringUtils::FormatStringVa(formatted_message.data(), ap)));
		va_end(ap);
	}
#endif // !CKPE_NO_LOGGER_FUNCTION

	CKPE_COMMON_API bool _READ_OPTION_BOOL(const std::string& section, const std::string& option, bool def)
	{
		return Common::Interface::GetSingleton()->GetSettings()->ReadBool(section, option, def);
	}

	CKPE_COMMON_API char _READ_OPTION_CHAR(const std::string& section, const std::string& option, char def)
	{
		return Common::Interface::GetSingleton()->GetSettings()->ReadChar(section, option, def);
	}

	CKPE_COMMON_API long _READ_OPTION_INT(const std::string& section, const std::string& option, long def)
	{
		return Common::Interface::GetSingleton()->GetSettings()->ReadInt(section, option, def);
	}

	CKPE_COMMON_API unsigned long _READ_OPTION_UINT(const std::string& section, const std::string& option, unsigned long def)
	{
		return Common::Interface::GetSingleton()->GetSettings()->ReadUInt(section, option, def);
	}

	CKPE_COMMON_API unsigned long _READ_OPTION_HEX(const std::string& section, const std::string& option, unsigned long def)
	{
		return Common::Interface::GetSingleton()->GetSettings()->ReadHex(section, option, def);
	}

	CKPE_COMMON_API float _READ_OPTION_FLOAT(const std::string& section, const std::string& option, float def)
	{
		return Common::Interface::GetSingleton()->GetSettings()->ReadFloat(section, option, def);
	}

	CKPE_COMMON_API Common::CustomSettingCollection::color_value _READ_OPTION_RGB(const std::string& section,
		const std::string& option, Common::CustomSettingCollection::color_value def)
	{
		return Common::Interface::GetSingleton()->GetSettings()->ReadRgbColor(section, option, def);
	}

	CKPE_COMMON_API Common::CustomSettingCollection::color_value _READ_OPTION_RGBA(const std::string& section,
		const std::string& option, Common::CustomSettingCollection::color_value def)
	{
		return Common::Interface::GetSingleton()->GetSettings()->ReadRgbaColor(section, option, def);
	}

	CKPE_COMMON_API std::string _READ_OPTION_STR(const std::string& section, const std::string& option,
		const std::string& def)
	{
		return Common::Interface::GetSingleton()->GetSettings()->ReadString(section, option, def);
	}

	CKPE_COMMON_API std::wstring _READ_OPTION_USTR(const std::string& section, const std::string& option,
		const std::wstring& def)
	{
		return Common::Interface::GetSingleton()->GetSettings()->ReadUnicodeString(section, option, def);
	}

	CKPE_COMMON_API void _WRITE_OPTION_BOOL(const std::string& section, const std::string& option, bool value)
	{
		Common::Interface::GetSingleton()->GetSettings()->WriteBool(section, option, value);
	}

	CKPE_COMMON_API void _WRITE_OPTION_CHAR(const std::string& section, const std::string& option, char value)
	{
		Common::Interface::GetSingleton()->GetSettings()->WriteChar(section, option, value);
	}

	CKPE_COMMON_API void _WRITE_OPTION_INT(const std::string& section, const std::string& option, long value)
	{
		Common::Interface::GetSingleton()->GetSettings()->WriteInt(section, option, value);
	}

	CKPE_COMMON_API void _WRITE_OPTION_UINT(const std::string& section, const std::string& option, unsigned long value)
	{
		Common::Interface::GetSingleton()->GetSettings()->WriteUInt(section, option, value);
	}

	CKPE_COMMON_API void _WRITE_OPTION_HEX(const std::string& section, const std::string& option, unsigned long value)
	{
		Common::Interface::GetSingleton()->GetSettings()->WriteHex(section, option, value);
	}

	CKPE_COMMON_API void _WRITE_OPTION_FLOAT(const std::string& section, const std::string& option, float value)
	{
		Common::Interface::GetSingleton()->GetSettings()->WriteFloat(section, option, value);
	}

	CKPE_COMMON_API void _WRITE_OPTION_RGB(const std::string& section, const std::string& option,
		Common::CustomSettingCollection::color_value value)
	{
		Common::Interface::GetSingleton()->GetSettings()->WriteRgbColor(section, option, value);
	}

	CKPE_COMMON_API void _WRITE_OPTION_RGBA(const std::string& section, const std::string& option,
		Common::CustomSettingCollection::color_value value)
	{
		Common::Interface::GetSingleton()->GetSettings()->WriteRgbaColor(section, option, value);
	}

	CKPE_COMMON_API void _WRITE_OPTION_STR(const std::string& section, const std::string& option,
		const std::string& value)
	{
		Common::Interface::GetSingleton()->GetSettings()->WriteString(section, option, value);
	}

	CKPE_COMMON_API void _WRITE_OPTION_USTR(const std::string& section, const std::string& option,
		const std::wstring& value)
	{
		Common::Interface::GetSingleton()->GetSettings()->WriteUnicodeString(section, option, value);
	}
}