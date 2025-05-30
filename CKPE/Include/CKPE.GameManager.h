// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <string>
#include <cstdint>
#include <map>
#include <CKPE.Common.h>
#include <CKPE.Application.h>

struct CKPEGameLibraryData
{
	enum { kVersion = 1, };

	std::uint32_t cbVersion;		// set to kVersion
	std::uint32_t dataVersion;		// version number of game library
	char name[256];					// null-terminated ASCII game library name
	char author[256];				// null-terminated ASCII game library author name
};

typedef std::uint32_t GameLibraryHandle;

struct CKPEGameLibraryInterface
{
	enum { kInterfaceVersion = 1, };

	std::uint32_t interfaceVersion;
	std::uint32_t ckpeVersion;

	void* (*QueryInterface)(std::uint32_t id);
	GameLibraryHandle (*GetGameLibraryHandle)(void);
};

typedef std::uint32_t (*_CKPEGameLibrary_Query)();
typedef bool (*_CKPEGameLibrary_Load)(const CKPEGameLibraryInterface* ckpe);

namespace CKPE
{
	class CKPE_API GameManager
	{
		bool _init{ false };

		void InitializeGameLibraries() noexcept(true);
		std::uint32_t StartGameLibraries() noexcept(true);
	public:
		enum : std::uint32_t
		{
			CK_UNKNOWN = (std::uint32_t)-1,
			CK_SUPPORTED = 0,
			CK_UNSUPPORTED,
			CK_DEPRECATED,
		};

		struct CKPE_API LibaryInfo
		{
			enum
			{
				kInfoVersion = 1
			};

			std::uint32_t infoVersion;
			std::uint32_t version;
			wchar_t name[128];
		};

		struct CKPE_API LoadedLibrary
		{
			LoadedLibrary();

			wchar_t dllName[128];
			void* handle;
			LibaryInfo info;
			GameLibraryHandle internalHandle;
			CKPEGameLibraryData	data;
			_CKPEGameLibrary_Load load;
			_CKPEGameLibrary_Query query;
			char errorState[128];
			std::uint32_t errorCode;
			bool hasLoad;
			bool hasQuery;
		};

		constexpr GameManager() = default;
		~GameManager();
	
		std::uint32_t Initialize() const noexcept(true);
		
		[[nodiscard]] std::uint32_t GetCount() const noexcept(true);
		[[nodiscard]] static const GameManager* GetSingleton() noexcept(true);
	private:
		std::uint32_t SafeQuery(const LoadedLibrary& lib);
		bool SafeLoad(const LoadedLibrary& lib);

		std::map<GameLibraryHandle, LoadedLibrary>* _libs{ nullptr };
	};
}