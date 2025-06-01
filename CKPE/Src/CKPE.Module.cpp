// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Module.h>
#include <CKPE.PathUtils.h>
#include <stdexcept>

namespace CKPE
{
	static constexpr std::array SEGMENTS{
			std::string_view(".text"),
			std::string_view(".interpr"),
			std::string_view(".idata"),
			std::string_view(".rdata"),
			std::string_view(".data"),
			std::string_view(".pdata"),
			std::string_view(".tls")
	};

	void Module::LoadSegments()
	{
		if (!_handle)
			throw std::invalid_argument("_base is null");

		PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)(GetBase() + ((PIMAGE_DOS_HEADER)GetBase())->e_lfanew);
		PIMAGE_SECTION_HEADER cur_section = IMAGE_FIRST_SECTION(ntHeader);
		const std::uint32_t size = std::min<std::uint32_t>(ntHeader->FileHeader.NumberOfSections, (std::uint32_t)_segments.size());
		char sectionName[sizeof(IMAGE_SECTION_HEADER::Name) + 1]{};
		constexpr auto sectionNameMaxLen = sizeof(IMAGE_SECTION_HEADER::Name);

		for (std::uint32_t i = 0; i < size; i++, cur_section++)
		{
			const auto it = std::find_if(SEGMENTS.begin(), SEGMENTS.end(), [&](auto&& a_elem)
				{
					return std::memcmp(a_elem.data(), cur_section->Name, std::min(a_elem.size(), sectionNameMaxLen)) == 0;
				});

			if (it != SEGMENTS.end())
			{
				const auto idx = static_cast<std::size_t>(std::distance(SEGMENTS.begin(), it));
				_segments[idx] = Segment{ GetBase(), GetBase() + cur_section->VirtualAddress, cur_section->Misc.VirtualSize };
			}
		}
	}

	void Module::LoadDirectories()
	{
		PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)(GetBase() + ((PIMAGE_DOS_HEADER)GetBase())->e_lfanew);
		auto data = ntHeaders->OptionalHeader.DataDirectory;

		for (std::uint32_t idx = 0; idx < PEDirectory::total; idx++)
			_directories[idx] = PEDirectory{ GetBase(), GetBase() + data[idx].VirtualAddress, data[idx].Size };
	}

	Module::Module(const void* m) noexcept(true) :
		_handle(m)
	{
		_fname = new std::wstring;
		if (_fname)
		{
			_fname->resize(1024);
			_fname->resize(GetModuleFileNameW((HMODULE)m, _fname->data(), 1024));
			PathUtils::Normalize(*_fname);
		}
	}

	void Module::LoadInfo()
	{
		LoadSegments();
		LoadDirectories();
	}

	bool Module::Is64() const noexcept(true)
	{
		auto* base = (const char*)(std::uintptr_t(_handle) & ~3);
		auto* dosHeader = (const IMAGE_DOS_HEADER*)base;
		auto* ntHeader = (const IMAGE_NT_HEADERS*)(base + dosHeader->e_lfanew);
		// FileHeader is PE32/64 independent
		return ntHeader->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64;
	}

	const void* Module::Resource::GetProcAddress(const char* exportName) const noexcept(true)
	{
		auto* base = (const char*)(uintptr_t(_module->GetBase()) & ~3);
		auto* dosHeader = (const IMAGE_DOS_HEADER*)base;
		auto* ntHeader = (const IMAGE_NT_HEADERS*)(base + dosHeader->e_lfanew);
		auto* exportTable = (const IMAGE_EXPORT_DIRECTORY*)(base + 
				ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

		auto* exportAddresses = (const std::uint32_t*)(base + exportTable->AddressOfFunctions);			// RVA array
		auto* exportNameOrdinals = (const std::uint16_t*)(base + exportTable->AddressOfNameOrdinals);	// index in to exportAddresses
		auto* exportNames = (const std::uint32_t*)(base + exportTable->AddressOfNames);					// RVA array

		const void* result = nullptr;

		for (std::uint32_t i = 0; i < exportTable->NumberOfNames; i++)
		{
			std::uint32_t nameRVA = exportNames[i];
			auto* name = (const char*)(base + nameRVA);

			if (!strcmp(exportName, name))
			{
				std::uint32_t addrIdx = exportNameOrdinals[i];
				if (addrIdx < exportTable->NumberOfFunctions)
				{
					std::uint32_t addrRVA = exportAddresses[addrIdx];
					result = (const void*)(base + addrRVA);

					break;
				}
			}
		}

		return result;
	}

	[[nodiscard]] const void* Module::GetProcAddress(const char* name) const noexcept(true)
	{
		return _handle ? ::GetProcAddress((HMODULE)_handle, name) : nullptr;
	}

	ScopeLoadedModule::ScopeLoadedModule(const Module& m) noexcept(true) :
		_m(&m)
	{}

	ScopeLoadedModule::~ScopeLoadedModule() noexcept(true)
	{
		if (_m && _m->GetBase())
			FreeLibrary((HMODULE)_m->GetBase());
	}
}