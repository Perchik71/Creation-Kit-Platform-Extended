// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/BSString.h"
#include "Patches/ConsolePatch.h"
#include "FaceGen.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		uintptr_t pointer_FaceGen_sub1 = 0;
		uintptr_t pointer_FaceGen_sub2 = 0;
		uintptr_t pointer_FaceGen_sub3 = 0;
		uintptr_t pointer_FaceGen_sub4 = 0;
		uintptr_t pointer_FaceGen_sub5 = 0;
		uintptr_t pointer_FaceGen_data = 0;

		EditorAPI::BSString Texconv;
		ConcurrencyArray<std::pair<String, String>> FacegenTextures;

		FaceGenPatch::FaceGenPatch() : Module(GlobalEnginePtr)
		{}

		bool FaceGenPatch::HasOption() const
		{
			return false;
		}

		bool FaceGenPatch::HasCanRuntimeDisabled() const
		{
			return false;
		}

		const char* FaceGenPatch::GetOptionName() const
		{
			return nullptr;
		}

		const char* FaceGenPatch::GetName() const
		{
			return "FaceGen";
		}

		bool FaceGenPatch::HasDependencies() const
		{
			return true;
		}

		Array<String> FaceGenPatch::GetDependencies() const
		{
			return { "Console" };
		}

		bool FaceGenPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			return true;
		}

		bool FaceGenPatch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			auto verPatch = lpRelocationDatabaseItem->Version();
			if (verPatch == 1)
			{
				{
					ScopeRelocator text; // fast patches

					//
					// FaceGen
					//
					// Disable automatic FaceGen on save
					if (_READ_OPTION_BOOL("FaceGen", "bDisableAutoFaceGen", false))
					{
						lpRelocator->Patch(_RELDATA_RAV(0), { 0xC3 });
						_MESSAGE("Disabling automatic calling FaceGen");
					}

					// Don't produce DDS files
					if (_READ_OPTION_BOOL("FaceGen", "bDisableExportDDS", false))
					{
						lpRelocator->PatchNop(_RELDATA_RAV(1), 5);
						_MESSAGE("Disabling export FaceGen .DDS files");
					}

					// Don't produce TGA files
					if (_READ_OPTION_BOOL("FaceGen", "bDisableExportTGA", false))
					{
						lpRelocator->PatchNop(_RELDATA_RAV(2), 5);
						_MESSAGE("Disabling export FaceGen .TGA files");
					}

					// Don't produce NIF files
					if (_READ_OPTION_BOOL("FaceGen", "bDisableExportNIF", false))
					{
						lpRelocator->Patch(_RELDATA_RAV(3), { 0xC3 });
						_MESSAGE("Disabling export FaceGen .NIF files");
					}

					// Allow variable tint mask resolution
					uint32_t tintResolution = _READ_OPTION_UINT("FaceGen", "uTintMaskResolution", 512);
					lpRelocator->Patch(_RELDATA_RAV(4), (uint8_t*)&tintResolution, sizeof(uint32_t));
					lpRelocator->Patch(_RELDATA_RAV(5), (uint8_t*)&tintResolution, sizeof(uint32_t));
				}

				// Prevent internal filesystem reloads when exporting FaceGen for many NPCs
				lpRelocator->DetourJump(_RELDATA_RAV(6), (uintptr_t)&sub);
				lpRelocator->PatchNop(_RELDATA_RAV(7), 5);

				pointer_FaceGen_data = _RELDATA_ADDR(8);
				pointer_FaceGen_sub1 = _RELDATA_ADDR(9);
				pointer_FaceGen_sub2 = _RELDATA_ADDR(10);
				pointer_FaceGen_sub3 = _RELDATA_ADDR(11);
				pointer_FaceGen_sub4 = _RELDATA_ADDR(12);
				pointer_FaceGen_sub5 = _RELDATA_ADDR(13);
				
				return true;
			}
			else if (verPatch == 2)
			{
				pointer_FaceGen_sub1 = _RELDATA_ADDR(16);
				pointer_FaceGen_sub2 = _RELDATA_ADDR(19);

				ScopeRelocator text; // fast patches

				// Don't produce DDS files
				if (_READ_OPTION_BOOL("FaceGen", "bDisableExportDDS", false)) {
					lpRelocator->PatchNop(_RELDATA_RAV(0), 5);
					lpRelocator->PatchNop(_RELDATA_RAV(1), 5);
					lpRelocator->PatchNop(_RELDATA_RAV(2), 5);
					lpRelocator->PatchNop(_RELDATA_RAV(3), 5);
					lpRelocator->PatchNop(_RELDATA_RAV(4), 5);
					lpRelocator->PatchNop(_RELDATA_RAV(5), 5);
				}
				else
				{
					lpRelocator->DetourCall(_RELDATA_RAV(1), (uintptr_t)&Fallout4::CreateDiffuseCompressDDS);
					lpRelocator->DetourCall(_RELDATA_RAV(3), (uintptr_t)&Fallout4::CreateNormalsCompressDDS);
					lpRelocator->DetourCall(_RELDATA_RAV(5), (uintptr_t)&Fallout4::CreateSpecularCompressDDS);

					Texconv = EditorAPI::BSString::Utils::GetApplicationPath() + "Tools\\Elric\\texconv.exe";
					lpRelocator->DetourCall(_RELDATA_RAV(17), (uintptr_t)&Fallout4::ExecuteGUI);
					lpRelocator->DetourCall(_RELDATA_RAV(18), (uintptr_t)&Fallout4::ExecuteCLI);
				}

				// Don't produce TGA files
				if (_READ_OPTION_BOOL("FaceGen", "bDisableExportTGA", false)) {
					lpRelocator->PatchNop(_RELDATA_RAV(6), 5);
					lpRelocator->PatchNop(_RELDATA_RAV(7), 5);
					lpRelocator->PatchNop(_RELDATA_RAV(8), 5);
					lpRelocator->PatchNop(_RELDATA_RAV(9), 5);
					lpRelocator->PatchNop(_RELDATA_RAV(10), 5);
					lpRelocator->PatchNop(_RELDATA_RAV(11), 5);
				}

				// Don't produce NIF files
				if (_READ_OPTION_BOOL("FaceGen", "bDisableExportNIF", false))
					lpRelocator->Patch(_RELDATA_RAV(12), { 0xC3 });

				// Allow variable tint mask resolution
				uint32_t tintResolution = _READ_OPTION_UINT("FaceGen", "uTintMaskResolution", 1024);
				lpRelocator->Patch(_RELDATA_RAV(13), (uint8_t*)&tintResolution, sizeof(uint32_t));
				lpRelocator->Patch(_RELDATA_RAV(14), (uint8_t*)&tintResolution, sizeof(uint32_t));

				// remove check format
				lpRelocator->PatchNop(_RELDATA_RAV(15), 6);

				return true;
			}
			else if (verPatch == 3)
			{
				ScopeRelocator text; // fast patches

				auto Rva1 = (uintptr_t)_RELDATA_RAV(0);
				// Remove useless stuff	
				lpRelocator->PatchNop(Rva1, 0xC4);
				// Set args for function and call
				lpRelocator->Patch(Rva1, { 0x4C, 0x89, 0xF1, 0x48, 0x8D, 0x95, 0xC0, 0x07, 0x00, 0x00 });
				lpRelocator->DetourCall(Rva1 + 10, (uintptr_t)&sub_sf);
			}

			return false;
		}

		bool FaceGenPatch::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			return false;
		}

		void FaceGenPatch::Fallout4::CreateDiffuseCompressDDS(__int64 lpThis, uint32_t TextureId, const char* lpFileName,
			int32_t Unk1, bool Unk2)
		{
			fastCall<void>(pointer_FaceGen_sub1, lpThis, TextureId, lpFileName, Unk1, Unk2);
			// Compress BC7
			FacegenTextures.push_back(std::make_pair(lpFileName, "BC7_UNORM"));
		}

		void FaceGenPatch::Fallout4::CreateNormalsCompressDDS(__int64 lpThis, uint32_t TextureId, const char* lpFileName,
			int32_t Unk1, bool Unk2)
		{
			fastCall<void>(pointer_FaceGen_sub1, lpThis, TextureId, lpFileName, Unk1, Unk2);
			// Compress BC5
			FacegenTextures.push_back(std::make_pair(lpFileName, "BC5_UNORM"));
		}

		void FaceGenPatch::Fallout4::CreateSpecularCompressDDS(__int64 lpThis, uint32_t TextureId, const char* lpFileName,
			int32_t Unk1, bool Unk2)
		{
			fastCall<void>(pointer_FaceGen_sub1, lpThis, TextureId, lpFileName, Unk1, Unk2);
			// Compress BC5
			FacegenTextures.push_back(std::make_pair(lpFileName, "BC5_UNORM"));
		}

		void FaceGenPatch::Fallout4::Execute(bool bShowDone)
		{
			_CONSOLE("FACEGEN: Total textures %llu", FacegenTextures.size());
			
			{
				EditorAPI::BSString FileName, NewFileName, Path, Command;

				for (auto it = FacegenTextures.begin(); it != FacegenTextures.end(); it++)
				{
					FileName = EditorAPI::BSString::Utils::GetApplicationPath() + it->first.c_str();
					if (EditorAPI::BSString::Utils::FileExists(FileName))
					{
						Path = EditorAPI::BSString::Utils::ExtractFilePath(FileName) + "compress";
						SHCreateDirectoryExA(NULL, Path.c_str(), NULL);

						Command.Format("\"%s\" -m 1 -f %s \"%s\" -o \"%s\"", Texconv.c_str(), it->second.c_str(),
							FileName.c_str(), Path.c_str());

						STARTUPINFO info;
						PROCESS_INFORMATION processInfo;
						ZeroMemory(&info, sizeof(STARTUPINFO));
						ZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));
						
						info.cb = sizeof(STARTUPINFO);
						info.wShowWindow = SW_HIDE;
						info.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
						info.hStdInput = NULL;
						info.hStdError = NULL;
						info.hStdOutput = NULL;

						if (CreateProcessA(NULL, const_cast<LPSTR>(Command.c_str()), NULL, NULL, TRUE,
							NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW | DETACHED_PROCESS, NULL, NULL, &info, &processInfo))
						{
							bool Success = false;

							if (WAIT_OBJECT_0 != WaitForSingleObject(processInfo.hProcess, 20000))
							{
								TerminateProcess(processInfo.hProcess, 0);
								_CONSOLE("FACEGEN: Texture could not be compressed \"%s\" (Timeout)", FileName.c_str());
							}
							else
							{
								DWORD rc;
								GetExitCodeProcess(processInfo.hProcess, &rc);
								Success = rc == 0;
							}

							CloseHandle(processInfo.hThread);
							CloseHandle(processInfo.hProcess);

							if (Success)
							{
								NewFileName = Path + "\\" + EditorAPI::BSString::Utils::ExtractFileName(it->first.c_str());
								MoveFileExA(NewFileName.c_str(), FileName.c_str(), MOVEFILE_REPLACE_EXISTING);
							}
							else
								_CONSOLE("FACEGEN: Texture could not be compressed \"%s\"", it->first.c_str());
						}
						else
							_CONSOLE("FACEGEN: Couldn't run \"Tools\\Elric\\texconv.exe\"");

						RemoveDirectoryA(Path.c_str());
					}
					else
						_CONSOLE("FACEGEN: The file \"%s\" was not found", it->first.c_str());
				}
			}

			FacegenTextures.clear();

			_CONSOLE("FACEGEN: Done.", FacegenTextures.size());
			if (bShowDone) MessageBoxA(0, "Done.", "Message", MB_OK | MB_ICONINFORMATION);
		}

		void FaceGenPatch::Fallout4::ExecuteGUI()
		{
			Execute(true);
		}

		void FaceGenPatch::Fallout4::ExecuteCLI(__int64 Arg1, __int64 Arg2, __int64 Arg3, __int64 Arg4)
		{
			fastCall<void>(pointer_FaceGen_sub2, Arg1, Arg2, Arg3, Arg4);
			Execute(false);
		}

		void FaceGenPatch::sub(__int64 a1, __int64 a2)
		{
			auto sub_1418F5210 = (bool(*)())pointer_FaceGen_sub1;
			auto sub_1418F5320 = (void(*)())pointer_FaceGen_sub2;
			auto sub_1413BAAC0 = (__int64(*)(HWND, __int64))pointer_FaceGen_sub3;
			auto sub_1418F5260 = (bool(*)(__int64, __int64))pointer_FaceGen_sub4;
			auto sub_141617680 = (void(*)(__int64))pointer_FaceGen_sub5;

			// Display confirmation message box first
			if (!sub_1418F5210())
				return;

			HWND listHandle = *(HWND*)(a1 + 16);
			int itemIndex = ListView_GetNextItem(listHandle, -1, LVNI_SELECTED);
			int itemCount = 0;

			for (bool flag = true; itemIndex >= 0 && flag; itemCount++)
			{
				flag = sub_1418F5260(a2, sub_1413BAAC0(listHandle, itemIndex));

				if (flag)
				{
					int oldIndex = itemIndex;
					itemIndex = ListView_GetNextItem(listHandle, itemIndex, LVNI_SELECTED);

					if (itemIndex == oldIndex)
						itemIndex = -1;
				}
			}

			// Reload loose file paths manually since it's patched out
			ConsolePatch::Log("Exported FaceGen for %d NPCs. Reloading loose file paths...", itemCount);
			sub_141617680(*(__int64*)pointer_FaceGen_data);

			sub_1418F5320();
		}

		void FaceGenPatch::sub_sf(__int64 a1, __int64 a2)
		{
			HWND listHandle = *(HWND*)(a1 + 0x3168);
			int itemIndex = ListView_GetNextItem(listHandle, -1, LVNI_SELECTED);
			int itemCount = 0;
			
			for (bool flag = true; itemIndex >= 0 && flag; itemCount++)
			{
				// get form
				LVITEM item;
				memset(&item, 0, sizeof(LVITEM));
				item.mask = LVIF_PARAM;
				item.iItem = itemIndex;
				ListView_GetItem(listHandle, &item);
				
				// dev
				_CONSOLE("%p", item.lParam);
				flag = true;

				if (flag)
				{
					int oldIndex = itemIndex;
					itemIndex = ListView_GetNextItem(listHandle, itemIndex, LVNI_SELECTED);

					if (itemIndex == oldIndex)
						itemIndex = -1;
				}
			}

			// Reload loose file paths manually since it's patched out
			ConsolePatch::Log("Exported FaceGen for %d NPCs. Reloading loose file paths...", itemCount);
		}
	}
}