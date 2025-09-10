// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <string>
#include <cstdint>
#include <map>
#include <CKPE.Common.h>

namespace CKPE
{
	class CKPE_API Application;
	class CKPE_API Logger;
}

struct CKPEGameLibraryData
{
	enum { kVersion = 1, };

	std::uint32_t cbVersion;		// set to kVersion
	std::uint64_t dataVersion;		// version number of game library
	char name[256];					// null-terminated ASCII game library name
	char author[256];				// null-terminated ASCII game library author name
};

typedef std::uint32_t GameLibraryHandle;

struct CKPEGameLibraryInterface
{
	enum { kInterfaceVersion = 1, };

	std::uint32_t interfaceVersion;
	std::uint64_t ckpeVersion;
	CKPE::Application* application;
	CKPE::Logger* logger;
	void* (*QueryInterface)(std::uint32_t id);
};

typedef std::uint32_t (__stdcall *_CKPEGameLibrary_Query)(std::wstring&);
typedef bool (__stdcall *_CKPEGameLibrary_Load)(const CKPEGameLibraryInterface*);

namespace CKPE
{
	class CKPE_API GameManager
	{
		bool _init{ false };
		_CKPEGameLibrary_Load _load{ nullptr };
		_CKPEGameLibrary_Query _query{ nullptr };
		CKPEGameLibraryInterface _interface{ 0 };
	public:
		enum Game : std::uint32_t
		{
			CK_UNKNOWN = 0,
			CK_SKYRIMSE,
			CK_FALLOUT4,
			CK_STARFIELD
		};

		enum : std::uint32_t
		{
			SUPPORTED = 0,
			UNSUPPORTED,
			DEPRECATED,
			FAKE,
		};

		GameManager() = default;
	
		[[nodiscard]] bool Initialize(Game game) noexcept(true);
		[[nodiscard]] static const GameManager* GetSingleton() noexcept(true);
		[[nodiscard]] inline constexpr virtual Game GetType() const noexcept(true) { return _game_type; }
		[[nodiscard]] virtual std::uint32_t QueryLib(std::wstring& version);
		[[nodiscard]] virtual bool LoadLib();

		static void* QueryInterface(std::uint32_t id) noexcept(true);
	private:
		Game _game_type{ CK_UNKNOWN };
	};
}