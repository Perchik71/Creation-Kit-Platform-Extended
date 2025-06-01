#pragma warning (disable : 4390)
#pragma warning (disable : 6335)

#include <iostream>
#include <algorithm>
#include <memory>
#include <string>
#include <map>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <windows.h>
#include <windowsx.h>
#include <shellapi.h>
#include <tlhelp32.h>

#include "resource.h"

#define DEBUGLOG	0
#define CREATIONKIT L"CreationKit.exe"

std::map<std::wstring, BOOL> dllENBs = {
	{ L"d3d12.dll", FALSE },
	{ L"d3d11.dll", FALSE },
	{ L"d3d10.dll", FALSE },
	{ L"d3d9.dll", FALSE },
	{ L"d3dcompiler_46e.dll", FALSE },
	{ L"dxgi.dll", FALSE },
	{ L"dinput8.dll", FALSE },
};

static constexpr const char steamapi_dllname[] = "steam_api64";

void RestoreFiles(const std::wstring& AppPath);

#if DEBUGLOG

FILE* DebugLog = nullptr;

class ScopeFileStream
{
public:
	ScopeFileStream(FILE* Stream) : _Stream(Stream) {}
	~ScopeFileStream() { fclose(_Stream); }
private:
	FILE* _Stream;
};

void _MESSAGE(const wchar_t* Format, ...)
{
	if (DebugLog)
	{
		va_list ap;
		va_start(ap, Format);
		vfwprintf_s(DebugLog, Format, ap);
		va_end(ap);
		fputc('\n', DebugLog);
		fflush(DebugLog);
	}
}

class ScopeFunction
{
public:
	ScopeFunction(const wchar_t* Name) : _Name(Name) { _MESSAGE(L"Start \"%s\"", _Name.c_str()); }
	~ScopeFunction() { _MESSAGE(L"End \"%s\"", _Name.c_str()); }
private:
	std::wstring _Name;
};

#else
#	define _MESSAGE(Format, ...)
#endif

INT_PTR CALLBACK DlgPleaseWaitProc(HWND DialogHwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

void HideConsole()
{
#if DEBUGLOG
	ScopeFunction Foo(L"HideConsole");
#endif

	ShowWindow(GetConsoleWindow(), SW_HIDE);
}

BOOL FileExists(const std::wstring fname) 
{
	auto dwAttrs = GetFileAttributes(fname.c_str());
	if (dwAttrs == INVALID_FILE_ATTRIBUTES) return FALSE;
	return (dwAttrs & FILE_ATTRIBUTE_DIRECTORY) == 0;
}

VOID WaitCloseCKLoader(VOID)
{
#if DEBUGLOG
	ScopeFunction Foo(L"WaitCloseCKLoader");
#endif

	DWORD dwCount = 0;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hProcess = INVALID_HANDLE_VALUE;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	DWORD dwCurrentProcessID = GetCurrentProcessId();

	if (Process32First(snapshot, &entry) == TRUE) {
		while (Process32Next(snapshot, &entry) == TRUE) {
			if (entry.th32ProcessID != dwCurrentProcessID && !_wcsicmp(entry.szExeFile, L"ckpe_loader.exe")) {
				dwCount++;
				if (dwCount >= 2)
					// user spam run
					TerminateProcess(GetCurrentProcess(), 0);

				hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
			}
		}
	}

	CloseHandle(snapshot);

	if (hProcess != INVALID_HANDLE_VALUE) {

		HWND hDlg = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_PLEASEWAIT), NULL, &DlgPleaseWaitProc);

		WaitForSingleObject(hProcess, INFINITE);

		EndDialog(hDlg, 0);
		DestroyWindow(hDlg);
	}
}

void RenameFiles(const std::wstring& AppPath)
{
#if DEBUGLOG
	ScopeFunction Foo(L"RenameFiles");
#endif

	for (auto it = dllENBs.begin(); it != dllENBs.end(); it++)
	{
		auto sname = AppPath + it->first;
		if (FileExists(sname.c_str()))
		{
			if (MoveFile(sname.c_str(), sname.substr(0, sname.length() - 1).append(L"_").c_str()))
				it->second = TRUE;
			else
			{
				RestoreFiles(AppPath);

				wchar_t* szBuf = new wchar_t[1024];
				swprintf_s(szBuf, 1024, L"It was not possible to rename the file \"%s\", most likely it is occupied by another process. "
					"There will be a forced shutdown of the application.", it->first.data());
				MessageBox(0, szBuf, L"Error", MB_OK | MB_ICONERROR);
				delete[] szBuf;

				TerminateProcess(GetCurrentProcess(), 1);
			}
		}
	}
}

void RestoreFiles(const std::wstring& AppPath)
{
#if DEBUGLOG
	ScopeFunction Foo(L"RestoreFiles");
#endif

	for (auto it = dllENBs.begin(); it != dllENBs.end(); it++)
	{
		if (it->second)
		{
			auto sname = AppPath + it->first;
			MoveFile(sname.substr(0, sname.length() - 1).append(L"_").c_str(), sname.c_str());
			it->second = FALSE;
		}
	}
}

void RunCK(const std::wstring& AppPath)
{
#if DEBUGLOG
	ScopeFunction Foo(L"RunCK");
#endif

	std::wstring Cmd = GetCommandLineW();
	if (Cmd.find_first_of(L"\"") == 0)
		Cmd = Cmd.substr((Cmd.find_first_of(L"\"", 1)) + 1);
	_MESSAGE(L"Command Line: %s", Cmd.empty() ? L"null" : Cmd.c_str());

	std::wstring CurPath;
	CurPath.resize(MAX_PATH);
	GetCurrentDirectoryW((DWORD)CurPath.length(), CurPath.data());
	_MESSAGE(L"Current Directory: %s", CurPath.c_str());

	if (AppPath != CurPath)
		_MESSAGE(L"The directories do not match, it is possible to run from the manager");

	if (!FileExists(AppPath + CREATIONKIT))
	{
		MessageBoxW(0, CREATIONKIT " was not found", L"Error", MB_OK | MB_ICONERROR);
		return;
	}

	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	si.cb = sizeof(STARTUPINFO);

	std::wstring Command;
	Command.resize(MAX_PATH);
	Command.resize(swprintf_s(Command.data(), MAX_PATH, L"\"%s\\%s\"", AppPath.c_str(), CREATIONKIT));
	if (!Cmd.empty()) Command.append(Cmd.c_str());
	_MESSAGE(L"Command: %s", Command.c_str());

	if (!CreateProcess(NULL, Command.data(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		return;

	Sleep(15000);
}

void SetAddressOfBaseFixed(const std::wstring& AppPath)
{
	if (!FileExists(AppPath + CREATIONKIT))
		return;

	FILE* Stream = nullptr;
	_wfopen_s(&Stream, (AppPath + CREATIONKIT).c_str(), L"rb+");
	if (!Stream)
		return;

	IMAGE_DOS_HEADER DosHeader = { 0 };
	fread(&DosHeader, sizeof(IMAGE_DOS_HEADER), 1, Stream);
	fseek(Stream, DosHeader.e_lfanew, SEEK_SET);

	IMAGE_NT_HEADERS ntHeaders = { 0 };
	fread(&ntHeaders, sizeof(IMAGE_NT_HEADERS), 1, Stream);
	ntHeaders.FileHeader.Characteristics =
		IMAGE_FILE_RELOCS_STRIPPED | IMAGE_FILE_EXECUTABLE_IMAGE | IMAGE_FILE_LARGE_ADDRESS_AWARE;
	fseek(Stream, DosHeader.e_lfanew, SEEK_SET);
	fwrite(&ntHeaders, sizeof(IMAGE_NT_HEADERS), 1, Stream);

	fclose(Stream);

	MessageBoxW(0, CREATIONKIT" Patched. Close.", L"Info", MB_OK | MB_ICONINFORMATION);
}

void SetAddressOfBaseFloating(const std::wstring& AppPath)
{
	if (!FileExists(AppPath + CREATIONKIT))
		return;

	FILE* Stream = nullptr;
	_wfopen_s(&Stream, (AppPath + CREATIONKIT).c_str(), L"rb+");
	if (!Stream)
		return;

	IMAGE_DOS_HEADER DosHeader = { 0 };
	fread(&DosHeader, sizeof(IMAGE_DOS_HEADER), 1, Stream);
	fseek(Stream, DosHeader.e_lfanew, SEEK_SET);

	IMAGE_NT_HEADERS ntHeaders = { 0 };
	fread(&ntHeaders, sizeof(IMAGE_NT_HEADERS), 1, Stream);
	ntHeaders.FileHeader.Characteristics =
		IMAGE_FILE_EXECUTABLE_IMAGE | IMAGE_FILE_LARGE_ADDRESS_AWARE;
	fseek(Stream, DosHeader.e_lfanew, SEEK_SET);
	fwrite(&ntHeaders, sizeof(IMAGE_NT_HEADERS), 1, Stream);

	fclose(Stream);

	MessageBoxW(0, CREATIONKIT" Patched. Close.", L"Info", MB_OK | MB_ICONINFORMATION);
}

void SetRedirectSteamAPI(const std::wstring& AppPath)
{
	if (!FileExists(AppPath + CREATIONKIT))
		return;

	// Algorithm is imperfect, and there may be exceptional situations

	FILE* Stream = nullptr;
	_wfopen_s(&Stream, (AppPath + CREATIONKIT).c_str(), L"rb+");
	if (!Stream)
		return;

	std::string Pattern = steamapi_dllname;
	std::string PatternWinhttp = "winhttp";
	std::string PatternDll = ".dll";

	auto Buffer = std::make_unique<char[]>(2048);
	size_t Total = 0;
	while (!feof(Stream))
	{
		auto ReadBytes = fread(Buffer.get(), 1, 2048, Stream);
		if (!ReadBytes) break;

		auto Start = Buffer.get();
		auto End = Start + ReadBytes;
		while (true)
		{
			auto Result = std::search(Start, End, Pattern.begin(), Pattern.end());
			if (Result >= End)
				break;

			if (*(Result + Pattern.size()) == '\x00')
			{
				auto SafePos = ftell(Stream);
				auto Pos = ((size_t)(Result - Buffer.get())) + Total;
				fseek(Stream, (long)Pos, SEEK_SET);
				fwrite(PatternWinhttp.c_str(), PatternWinhttp.size(), 1, Stream);
				
				char ch = 0;
				for(int i = 0; i < (int)(Pattern.size() - PatternWinhttp.size()); i++)
					fwrite(&ch, 1, 1, Stream);

				fseek(Stream, SafePos, SEEK_SET);
			}
			else if (*(Result + Pattern.size()) == '.')
			{
				auto SafePos = ftell(Stream);
				auto Pos = ((size_t)(Result - Buffer.get())) + Total;
				fseek(Stream, (long)Pos, SEEK_SET);
				fwrite(PatternWinhttp.c_str(), PatternWinhttp.size(), 1, Stream);
				fwrite(PatternDll.c_str(), PatternDll.size(), 1, Stream);

				char ch = 0;
				for (int i = 0; i < (int)(Pattern.size() - PatternWinhttp.size()); i++)
					fwrite(&ch, 1, 1, Stream);

				fseek(Stream, SafePos, SEEK_SET);
			}
			else
			{
				MessageBoxA(0, "An unexpected find pattern. Close.", "Warning", MB_OK | MB_ICONWARNING);
				return;
			}

			Start = Result + Pattern.size();
		}

		Total += ReadBytes;
	}

	fclose(Stream);

	MessageBoxW(0, CREATIONKIT" Patched. Close.", L"Info", MB_OK | MB_ICONINFORMATION);
}

int main(int argc, char* argv[])
{
#if DEBUGLOG
	DebugLog = _wfopen(L"F:\\SteamLibrary\\steamapps\\common\\Skyrim Special Edition\\ckpe_loader.log", L"w+");
	ScopeFileStream Stream(DebugLog);
#endif

	std::wstring AppPath;
	AppPath.resize(MAX_PATH);
	GetModuleFileNameW(GetModuleHandle(NULL), AppPath.data(), (DWORD)AppPath.length());
	AppPath = AppPath.substr(0, AppPath.find_last_of(L"\\") + 1);
	_MESSAGE(L"Application Directory: %s", AppPath.c_str());

	HideConsole();
	WaitCloseCKLoader();

	if ((argc > 2) && !_stricmp(argv[1], "-c"))
	{
		if (!_stricmp(argv[2], "address_of_base_fixed"))
		{
			SetAddressOfBaseFixed(AppPath);
			return 0;
		}
		else if (!_stricmp(argv[2], "address_of_base_floating"))
		{
			SetAddressOfBaseFloating(AppPath);
			return 0;
		}
		else if (!_stricmp(argv[2], "redirect_steam_api"))
		{
			SetRedirectSteamAPI(AppPath);
			return 0;
		}
	}

	RenameFiles(AppPath);
	RunCK(AppPath);
	RestoreFiles(AppPath);

	return 0;
}