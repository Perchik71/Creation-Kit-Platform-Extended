// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Patches/ConsolePatch.h"
#include "LipGen.h"

#define UI_LIPRECORD_DIALOG_STOPRECORD		1
#define UI_LIPRECORD_DIALOG_PROGRESSBAR		31007
#define UI_LIPRECORD_DIALOG_STARTRECORD		(WM_USER + 22)

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_LipGen_sub1 = 0;
			uintptr_t pointer_LipGen_sub2 = 0;
			uintptr_t pointer_LipGen_data = 0;

			LipGenPatch::LipGenPatch() : Module(GlobalEnginePtr)
			{}

			bool LipGenPatch::HasOption() const
			{
				return false;
			}

			bool LipGenPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* LipGenPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* LipGenPatch::GetName() const
			{
				return "LipGen";
			}

			bool LipGenPatch::HasDependencies() const
			{
				return true;
			}

			Array<String> LipGenPatch::GetDependencies() const
			{
				return { "Console" };
			}

			bool LipGenPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				// In version 1.6.1130, this doesn't make sense.
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_1_6_438;
			}

			bool LipGenPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// LipGen
					//
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(0), (uintptr_t)&CreateLipGenProcess);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(1), (uintptr_t)&IsLipDataPresent);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(2), (uintptr_t)&WriteLipData);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(3), (uintptr_t)&IsWavDataPresent);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(4), (uintptr_t)&LipRecordDialogProc);

					pointer_LipGen_sub1 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(5));
					pointer_LipGen_sub2 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(6));
					pointer_LipGen_data = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(7));

					return true;
				}

				return false;
			}

			bool LipGenPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void LipGenPatch::CreateLipGenProcess(__int64 a1)
			{
				char procIdString[32];
				sprintf_s(procIdString, "%d", GetCurrentProcessId());
				SetEnvironmentVariableA("Ckpid", procIdString);

				char procToolPath[MAX_PATH];
				strcpy_s(procToolPath, (const char*)(a1 + 0x0));
				strcat_s(procToolPath, (const char*)(a1 + 0x104));

				auto procInfo = (PROCESS_INFORMATION*)(a1 + 0x228);
				memset(procInfo, 0, sizeof(PROCESS_INFORMATION));

				STARTUPINFOA startupInfo
				{
					.cb = sizeof(STARTUPINFOA)
				};

				if (_READ_OPTION_BOOL("CreationKit", "bLipDebugOutput", false) && 
					GlobalConsoleWindowPtr->CreateStdoutListener())
				{
					startupInfo.dwFlags |= STARTF_USESTDHANDLES;
					startupInfo.hStdError = GlobalConsoleWindowPtr->GetStdoutListenerPipe();
					startupInfo.hStdOutput = GlobalConsoleWindowPtr->GetStdoutListenerPipe();
					startupInfo.hStdInput = nullptr;
				}

				if (!CreateProcessA(procToolPath, nullptr, nullptr, nullptr, TRUE, 0, nullptr, nullptr, &startupInfo, procInfo))
					ConsolePatch::Log("FaceFXWrapper could not be started (%d). LIP generation will be disabled.\n", GetLastError());
				else
					ConsolePatch::Log("FaceFXWrapper background process started.\n");
			}

			bool LipGenPatch::IsLipDataPresent(void* Thisptr)
			{
				char currentDir[MAX_PATH];
				GetCurrentDirectory(MAX_PATH, currentDir);
				strcat_s(currentDir, "\\Data\\Sound\\Voice\\Processing\\Temp.lip");

				return GetFileAttributesA(currentDir) != INVALID_FILE_ATTRIBUTES;
			}

			bool LipGenPatch::WriteLipData(void* Thisptr, const char* Path, int Unkown1, bool Unknown2, bool Unknown3)
			{
				char srcDir[MAX_PATH];
				GetCurrentDirectory(MAX_PATH, srcDir);
				strcat_s(srcDir, "\\Data\\Sound\\Voice\\Processing\\Temp.lip");

				char destDir[MAX_PATH];
				GetCurrentDirectory(MAX_PATH, destDir);
				strcat_s(destDir, "\\");
				strcat_s(destDir, Path);

				bool status = MoveFileEx(srcDir, destDir, MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) != FALSE;

				ConsolePatch::LogWarning(6, "Moving temporary LIP file to '%s' (%s)", destDir, status ? "Succeeded" : "Failed");
				return status;
			}

			int LipGenPatch::IsWavDataPresent(const char* Path, __int64 a2, __int64 a3, __int64 a4)
			{
				return ((int(__fastcall*)(const char*, __int64, __int64, __int64))pointer_LipGen_sub1)
					("Sound\\Voice\\Temp.wav", a2, a3, a4);
			}

			INT_PTR CALLBACK LipGenPatch::LipRecordDialogProc(HWND DialogHwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				// Id's for "Recording..." dialog window
				switch (Message)
				{
				case WM_APP:
					// Don't actually kill the dialog, just hide it. It gets destroyed later when the parent window closes.
					SendMessageA(GetDlgItem(DialogHwnd, UI_LIPRECORD_DIALOG_PROGRESSBAR), PBM_SETPOS, 0, 0);
					ShowWindow(DialogHwnd, SW_HIDE);
					PostQuitMessage(0);
					return TRUE;

				case WM_INITDIALOG:
					SendMessageA(GetDlgItem(DialogHwnd, UI_LIPRECORD_DIALOG_PROGRESSBAR), PBM_SETRANGE, 0, 32768 * 1000);
					SendMessageA(GetDlgItem(DialogHwnd, UI_LIPRECORD_DIALOG_PROGRESSBAR), PBM_SETSTEP, 1, 0);
					return TRUE;

				case WM_COMMAND:
					// Stop recording
					if (LOWORD(wParam) != UI_LIPRECORD_DIALOG_STOPRECORD)
						return FALSE;

					*reinterpret_cast<bool*>(pointer_LipGen_data) = false;

					if (FAILED(((HRESULT(__fastcall*)(bool))pointer_LipGen_sub2)(false)))
						MessageBoxA(DialogHwnd, "Error with DirectSoundCapture buffer.", "DirectSound Error", MB_ICONERROR);

					return LipRecordDialogProc(DialogHwnd, WM_APP, 0, 0);

				case UI_LIPRECORD_DIALOG_STARTRECORD:
					// Start recording
					ShowWindow(DialogHwnd, SW_SHOW);
					*reinterpret_cast<bool*>(pointer_LipGen_data) = true;

					if (FAILED(((HRESULT(__fastcall*)(bool))pointer_LipGen_sub2)(true)))
					{
						MessageBoxA(DialogHwnd, "Error with DirectSoundCapture buffer.", "DirectSound Error", MB_ICONERROR);
						return LipRecordDialogProc(DialogHwnd, WM_APP, 0, 0);
					}
					return TRUE;
				}

				return FALSE;
			}
		}
	}
}