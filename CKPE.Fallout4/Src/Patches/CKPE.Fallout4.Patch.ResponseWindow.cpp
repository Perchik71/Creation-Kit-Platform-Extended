// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.StringUtils.h>
#include <CKPE.PathUtils.h>
#include <CKPE.Detours.h>
#include <CKPE.Graphics.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/Forms/TESTopic.h>
#include <EditorAPI/Forms/BGSVoiceType.h>
#include <Patches/CKPE.Fallout4.Patch.Console.h>
#include <Patches/CKPE.Fallout4.Patch.ResponseWindow.h>

#include <shellapi.h>
#include <commctrl.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			static bool EnableLipGeneration = false;
			static bool EnableFuzGeneration = false;

			std::uintptr_t pointer_ResponseWindow_data = 0;
			std::uintptr_t pointer_ResponseWindow_sub1 = 0;
			std::uintptr_t pointer_ResponseWindow_sub2 = 0;
			std::uintptr_t pointer_ResponseWindow_sub3 = 0;

			static std::string V_LANG = "USEnglish";
			static float V_GESTUREEXAGGERATION = 1.f;

			static const char* V_TOOLPATH = "Tools\\LipGen\\LipGenerator\\";
			static const char* V_WRAPPER = "LipGenerator.exe";
			static const char* V_XWMAENCD = "xWMAEncode.exe";
			static const char* V_FUZTOOLS = "fuz_extractor.exe";
			static const char* V_FFMPEG = "ffmpeg.exe";
			static const char* V_FXDATA = "FonixData.cdf";
			static const char* V_CMD_FMT = "\"%s\" \"%s\" \"%s\" -OutputFileName:\"%s\" -Language:\"%s\" -GestureExaggeration:%.1f";
			static const char* V_CMD_FMT2 = "\"%s\" -i \"%s\" -ac 1 -acodec pcm_s16le -ar 16000 \"%s\"";
			static const char* V_CMD_FMT3 = "\"%s\" -i \"%s\" -ac 2 -acodec pcm_s16le -ab 192000 -ar 44100 \"%s\"";
			static const char* V_CMD_FMT4 = "\"%s\" -c \"%s\" \"%s\" \"%s\"";
			static const char* V_CMD_FMT5 = "\"%s\" -b 192000 \"%s\" \"%s\"";

			typedef struct RESPONSE_DATA
			{
				const char* Title;
				std::uint64_t UnkParm;
				EditorAPI::Forms::TESTopicInfo::Response* ResponseSource;
				EditorAPI::Forms::TESTopicInfo::Response* ResponseTemp;
				EditorAPI::Forms::TESTopic* Topic;
				EditorAPI::Forms::TESTopicInfo* TopicInfo;
				struct AUDIO
				{
					EditorAPI::Forms::BGSVoiceType* VoiceType;
					char pad08[0x4];
					char AudioFileName[MAX_PATH];
				} Audio;
			} *LPRESPONSE_DATA;

			ResponseWindow::ResponseWindow() : Common::PatchBaseWindow()
			{
				SetName("Response Window");
				Singleton = this;
			}

			bool ResponseWindow::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* ResponseWindow::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool ResponseWindow::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> ResponseWindow::GetDependencies() const noexcept(true)
			{
				return { "Console" };
			}

			bool ResponseWindow::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool ResponseWindow::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto verPatch = db->GetVersion();
				if ((verPatch != 1) && (verPatch != 2))
					return false;

				auto _interface = Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				*(std::uintptr_t*)&_oldWndProc = Detours::DetourClassJump(__CKPE_OFFSET(0), (std::uintptr_t)&HKWndProc);

				pointer_ResponseWindow_data = __CKPE_OFFSET(1);
				pointer_ResponseWindow_sub1 = __CKPE_OFFSET(2);
				pointer_ResponseWindow_sub2 = __CKPE_OFFSET(3);
				pointer_ResponseWindow_sub3 = __CKPE_OFFSET(4);

				// Hooking the jump. I don't allow the deny button.
				if (verPatch == 1)
					Detours::DetourJump(__CKPE_OFFSET(5), (std::uintptr_t)&sub);
				else
					Detours::DetourCall(__CKPE_OFFSET(5), (std::uintptr_t)&sub);
				// Then continue in the same spirit, remove the button.... skip it
				SafeWrite::WriteNop(__CKPE_OFFSET(6), 25);
				// In case of cancellation .wav triggers and closes the button, we will remove everything
				Detours::DetourCall(__CKPE_OFFSET(7), (std::uintptr_t)&sub);
				// Hook generate via LipGenerator (By the way, Bethesda does not post it)
				Detours::DetourJump(__CKPE_OFFSET(8), (std::uintptr_t)&sub2);

				V_LANG = _READ_OPTION_STR("LipGen", "sLanguage", "USEnglish");
				V_GESTUREEXAGGERATION = _READ_OPTION_FLOAT("LipGen", "fGestureExaggeration", 1.f);

				return true;
			}

			INT_PTR CALLBACK ResponseWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				if (Message == WM_INITDIALOG)
				{
					auto _This = ResponseWindow::Singleton.GetSingleton();

					_This->m_hWnd = Hwnd;
					_This->m_List = GetDlgItem(Hwnd, 2168);

					EnableWindow(GetDlgItem(Hwnd, 1017), FALSE);
					
					/*
					In the generation of.lip for Fallout 4, you need to consider the following:
					For high - quality communication with voice and time, the audio file must be in mono
					format at 16kHz per sample.
					1. ffmpeg will help convert the audio file to a suitable format. Can't delete the original,
					will need it in the future.
					2. Do it .lip file. And delete the temporary audio file.
					3. Improve the quality .wav to convert to the format .xwm in a higher quality form.
					4. Assemble .fuz deleting the .xwm file, but leaving .lip file.
					*/

					auto LipGenPath = StringUtils::Utf16ToWinCP(PathUtils::GetApplicationPath()) + V_TOOLPATH;

					EnableLipGeneration =
						PathUtils::FileExists(LipGenPath + V_WRAPPER) &&
						PathUtils::FileExists(LipGenPath + V_FXDATA);

					EnableFuzGeneration =
						PathUtils::FileExists(LipGenPath + V_XWMAENCD) &&
						PathUtils::FileExists(LipGenPath + V_FUZTOOLS) &&
						PathUtils::FileExists(LipGenPath + V_FFMPEG);

					if (!EnableLipGeneration)
						Console::LogWarning(Console::EDITOR, "\"%s\", \"%s\", \"%s\" is missing from \"<GAMEDIR>%s\" directory."
							" LIP generation will be disabled.", V_WRAPPER, V_FXDATA, V_FFMPEG, LipGenPath.c_str());

					if (!EnableFuzGeneration)
						Console::LogWarning(Console::EDITOR, "\"%s\", \"%s\" is missing from \"<GAMEDIR>%s\" directory."
							" FUZ generation will be disabled.", V_XWMAENCD, V_FUZTOOLS, LipGenPath.c_str());
				}
				else if (Message == WM_COMMAND)
				{
					switch (const UINT32 param = LOWORD(wParam); param)
					{
						// "Generate Lip File"
					case 1016:
						// "Generate Fuz File"
					case 1017:
						// "From WAV"
					case 2379:
					{
						auto item = (RESPONSE_DATA::AUDIO*)
							Common::EditorUI::ListViewGetSelectedItem(ResponseWindow::Singleton.GetSingleton()->m_List.Handle);

						// "Generate Lip File"
						if (param == 1016)
						{
							if (!EnableLipGeneration || !item)
								return S_FALSE;

							auto AudioFilePath = StringUtils::Utf16ToWinCP(PathUtils::GetApplicationPath()) +
								item->AudioFileName;

							// only .wav
							AudioFilePath = PathUtils::ChangeFileExt(AudioFilePath, ".wav");
							if (!PathUtils::FileExists(AudioFilePath.c_str()))
							{
								Console::LogWarning(Console::EDITOR, "File \"%s\" no found. Trying WAV extension fallback.",
									AudioFilePath.c_str());
								MessageBoxA(Hwnd, "Unable to find audio file on disk", "Error", MB_ICONERROR);

								return S_FALSE;
							}

							auto LipFilePath = PathUtils::ChangeFileExt(AudioFilePath, ".lip");

							auto data = *(LPRESPONSE_DATA*)pointer_ResponseWindow_data;

							if (!GenerationLip(AudioFilePath.c_str(), LipFilePath.c_str(), 
								data->ResponseTemp->ResponseText.c_str()) || !PathUtils::FileExists(LipFilePath.c_str()))
							{
								Console::LogWarning(Console::EDITOR, "File \"%s\" LIP generation failed.", LipFilePath.c_str());
								return S_FALSE;
							}

							MessageBoxA(0, "Done!", "Message", MB_OK);

							return S_OK;
						}
						// "Generate Lip File"
						else if (param == 1017)
						{
							if (!EnableFuzGeneration || !item)
								return S_FALSE;

							auto AudioFilePath = StringUtils::Utf16ToWinCP(PathUtils::GetApplicationPath()) + 
								item->AudioFileName;

							// only .wav
							AudioFilePath = PathUtils::ChangeFileExt(AudioFilePath, ".wav");
							if (!PathUtils::FileExists(AudioFilePath.c_str()))
							{
								Console::LogWarning(Console::EDITOR, "File \"%s\" no found. Trying WAV extension fallback.",
									AudioFilePath.c_str());
								MessageBoxA(Hwnd, "Unable to find audio file on disk", "Error", MB_ICONERROR);

								return S_FALSE;
							}

							auto LipFilePath = PathUtils::ChangeFileExt(AudioFilePath, ".lip");
							if (!PathUtils::FileExists(LipFilePath.c_str()))
							{
								Console::LogWarning(Console::EDITOR, "File \"%s\" no found. Trying LIP extension fallback.",
									LipFilePath.c_str());
								MessageBoxA(Hwnd, "Unable to find lip file on disk", "Error", MB_ICONERROR);

								return S_FALSE;
							}

							auto FuzFilePath = PathUtils::ChangeFileExt(AudioFilePath, ".fuz");
							if (!GenerationFuz(FuzFilePath.c_str(), AudioFilePath.c_str(), LipFilePath.c_str()) ||
								!PathUtils::FileExists(LipFilePath.c_str()))
							{
								Console::LogWarning(Console::EDITOR, "File \"%s\" FUZ generation failed.", FuzFilePath.c_str());
								return S_FALSE;
							}

							MessageBoxA(0, "Done!", "Message", MB_OK);

							return S_OK;
						}
					}
					}
				}
				else if (Message == WM_NOTIFY)
				{
					auto notify = (LPNMHDR)lParam;

					if (notify->code == LVN_ITEMCHANGED && notify->idFrom == 2168)
					{
						Common::EditorUI::Hook::HKSendMessageA(Hwnd, WM_COMMAND, 2379, 0);
						EnableWindow(GetDlgItem(Hwnd, 1017), ListView_GetSelectedCount(notify->hwndFrom) > 0);
					}
				}

				return CallWindowProc(ResponseWindow::Singleton->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}

			void ResponseWindow::sub(HWND hWndButtonGenerate) noexcept(true)
			{
				HWND hDlg = GetParent(hWndButtonGenerate);
				HWND hList = GetDlgItem(hDlg, 0x878);
				HWND hLTFCheck = GetDlgItem(hDlg, 0x94C);

				// no support .ltf
				EnableWindow(hLTFCheck, FALSE);
				// wav check default
				CheckDlgButton(hDlg, 0x94B, BST_CHECKED);

				bool bEnableGenerate = false;

				if (int iCount = ListView_GetItemCount(hList); iCount > 0) 
				{
					if (auto item = (std::int64_t)Common::EditorUI::ListViewGetSelectedItem(hList); item)
					{
						bEnableGenerate =
							IsDlgButtonChecked(hDlg, 2379) && *(std::uint32_t*)(item + 0x110) ||
							IsDlgButtonChecked(hDlg, 2380) && *(std::uint32_t*)(item + 0x118);
					}
				}

				EnableWindow(hWndButtonGenerate, bEnableGenerate);
			}

			bool ResponseWindow::sub2(const char* AudioPath, const char* ResponseText, const char* LipPath) noexcept(true)
			{
				return !GenerationLip(
					(StringUtils::Utf16ToWinCP(PathUtils::GetApplicationPath()) + AudioPath).c_str(),
					(StringUtils::Utf16ToWinCP(PathUtils::GetApplicationPath()) + LipPath).c_str(),
					ResponseText);
			}

			bool ResponseWindow::ExecuteApplication(const char* CmdLine, bool Hide, std::uint32_t Timeout) noexcept(true)
			{
				STARTUPINFOA si;
				PROCESS_INFORMATION pi;
				memset(&si, 0, sizeof(si)); si.cb = sizeof(si);
				memset(&pi, 0, sizeof(pi));

				si.dwFlags = STARTF_USESHOWWINDOW;
				si.wShowWindow = Hide ? SW_HIDE : SW_SHOW;

				int argc = 0;
				auto ArgsCmdLine = CommandLineToArgvW(StringUtils::WinCPToUtf16(CmdLine).c_str(), &argc);
				if (argc < 1)
					return false;

				auto ParentPath = PathUtils::ExtractFilePath(StringUtils::Utf16ToWinCP(ArgsCmdLine[0]));

				if (!CreateProcessA(
					NULL,
					const_cast<LPSTR>(CmdLine),
					NULL,
					NULL,
					FALSE,
					NORMAL_PRIORITY_CLASS,
					NULL,
					ParentPath.c_str(),
					&si,
					&pi))
					return false;

				WaitForSingleObject(pi.hProcess, Timeout);
				CloseHandle(pi.hProcess);

				return true;
			}

			bool ResponseWindow::GenerationLip(const char* AudioPath, const char* LipPath,
				const char* ResponseText) noexcept(true)
			{
				if (!EnableLipGeneration)
					return false;

				DeleteFileA(LipPath);

				auto LipGenPath = StringUtils::Utf16ToWinCP(PathUtils::GetApplicationPath()) + V_TOOLPATH;
				auto TempAudioPath = PathUtils::ChangeFileExt(AudioPath, "") + "_temp.wav";

				DeleteFileA(TempAudioPath.c_str());

				// Convert audio to 16KHz
				// "\"%s\" -i \"%s\" -ac 1 -acodec pcm_s16le -ar 16000 \"%s\"";
				auto CmdLine = StringUtils::FormatString(V_CMD_FMT2, (LipGenPath + V_FFMPEG).c_str(),
					AudioPath, TempAudioPath.c_str());
				ExecuteApplication(CmdLine.c_str());

				if (!PathUtils::FileExists(TempAudioPath))
					return false;

				// Generate Lip
				CmdLine = StringUtils::FormatString(V_CMD_FMT, (LipGenPath + V_WRAPPER).c_str(),
					TempAudioPath.c_str(), StringUtils::WinCPToUtf8(ResponseText).c_str(), LipPath, 
					V_LANG.c_str(), V_GESTUREEXAGGERATION);
				ExecuteApplication(CmdLine.c_str());

				DeleteFileA(TempAudioPath.c_str());
				DeleteFileA((LipGenPath + "tmp16khz.wav").c_str());

				return true;
			}

			bool ResponseWindow::GenerationFuz(const char* FuzPath, const char* AudioPath,
				const char* LipPath)  noexcept(true)
			{
				if (!EnableFuzGeneration)
					return false;

				DeleteFileA(FuzPath);

				if (!PathUtils::FileExists(AudioPath) || !PathUtils::FileExists(LipPath))
					return false;

				auto LipGenPath = StringUtils::Utf16ToWinCP(PathUtils::GetApplicationPath()) + V_TOOLPATH;
				auto TempAudioPath = PathUtils::ChangeFileExt(AudioPath, "") + "_temp.wav";

				DeleteFileA(TempAudioPath.c_str());

				// Convert audio to HD
				auto CmdLine = StringUtils::FormatString(V_CMD_FMT3, (LipGenPath + V_FFMPEG).c_str(), AudioPath,
					TempAudioPath.c_str());
				ExecuteApplication(CmdLine.c_str());

				auto TempAudioPath2 = PathUtils::ChangeFileExt(AudioPath, ".xwm");
				CmdLine = StringUtils::FormatString(V_CMD_FMT5, (LipGenPath + V_XWMAENCD).c_str(),
					TempAudioPath.c_str(), TempAudioPath2.c_str());
				ExecuteApplication(CmdLine.c_str());

				DeleteFileA(TempAudioPath.c_str());

				if (!PathUtils::FileExists(TempAudioPath2))
					return false;

				// Create Fuz
				CmdLine = StringUtils::FormatString(V_CMD_FMT4, (LipGenPath + V_FUZTOOLS).c_str(), FuzPath,
					LipPath, TempAudioPath2.c_str());
				ExecuteApplication(CmdLine.c_str());

				DeleteFileA(TempAudioPath2.c_str());

				return true;
			}
		}
	}
}