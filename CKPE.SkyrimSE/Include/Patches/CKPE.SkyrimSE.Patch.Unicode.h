// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <list>
#include <CKPE.Common.Patch.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			class StringCache
			{
				std::list<std::string> cache;
			public:
				StringCache() = default;
				~StringCache() = default;
			public:
				inline std::size_t Size() const noexcept(true) { return cache.size(); }
				inline void Clear() noexcept(true) { cache.clear(); }
				inline void Push(const std::string& s) noexcept(true) { cache.push_back(s); }
				inline const char* Last() const noexcept(true) { return &cache.back()[0]; }
			};

			class ConvertorString
			{
				ConvertorString(const ConvertorString&) = delete;
				ConvertorString& operator=(const ConvertorString&) = delete;
			public:
				enum Mode : std::uint8_t {
					MODE_ANSI = 0,
					MODE_UTF8
				};
			private:
				const char* pre;
				Mode mode;
			public:
				ConvertorString() noexcept(true) = default;
				~ConvertorString() noexcept(true) = default;
			private:
				[[nodiscard]] bool IsValid(const char* s) const noexcept(true);
				[[nodiscard]] const char* Utf8ToWinCP(const char* src) noexcept(true);
				[[nodiscard]] const char* WinCPToUtf8(const char* src) noexcept(true);
			public:
				[[nodiscard]] Mode GetMode() const noexcept(true) { return mode; }
				inline void SetMode(Mode m) noexcept(true);
				[[nodiscard]] const char* Convert(const char* s) noexcept(true);
			};

			class Unicode : public Common::Patch
			{
				Unicode(const Unicode&) = delete;
				Unicode& operator=(const Unicode&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				Unicode();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				static bool BeginPluginSave() noexcept(true);
				static void EndPluginSave(std::int64_t hCursor) noexcept(true);

				static bool HKSetDlgItemTextA(std::int64_t hDlg, std::int32_t nIDDlgItem, const char* lpString) noexcept(true);
				static std::int64_t HKSendDlgItemMessageA(std::int64_t hDlg, std::int32_t nIDDlgItem, std::uint32_t Msg,
					std::int64_t wParam, std::int64_t lParam) noexcept(true);
			};
		}
	}
}