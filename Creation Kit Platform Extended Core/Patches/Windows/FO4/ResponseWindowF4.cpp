// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "ResponseWindowF4.h"
#include "Editor API/EditorUI.h"
#include "Editor API/BSString.h"
#include "Editor API/FO4/TESTopic.h"
#include "Editor API/FO4/BGSVoiceType.h"
#include "Patches/ConsolePatch.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			using namespace EditorAPI;
			using namespace EditorAPI::Fallout4;

			ResponseWindow* GlobalResponseWindowPtr = nullptr;
			bool EnableLipGeneration = false;
			bool EnableFuzGeneration = false;

			uintptr_t pointer_ResponseWindow_data = 0;
			uintptr_t pointer_ResponseWindow_sub1 = 0;
			uintptr_t pointer_ResponseWindow_sub2 = 0;
			uintptr_t pointer_ResponseWindow_sub3 = 0;

			static const char* V_TOOLPATH = "Tools\\LipGen\\";
			static const char* V_WRAPPER = "FaceFXWrapper.exe";
			static const char* V_XWMAENCD = "xWMAEncode.exe";
			static const char* V_FUZTOOLS = "fuz_extractor.exe";
			static const char* V_FFMPEG = "ffmpeg.exe";
			static const char* V_FXDATA = "FonixData.cdf";
			static const char* V_CMD_FMT = "\"%s\" Fallout4 USEnglish FonixData.cdf \"%s\" \"%s\" \"%s\"";
			static const char* V_CMD_FMT2 = "\"%s\" -i \"%s\" -ac 1 -acodec pcm_s16le -ar 16000 \"%s\"";
			static const char* V_CMD_FMT3 = "\"%s\" -i \"%s\" -ac 2 -acodec pcm_s16le -ab 192000 -ar 44100 \"%s\"";
			static const char* V_CMD_FMT4 = "\"%s\" -c \"%s\" \"%s\" \"%s\"";
			static const char* V_CMD_FMT5 = "\"%s\" -b 192000 \"%s\" \"%s\"";

			typedef struct RESPONSE_DATA
			{
				const char* Title;
				uint64_t UnkParm;
				TESTopicInfo::Response* ResponseSource;
				TESTopicInfo::Response* ResponseTemp;
				TESTopic* Topic;
				TESTopicInfo* TopicInfo;
				struct AUDIO
				{
					BGSVoiceType* VoiceType;
					char pad08[0x4];
					char AudioFileName[MAX_PATH];
				} Audio;
			} *LPRESPONSE_DATA;

			bool ResponseWindow::HasOption() const
			{
				return false;
			}

			bool ResponseWindow::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* ResponseWindow::GetOptionName() const
			{
				return nullptr;
			}

			const char* ResponseWindow::GetName() const
			{
				return "Response Window";
			}

			bool ResponseWindow::HasDependencies() const
			{
				return true;
			}

			Array<String> ResponseWindow::GetDependencies() const
			{
				return { "Console" };
			}

			bool ResponseWindow::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_FALLOUT_C4_LAST;
			}

			bool ResponseWindow::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					*(uintptr_t*)&_oldWndProc =
						Detours::X64::DetourFunctionClass(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0)),
							(uintptr_t)&HKWndProc);

					pointer_ResponseWindow_data = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));
					pointer_ResponseWindow_sub1 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(2));
					pointer_ResponseWindow_sub2 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(3));
					pointer_ResponseWindow_sub3 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(4));

					// Hooking the jump. I don't allow the deny button.
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(5), (uintptr_t)&sub);
					// Then continue in the same spirit, remove the button.... skip it
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(6), 25);
					// In case of cancellation .wav triggers and closes the button, we will remove everything
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(7), (uintptr_t)&sub);
					
					return true;
				}

				return false;
			}

			bool ResponseWindow::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			ResponseWindow::ResponseWindow() : BaseWindow(), Classes::CUIBaseWindow()
			{
				Assert(!GlobalResponseWindowPtr);
				GlobalResponseWindowPtr = this;
			}

			LRESULT CALLBACK ResponseWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				if (Message == WM_INITDIALOG)
				{
					GlobalResponseWindowPtr->m_hWnd = Hwnd;
					GlobalResponseWindowPtr->ListViewItems = GetDlgItem(Hwnd, 2168);

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

					auto LipGenPath = BSString::Utils::GetApplicationPath() + V_TOOLPATH;

					EnableLipGeneration = 
						Utils::FileExists((LipGenPath + V_WRAPPER).c_str()) &&
						Utils::FileExists((LipGenPath + V_FXDATA).c_str());

					EnableFuzGeneration =
						Utils::FileExists((LipGenPath + V_XWMAENCD).c_str()) &&
						Utils::FileExists((LipGenPath + V_FUZTOOLS).c_str()) &&
						Utils::FileExists((LipGenPath + V_FFMPEG).c_str());

					if (!EnableLipGeneration)
						ConsolePatch::Log("LIPGEN: \"%s\", \"%s\", \"%s\" is missing from \"<GAMEDIR>%s\" directory."
							" LIP generation will be disabled.", V_WRAPPER, V_FXDATA, V_FFMPEG, LipGenPath);

					if (!EnableFuzGeneration)
						ConsolePatch::Log("FUZGEN: \"%s\", \"%s\" is missing from \"<GAMEDIR>%s\" directory."
							" FUZ generation will be disabled.", V_XWMAENCD, V_FUZTOOLS, LipGenPath);
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
								EditorUI::ListViewGetSelectedItem(GlobalResponseWindowPtr->ListViewItems.Handle);

							// "Generate Lip File"
							if (param == 1016)
							{
								if (!EnableLipGeneration || !item)
									return S_FALSE;

								BSString AudioFilePath = BSString::Utils::GetApplicationPath() + item->AudioFileName;

								// only .wav
								AudioFilePath = BSString::Utils::ChangeFileExt(AudioFilePath, ".wav");
								if (!Utils::FileExists(AudioFilePath.c_str()))
								{
									ConsolePatch::Log("LIPGEN: File \"%s\" no found. Trying WAV extension fallback.",
										AudioFilePath.c_str());
									MessageBoxA(Hwnd, "Unable to find audio file on disk", "Error", MB_ICONERROR);

									return S_FALSE;
								}

								BSString LipFilePath = BSString::Utils::ChangeFileExt(AudioFilePath, ".lip");

								auto data = *(LPRESPONSE_DATA*)pointer_ResponseWindow_data;

								// It became unnecessary, and so i know everything, thanks RE
								// auto topic = fastCall<int64_t>(pointer_ResponseWindow_sub1, *(int64_t*)(data + 0x28), 
								//	 *(uint8_t*)(*(int64_t*)(data + 0x18) + 0x1A));
								// BSString InputText = fastCall<const char*>(pointer_ResponseWindow_sub2, topic);

								// Abandoning CreationKit32
								// fastCall<bool>(pointer_ResponseWindow_sub3, Hwnd, AudioFilePath.c_str(), InputText.c_str());

								if (!GenerationLip(AudioFilePath.c_str(), LipFilePath.c_str(), data->ResponseTemp->ResponseText.Get()) ||
									!Utils::FileExists(LipFilePath.c_str()))
								{
									ConsolePatch::Log("LIPGEN: File \"%s\" LIP generation failed.", LipFilePath.c_str());
									return S_FALSE;
								}
							}
							// "Generate Lip File"
							else if (param == 1017)
							{
								if (!EnableFuzGeneration || !item)
									return S_FALSE;

								BSString AudioFilePath = BSString::Utils::GetApplicationPath() + item->AudioFileName;

								// only .wav
								AudioFilePath = BSString::Utils::ChangeFileExt(AudioFilePath, ".wav");
								if (!Utils::FileExists(AudioFilePath.c_str()))
								{
									ConsolePatch::Log("FUZGEN: File \"%s\" no found. Trying WAV extension fallback.",
										AudioFilePath.c_str());
									MessageBoxA(Hwnd, "Unable to find audio file on disk", "Error", MB_ICONERROR);

									return S_FALSE;
								}

								BSString LipFilePath = BSString::Utils::ChangeFileExt(AudioFilePath, ".lip");
								if (!Utils::FileExists(LipFilePath.c_str()))
								{
									ConsolePatch::Log("FUZGEN: File \"%s\" no found. Trying LIP extension fallback.",
										LipFilePath.c_str());
									MessageBoxA(Hwnd, "Unable to find lip file on disk", "Error", MB_ICONERROR);

									return S_FALSE;
								}

								BSString FuzFilePath = BSString::Utils::ChangeFileExt(AudioFilePath, ".fuz");
								if (!GenerationFuz(FuzFilePath.c_str(), AudioFilePath.c_str(), LipFilePath.c_str()) ||
									!Utils::FileExists(LipFilePath.c_str()))
								{
									ConsolePatch::Log("FUZGEN: File \"%s\" FUZ generation failed.", FuzFilePath.c_str());
									return S_FALSE;
								}
							}
						}
					}
				}
				else if (Message == WM_NOTIFY)
				{
					auto notify = (LPNMHDR)lParam;

					if (notify->code == LVN_ITEMCHANGED && notify->idFrom == 2168)
					{
						EditorAPI::EditorUI::HKSendMessageA(Hwnd, WM_COMMAND, 2379, 0);
						EnableWindow(GetDlgItem(Hwnd, 1017), ListView_GetSelectedCount(notify->hwndFrom) > 0);
					}
				}

				return CallWindowProc(GlobalResponseWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}

			void ResponseWindow::sub(HWND hWndButtonGenerate)
			{
				HWND hDlg = GetParent(hWndButtonGenerate);
				HWND hList = GetDlgItem(hDlg, 0x878);
				HWND hLTFCheck = GetDlgItem(hDlg, 0x94C);

				// no support .ltf
				EnableWindow(hLTFCheck, FALSE);
				// wav check default
				CheckDlgButton(hDlg, 0x94B, BST_CHECKED);

				bool bEnableGenerate = false;

				if (int iCount = ListView_GetItemCount(hList); iCount > 0) {
					if (auto item = (int64_t)EditorUI::ListViewGetSelectedItem(hList); item) {
						bEnableGenerate =
							IsDlgButtonChecked(hDlg, 2379) && *(uint32_t*)(item + 0x110) ||
							IsDlgButtonChecked(hDlg, 2380) && *(uint32_t*)(item + 0x118);
					}
				}

				EnableWindow(hWndButtonGenerate, bEnableGenerate);
			}

			bool ResponseWindow::ExecuteApplication(const char* CmdLine, bool Hide, uint32_t Timeout)
			{
				STARTUPINFO si;
				PROCESS_INFORMATION pi;
				memset(&si, 0, sizeof(si)); si.cb = sizeof(si);
				memset(&pi, 0, sizeof(pi));

				si.dwFlags = STARTF_USESHOWWINDOW;
				si.wShowWindow = Hide ? SW_HIDE : SW_SHOW;

				int argc = 0;
				auto ArgsCmdLine = CommandLineToArgvW(Utils::Utf82Wide((const char8_t*)CmdLine).c_str(), &argc);
				if (argc < 1)
					return false;

				auto ParentPath = BSString::Utils::ExtractFilePath(Utils::Wide2Utf8(ArgsCmdLine[0]).c_str());

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

			bool ResponseWindow::GenerationLip(const char* AudioPath, const char* LipPath, const char* ResponseText)
			{
				if (!EnableLipGeneration)
					return false;

				DeleteFileA(LipPath);
				
				auto LipGenPath = BSString::Utils::GetApplicationPath() + V_TOOLPATH;
				auto TempAudioPath = BSString::Utils::ChangeFileExt(AudioPath, "") + "_temp.wav";

				DeleteFileA(TempAudioPath.c_str());

				// Convert audio to 16KHz
				auto CmdLine = BSString::FormatString(V_CMD_FMT2, (LipGenPath + V_FFMPEG).c_str(), AudioPath, TempAudioPath.c_str());
				ExecuteApplication(CmdLine.c_str());

				if (!BSString::Utils::FileExists(TempAudioPath))
					return false;

				// Generate Lip
				CmdLine = BSString::FormatString(V_CMD_FMT, (LipGenPath + V_WRAPPER).c_str(), TempAudioPath.c_str(), LipPath, ResponseText);
				ExecuteApplication(CmdLine.c_str());
				
				DeleteFileA(TempAudioPath.c_str());

				return true;
			}

			bool ResponseWindow::GenerationFuz(const char* FuzPath, const char* AudioPath, const char* LipPath)
			{
				if (!EnableFuzGeneration)
					return false;

				DeleteFileA(FuzPath);

				if (!BSString::Utils::FileExists(AudioPath) || !BSString::Utils::FileExists(LipPath))
					return false;

				auto LipGenPath = BSString::Utils::GetApplicationPath() + V_TOOLPATH;
				auto TempAudioPath = BSString::Utils::ChangeFileExt(AudioPath, "") + "_temp.wav";

				DeleteFileA(TempAudioPath.c_str());

				// Convert audio to HD
				auto CmdLine = BSString::FormatString(V_CMD_FMT3, (LipGenPath + V_FFMPEG).c_str(), AudioPath,
					TempAudioPath.c_str());
				ExecuteApplication(CmdLine.c_str());

				auto TempAudioPath2 = BSString::Utils::ChangeFileExt(AudioPath, ".xwm");
				CmdLine = BSString::FormatString(V_CMD_FMT5, (LipGenPath + V_XWMAENCD).c_str(), 
					TempAudioPath.c_str(), TempAudioPath2.c_str());
				ExecuteApplication(CmdLine.c_str());
				
				DeleteFileA(TempAudioPath.c_str());

				if (!BSString::Utils::FileExists(TempAudioPath2))
					return false;

				// Create Fuz
				CmdLine = BSString::FormatString(V_CMD_FMT4, (LipGenPath + V_FUZTOOLS).c_str(), FuzPath,
					LipPath, TempAudioPath2.c_str());
				ExecuteApplication(CmdLine.c_str());

				DeleteFileA(TempAudioPath2.c_str());

				return true;
			}
		}
	}
}