// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <CKPE.Logger.h>
#include <CKPE.Common.Patch.h>
#include <CKPE.PluginAPI.PluginAPI.h>

namespace CKPE
{
	namespace PluginAPI
	{
		class CKPE_PLUGINAPI_API Plugin : public Common::Patch
		{
			std::wstring* _dllname{ nullptr };
			std::uintptr_t _base{ 0 };
			CKPEPluginVersionData _ver_data{};
			std::vector<std::uintptr_t>* _funcs{ nullptr };

			Plugin(const Plugin&) = delete;
			Plugin& operator=(const Plugin&) = delete;

			[[nodiscard]] static std::uintptr_t SafeLoadLibraryW(const wchar_t* fname);
			[[nodiscard]] bool SafeActive(const CKPEPluginInterface* intf) const;
			static void Sanitize(CKPEPluginVersionData* version) noexcept(true);
			[[nodiscard]] static const wchar_t* CheckPluginCompatibility(const CKPEPluginVersionData* version) noexcept(true);
		protected:
			virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
			virtual bool DoQuery() const noexcept(true);
		public:
			enum : std::int32_t
			{
				NoError = 0,
				ErrorInvalidArgs = -1,
				ErrorNoInit = -2,
				ErrorLoadLib = -3,
				ErrorInvalidLib = -4,
				ErrorNoCompatibility = -5,
			};
			
			Plugin() noexcept(true);
			virtual ~Plugin() noexcept(true);

			[[nodiscard]] virtual bool HasOption() const noexcept(true);
			[[nodiscard]] virtual const char* GetOptionName() const noexcept(true);
			[[nodiscard]] virtual bool HasDependencies() const noexcept(true);
			[[nodiscard]] virtual std::vector<std::string> GetDependencies() const noexcept(true);

			[[nodiscard]] constexpr virtual std::string GetAuthor() const noexcept(true) { return _ver_data.Author; }
			[[nodiscard]] constexpr virtual std::uint32_t GetVersion() const noexcept(true) { return _ver_data.PluginVersion; }
			[[nodiscard]] constexpr virtual const wchar_t* GetFileNameDLL() const noexcept(true) 
			{ return _dllname ? _dllname->c_str() : L""; }

			[[nodiscard]] constexpr virtual bool IsLoaded() const noexcept(true) { return _base != 0; }

			[[nodiscard]] virtual std::int32_t CanLoad(const char* fname) noexcept(true);
			[[nodiscard]] virtual std::int32_t CanLoad(const wchar_t* fname) noexcept(true);
			[[nodiscard]] virtual std::int32_t CanLoad(const std::string& fname) noexcept(true);
			[[nodiscard]] virtual std::int32_t CanLoad(const std::wstring& fname) noexcept(true);

			[[nodiscard]] virtual bool Load(const char* fname, bool needCheck = true) noexcept(true);
			[[nodiscard]] virtual bool Load(const wchar_t* fname, bool needCheck = true) noexcept(true);
			[[nodiscard]] virtual bool Load(const std::string& fname, bool needCheck = true) noexcept(true);
			[[nodiscard]] virtual bool Load(const std::wstring& fname, bool needCheck = true) noexcept(true);

			virtual bool Unload() noexcept(true);
		};
	}
}