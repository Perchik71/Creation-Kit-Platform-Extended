// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Core/Module.h"
#include "Core/Relocator.h"
#include "Core/RelocationDatabase.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		using namespace CreationKitPlatformExtended::Core;

		class INICacheDataPatch : public Module
		{
		public:
			INICacheDataPatch();
			~INICacheDataPatch();

			virtual bool HasOption() const;
			virtual bool HasCanRuntimeDisabled() const;
			virtual const char* GetOptionName() const;
			virtual const char* GetName() const;
			virtual bool HasDependencies() const;
			virtual Array<String> GetDependencies() const;

			void ClearAndFlush();

			static UINT HKGetPrivateProfileIntA(LPCSTR lpAppName, LPCSTR lpKeyName, INT nDefault, LPCSTR lpFileName);
			static DWORD HKGetPrivateProfileStringA(LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault,
				LPSTR lpReturnedString, DWORD nSize, LPCSTR lpFileName);
			static BOOL HKGetPrivateProfileStructA(LPCSTR lpszSection, LPCSTR lpszKey, LPVOID lpStruct,
				UINT uSizeStruct, LPCSTR szFile);
			static BOOL HKWritePrivateProfileStringA(LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpString, LPCSTR lpFileName);
			static BOOL HKWritePrivateProfileStructA(LPCSTR lpszSection, LPCSTR lpszKey, LPVOID lpStruct,
				UINT uSizeStruct, LPCSTR szFile);

			static UINT HKGetPrivateProfileIntW(LPCWSTR lpAppName, LPCWSTR lpKeyName, INT nDefault, LPCWSTR lpFileName);
			static DWORD HKGetPrivateProfileStringW(LPCWSTR lpAppName, LPCWSTR lpKeyName, LPCWSTR lpDefault,
				LPWSTR lpReturnedString, DWORD nSize, LPCWSTR lpFileName);
			static BOOL HKWritePrivateProfileStringW(LPCWSTR lpAppName, LPCWSTR lpKeyName, LPCWSTR lpString, LPCWSTR lpFileName);

			static std::string GetAbsoluteFileName(LPCSTR lpFileName);
			static std::wstring GetAbsoluteFileNameUnicode(LPCWSTR lpFileName);
			static HANDLE GetFileFromCacheOrOpen(const std::string& sFileName);
		protected:
			virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const;
			virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
		private:
			INICacheDataPatch(const INICacheDataPatch&) = default;
			INICacheDataPatch& operator=(const INICacheDataPatch&) = default;
		};
	}
}