#include <iostream>
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

#define CREATIONKIT L"CreationKit.exe"

std::map<std::wstring, BOOL> dllENBs = {
	{ L"d3d11.dll", FALSE },
	{ L"d3d10.dll", FALSE },
	{ L"d3d9.dll", FALSE },
	{ L"d3dcompiler_46e.dll", FALSE },
	{ L"dxgi.dll", FALSE },
	{ L"dinput8.dll", FALSE },
};

INT_PTR CALLBACK DlgPleaseWaitProc(HWND DialogHwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	return FALSE;
}

void HideConsole()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
}

BOOL FileExists(const std::wstring fname) {
	auto dwAttrs = GetFileAttributes(fname.c_str());
	if (dwAttrs == INVALID_FILE_ATTRIBUTES) return FALSE;
	return (dwAttrs & FILE_ATTRIBUTE_DIRECTORY) == 0;
}

VOID WaitCloseCKLoader(VOID) {
	DWORD dwCount = 0;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hProcess = INVALID_HANDLE_VALUE;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	DWORD dwCurrentProcessID = GetCurrentProcessId();

	if (Process32First(snapshot, &entry) == TRUE) {
		while (Process32Next(snapshot, &entry) == TRUE) {
			if (entry.th32ProcessID != dwCurrentProcessID && !_wcsicmp(entry.szExeFile, L"sseck_loader.exe")) {
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

void RenameFiles()
{
	for (auto it = dllENBs.begin(); it != dllENBs.end(); it++)
	{
		auto sname = it->first;
		if (FileExists(sname.c_str()))
		{
			MoveFile(sname.c_str(), sname.substr(0, sname.length() - 1).append(L"_").c_str());
			it->second = TRUE;
		}
	}
}

void RestoreFiles()
{
	for (auto it = dllENBs.begin(); it != dllENBs.end(); it++)
	{
		if (it->second)
		{
			auto sname = it->first;
			MoveFile(sname.substr(0, sname.length() - 1).append(L"_").c_str(), sname.c_str());
			it->second = FALSE;
		}
	}
}

void RunCK()
{
	if (!FileExists(CREATIONKIT))
		return;
	
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(si)); si.cb = sizeof(si);
	memset(&pi, 0, sizeof(pi));

	std::wstring Cmd = GetCommandLine();
	if (Cmd.find_first_of(L"\"") == 0)
		Cmd = Cmd.substr((Cmd.find_first_of(L"\"", 1)) + 2);

	Cmd.insert(0, CREATIONKIT" ");
	if (!CreateProcess(NULL, Cmd.data(), NULL, NULL,
						FALSE, 0, NULL, NULL, &si, &pi))
		return;

	Sleep(15000);
}

int main(int argc, char* argv[])
{
	HideConsole();
	WaitCloseCKLoader();

	RenameFiles();
	RunCK();
	RestoreFiles();

	return 0;
}