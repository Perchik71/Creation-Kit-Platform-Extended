// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <commctrl.h>
#include <CKPE.ErrorHandler.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.Console.h>
#include <Patches/CKPE.SkyrimSE.Patch.LipGen.h>

#define UI_LIPRECORD_DIALOG_STOPRECORD		1
#define UI_LIPRECORD_DIALOG_PROGRESSBAR		31007
#define UI_LIPRECORD_DIALOG_STARTRECORD		(WM_USER + 22)

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			std::uintptr_t pointer_LipGen_sub1 = 0;
			std::uintptr_t pointer_LipGen_sub2 = 0;
			std::uintptr_t pointer_LipGen_data = 0;

			LipGen::LipGen() : Common::Patch()
			{
				SetName("LipGen");
			}

			bool LipGen::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* LipGen::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool LipGen::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> LipGen::GetDependencies() const noexcept(true)
			{
				return { "Console" };
			}

			bool LipGen::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_1_6_438;
			}

			bool LipGen::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto _interface = CKPE::Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				//
				// LipGen
				//
				Detours::DetourJump(__CKPE_OFFSET(0), (std::uintptr_t)&CreateLipGenProcess);
				Detours::DetourJump(__CKPE_OFFSET(1), (std::uintptr_t)&IsLipDataPresent);
				Detours::DetourJump(__CKPE_OFFSET(2), (std::uintptr_t)&WriteLipData);
				Detours::DetourCall(__CKPE_OFFSET(3), (std::uintptr_t)&IsWavDataPresent);
				Detours::DetourJump(__CKPE_OFFSET(4), (std::uintptr_t)&LipRecordDialogProc);

				pointer_LipGen_sub1 = __CKPE_OFFSET(5);
				pointer_LipGen_sub2 = __CKPE_OFFSET(6);
				pointer_LipGen_data = __CKPE_OFFSET(7);

				return true;
			}

			void LipGen::CreateLipGenProcess(std::int64_t a1) noexcept(true)
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
					Common::LogWindow::GetSingleton()->CreateStdoutListener())
				{
					startupInfo.dwFlags |= STARTF_USESTDHANDLES;
					startupInfo.hStdError = Common::LogWindow::GetSingleton()->GetStdoutListenerPipe();
					startupInfo.hStdOutput = Common::LogWindow::GetSingleton()->GetStdoutListenerPipe();
					startupInfo.hStdInput = nullptr;
				}

				if (!CreateProcessA(procToolPath, nullptr, nullptr, nullptr, true, 0, nullptr, nullptr, 
					&startupInfo, procInfo))
					Console::LogWarning(Console::DIALOGUE,
						"FaceFXWrapper could not be started (%d : %s). LIP generation will be disabled.", 
						GetLastError(), ErrorHandler::GetSystemMessage(GetLastError()).c_str());
				else
					Console::LogWarning(Console::DIALOGUE, "FaceFXWrapper background process started.");
			}

			bool LipGen::IsLipDataPresent(void* Thisptr) noexcept(true)
			{
				char currentDir[MAX_PATH];
				GetCurrentDirectory(MAX_PATH, currentDir);
				strcat_s(currentDir, "\\Data\\Sound\\Voice\\Processing\\Temp.lip");

				return GetFileAttributesA(currentDir) != INVALID_FILE_ATTRIBUTES;
			}

			bool LipGen::WriteLipData(void* Thisptr, const char* Path, std::int32_t Unkown1, bool Unknown2,
				bool Unknown3) noexcept(true)
			{
				char srcDir[MAX_PATH];
				GetCurrentDirectory(MAX_PATH, srcDir);
				strcat_s(srcDir, "\\Data\\Sound\\Voice\\Processing\\Temp.lip");

				char destDir[MAX_PATH];
				GetCurrentDirectory(MAX_PATH, destDir);
				strcat_s(destDir, "\\");
				strcat_s(destDir, Path);

				bool status = MoveFileEx(srcDir, destDir, MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING |
					MOVEFILE_WRITE_THROUGH) != FALSE;

				Console::LogWarning(Console::DIALOGUE, "Moving temporary LIP file to '%s' (%s)",
					destDir, status ? "Succeeded" : "Failed");
				return status;
			}

			std::int32_t LipGen::IsWavDataPresent(const char* Path, std::int64_t a2, std::int64_t a3,
				std::int64_t a4) noexcept(true)
			{
				return ((std::int32_t(__fastcall*)(const char*, std::int64_t, std::int64_t, 
					std::int64_t))pointer_LipGen_sub1)("Sound\\Voice\\Temp.wav", a2, a3, a4);
			}

			INT_PTR CALLBACK LipGen::LipRecordDialogProc(HWND DialogHwnd, UINT Message, WPARAM wParam, 
				LPARAM lParam) noexcept(true)
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
					SendMessageA(GetDlgItem(DialogHwnd, UI_LIPRECORD_DIALOG_PROGRESSBAR), PBM_SETRANGE, 0, 
						32768ull * 1000);
					SendMessageA(GetDlgItem(DialogHwnd, UI_LIPRECORD_DIALOG_PROGRESSBAR), PBM_SETSTEP,
						1, 0);
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