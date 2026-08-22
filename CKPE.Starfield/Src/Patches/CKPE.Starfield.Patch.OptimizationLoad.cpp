// Copyright © 2023-2024 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.PathUtils.h>
#include <CKPE.StringUtils.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Starfield.VersionLists.h>
#include <EditorAPI/BSFile.h>
#include <Patches/CKPE.Starfield.Patch.OptimizationLoad.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
			namespace INI
			{
				static std::string app_path;
				static std::wstring app_pathw;

				static UINT _hook_GetPrivateProfileIntA(LPCSTR lpAppName, LPCSTR lpKeyName, INT nDefault, LPCSTR lpFileName) noexcept
				{
					if (PathUtils::IsRelative(lpFileName))
						return GetPrivateProfileIntA(lpAppName, lpKeyName, nDefault,
							(app_path + PathUtils::ExtractFileName(lpFileName)).c_str());
					else
						return GetPrivateProfileIntA(lpAppName, lpKeyName, nDefault, lpFileName);
				}

				static UINT _hook_GetPrivateProfileIntW(LPCWSTR lpAppName, LPCWSTR lpKeyName, INT nDefault, LPCWSTR lpFileName) noexcept
				{
					if (PathUtils::IsRelative(lpFileName))
						return GetPrivateProfileIntW(lpAppName, lpKeyName, nDefault,
							(app_pathw + PathUtils::ExtractFileName(lpFileName)).c_str());
					else
						return GetPrivateProfileIntW(lpAppName, lpKeyName, nDefault, lpFileName);
				}

				static DWORD _hook_GetPrivateProfileStringA(LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault, 
					LPSTR lpReturnedString, DWORD nSize, LPCSTR lpFileName) noexcept
				{
					if (PathUtils::IsRelative(lpFileName))
						return GetPrivateProfileStringA(lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize,
							(app_path + PathUtils::ExtractFileName(lpFileName)).c_str());
					else
						return GetPrivateProfileStringA(lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize, lpFileName);
				}

				static DWORD _hook_GetPrivateProfileStringW(LPCWSTR lpAppName, LPCWSTR lpKeyName, LPCWSTR lpDefault,
					LPWSTR lpReturnedString, DWORD nSize, LPCWSTR lpFileName) noexcept
				{
					if (PathUtils::IsRelative(lpFileName))
						return GetPrivateProfileStringW(lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize,
							(app_pathw + PathUtils::ExtractFileName(lpFileName)).c_str());
					else
						return GetPrivateProfileStringW(lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize, lpFileName);
				}

				static DWORD _hook_GetPrivateProfileStructA(LPCSTR lpszSection, LPCSTR lpszKey, LPVOID lpStruct,
					UINT uSizeStruct, LPCSTR lpFileName) noexcept
				{
					if (PathUtils::IsRelative(lpFileName))
						return GetPrivateProfileStructA(lpszSection, lpszKey, lpStruct, uSizeStruct,
							(app_path + PathUtils::ExtractFileName(lpFileName)).c_str());
					else
						return GetPrivateProfileStructA(lpszSection, lpszKey, lpStruct, uSizeStruct, lpFileName);
				}

				static BOOL _hook_WritePrivateProfileStringA(LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpString, LPCSTR lpFileName) noexcept
				{
					if (PathUtils::IsRelative(lpFileName))
						return WritePrivateProfileStringA(lpAppName, lpKeyName, lpString,
							(app_path + PathUtils::ExtractFileName(lpFileName)).c_str());
					else
						return WritePrivateProfileStringA(lpAppName, lpKeyName, lpString, lpFileName);
				}

				static BOOL _hook_WritePrivateProfileStringW(LPCWSTR lpAppName, LPCWSTR lpKeyName, LPCWSTR lpString, LPCWSTR lpFileName) noexcept
				{
					if (PathUtils::IsRelative(lpFileName))
						return WritePrivateProfileStringW(lpAppName, lpKeyName, lpString, (app_pathw + PathUtils::ExtractFileName(lpFileName)).c_str());
					else
						return WritePrivateProfileStringW(lpAppName, lpKeyName, lpString, lpFileName);
				}

				static BOOL _hook_WritePrivateProfileStructA(LPCSTR lpszSection, LPCSTR lpszKey, LPVOID lpStruct,
					UINT uSizeStruct, LPCSTR lpFileName) noexcept
				{
					if (PathUtils::IsRelative(lpFileName))
						return WritePrivateProfileStructA(lpszSection, lpszKey, lpStruct, uSizeStruct,
							(app_path + PathUtils::ExtractFileName(lpFileName)).c_str());
					else
						return WritePrivateProfileStructA(lpszSection, lpszKey, lpStruct, uSizeStruct, lpFileName);
				}
			}

			namespace IO
			{
				static HANDLE _hook_CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
					DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) noexcept
				{
					if (!(dwFlagsAndAttributes & FILE_FLAG_WRITE_THROUGH))
					{
						if (dwFlagsAndAttributes & FILE_FLAG_OVERLAPPED)
							dwFlagsAndAttributes |= FILE_FLAG_RANDOM_ACCESS;

						if (dwFlagsAndAttributes & FILE_FLAG_NO_BUFFERING)
							dwFlagsAndAttributes &= ~FILE_FLAG_NO_BUFFERING;
					}

					return CreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition,
						dwFlagsAndAttributes, hTemplateFile);
				}

				static HANDLE _hook_CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
					DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) noexcept
				{
					if (!(dwFlagsAndAttributes & FILE_FLAG_WRITE_THROUGH))
					{
						if (dwFlagsAndAttributes & FILE_FLAG_OVERLAPPED)
							dwFlagsAndAttributes |= FILE_FLAG_RANDOM_ACCESS;

						if (dwFlagsAndAttributes & FILE_FLAG_NO_BUFFERING)
							dwFlagsAndAttributes &= ~FILE_FLAG_NO_BUFFERING;
					}

					return CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition,
						dwFlagsAndAttributes, hTemplateFile);
				}
			}

			static HANDLE HKFindFirstFileA(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData) noexcept(true)
			{
				return FindFirstFileExA(lpFileName, FindExInfoStandard, lpFindFileData, FindExSearchNameMatch,
					nullptr, FIND_FIRST_EX_LARGE_FETCH);
			}

			OptimizationLoad::OptimizationLoad() : Common::Patch()
			{
				SetName("Optimization load");
			}

			bool OptimizationLoad::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* OptimizationLoad::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool OptimizationLoad::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> OptimizationLoad::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool OptimizationLoad::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() >= VersionLists::EDITOR_STARFIELD_1_14_70_0;
			}

			bool OptimizationLoad::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Plugin loading optimizations:
				//
				// - Replacing FindFirstNextA with a more optimized function FindFirstFileExA					
				// - Skip remove failed forms (This function is more likely to result in CTD) 
				// - Eliminate millions of calls to update the statusbar
				// - Increasing the read memory buffer to reduce disk access
				// - Reducing spin time. Important: materials are loaded in the background.
				// - Fixed bugs this version CK, relative path for this functions - errors.
				// - IO enable random access.

				Detours::DetourIAT(base, "kernel32.dll", "FindFirstFileA", (std::uintptr_t)&HKFindFirstFileA);

				{
					// Cutting a lot is faster this way
					auto stext = interface->GetApplication()->GetSegment(Segment::text);
					ScopeSafeWrite text(stext.GetAddress(), stext.GetSize());

					// If there is an ZLib error, will try to leave the function 
					text.Write(__CKPE_OFFSET(2), { 0xE9, 0x41, 0x01, 0x00, 0x00, 0x90 });
					// Skip remove failed forms
					text.Write(__CKPE_OFFSET(3), { 0xEB });
					// Spam in the status bar
					text.Write(__CKPE_OFFSET(4), { 0xC3, 0x90 });
					// Spam Primitive with zero extents encountered for reference %08X
					text.Write(__CKPE_OFFSET(6), { 0xEB });
					// Spam Demanding CPU Geometry Data for '%s' because it wasn't ready on time
					text.WriteNop(__CKPE_OFFSET(7), 5);
					// Increasing the memory buffer to read the file
					auto rva = (std::uintptr_t)__CKPE_OFFSET(8);
					text.Write(rva, { 0x04 });
					text.Write(rva + 8, { 0x86 });
					// full skip "%s: Imported file missing: %s"
					text.Write(__CKPE_OFFSET(10), { 0xEB });

					INI::app_pathw = PathUtils::GetApplicationPath();
					INI::app_path = StringUtils::Utf16ToWinCP(INI::app_pathw);

					// Fixed bugs this version CK, relative path for this functions - errors
					Detours::DetourIAT(base, "kernel32.dll", "GetPrivateProfileIntA", (uintptr_t)&INI::_hook_GetPrivateProfileIntA);
					Detours::DetourIAT(base, "kernel32.dll", "GetPrivateProfileIntW", (uintptr_t)&INI::_hook_GetPrivateProfileIntW);
					Detours::DetourIAT(base, "kernel32.dll", "GetPrivateProfileStringA", (uintptr_t)&INI::_hook_GetPrivateProfileStringA);
					Detours::DetourIAT(base, "kernel32.dll", "GetPrivateProfileStringW", (uintptr_t)&INI::_hook_GetPrivateProfileStringW);
					Detours::DetourIAT(base, "kernel32.dll", "GetPrivateProfileStructA", (uintptr_t)&INI::_hook_GetPrivateProfileStructA);
					Detours::DetourIAT(base, "kernel32.dll", "WritePrivateProfileStringA", (uintptr_t)&INI::_hook_WritePrivateProfileStringA);
					Detours::DetourIAT(base, "kernel32.dll", "WritePrivateProfileStringW", (uintptr_t)&INI::_hook_WritePrivateProfileStringW);
					Detours::DetourIAT(base, "kernel32.dll", "WritePrivateProfileStructA", (uintptr_t)&INI::_hook_WritePrivateProfileStructA);
					
					// IO random access
					Detours::DetourIAT(base, "kernel32.dll", "CreateFileA", (uintptr_t)&IO::_hook_CreateFileA);
					Detours::DetourIAT(base, "kernel32.dll", "CreateFileW", (uintptr_t)&IO::_hook_CreateFileW);
				}

				// Reducing spin time.
				// While need to wait, will occupy thread with something useful, for example, message processing
				//Detours::DetourCall(__CKPE_OFFSET(9), (std::uintptr_t)&ProcessMessage);
				//Detours::DetourCall(__CKPE_OFFSET(11), (std::uintptr_t)&ProcessMessage);
				//Detours::DetourCall(__CKPE_OFFSET(12), (std::uintptr_t)&ProcessMessage);
				//Detours::DetourCall(__CKPE_OFFSET(13), (std::uintptr_t)&ProcessMessage);
				//Detours::DetourCall(__CKPE_OFFSET(14), (std::uintptr_t)&ProcessMessage);

				// 2 kb -> x kb >= 256 kb
				*(std::uintptr_t*)&EditorAPI::BSFile::ICreateInstance =
					Detours::DetourClassJump(__CKPE_OFFSET(5), (std::uintptr_t)&EditorAPI::BSFile::HKCreateInstance);

				return true;
			}

			void OptimizationLoad::ProcessMessage() noexcept(true)
			{
				Sleep(0);
				Common::Interface::GetSingleton()->GetApplication()->MessageProcessing();
			}
		}
	}
}