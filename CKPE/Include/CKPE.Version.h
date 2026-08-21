#pragma once

#include <CKPE.Utils.h>

#include <array>
#include <optional>
#include <cstdint>
#include <string_view>
#include <algorithm>
#include <format>

namespace CKPE
{
	template <typename T>
	class VersionBase
	{
	public:
		using value_type = std::uint16_t;
		using reference = value_type&;
		using const_reference = const value_type&;

		constexpr VersionBase() noexcept = default;

		explicit constexpr VersionBase(std::array<value_type, 4> a_version) noexcept :
			_impl(a_version)
		{}

		constexpr VersionBase(value_type a_v1, value_type a_v2 = 0, value_type a_v3 = 0, value_type a_v4 = 0) noexcept :
			_impl{ a_v1, a_v2, a_v3, a_v4 }
		{}

		[[nodiscard]] constexpr reference       operator[](std::size_t a_idx) noexcept { return _impl[a_idx]; }
		[[nodiscard]] constexpr const_reference operator[](std::size_t a_idx) const noexcept { return _impl[a_idx]; }

		[[nodiscard]] constexpr decltype(auto) begin() const noexcept { return _impl.begin(); }
		[[nodiscard]] constexpr decltype(auto) cbegin() const noexcept { return _impl.cbegin(); }
		[[nodiscard]] constexpr decltype(auto) end() const noexcept { return _impl.end(); }
		[[nodiscard]] constexpr decltype(auto) cend() const noexcept { return _impl.cend(); }

		[[nodiscard]] std::strong_ordering constexpr compare(const VersionBase& a_rhs) const noexcept
		{
			for (std::size_t i = 0; i < _impl.size(); ++i) {
				if ((*this)[i] != a_rhs[i]) {
					return (*this)[i] < a_rhs[i] ? std::strong_ordering::less : std::strong_ordering::greater;
				}
			}
			return std::strong_ordering::equal;
		}

		[[nodiscard]] constexpr std::uint32_t pack() const noexcept
		{
			return static_cast<std::uint32_t>(
				(_impl[0] & T::AND_MAJOR) << T::SHL_MAJOR |
				(_impl[1] & T::AND_MINOR) << T::SHL_MINOR |
				(_impl[2] & T::AND_PATCH) << T::SHL_PATCH |
				(_impl[3] & T::AND_BUILD) << T::SHL_BUILD);
		}

		[[nodiscard]] constexpr value_type major() const noexcept { return _impl[0]; }
		[[nodiscard]] constexpr value_type minor() const noexcept { return _impl[1]; }
		[[nodiscard]] constexpr value_type patch() const noexcept { return _impl[2]; }
		[[nodiscard]] constexpr value_type build() const noexcept { return _impl[3]; }

		[[nodiscard]] constexpr std::string string(const std::string_view a_separator = "."sv) const
		{
			std::string result;
			for (auto&& ver : _impl) {
				result += std::to_string(ver);
				result.append(a_separator.data(), a_separator.size());
			}
			result.erase(result.size() - a_separator.size(), a_separator.size());
			return result;
		}

		[[nodiscard]] constexpr std::wstring wstring(const std::wstring_view a_separator = L"."sv) const
		{
			std::wstring result;
			for (auto&& ver : _impl) {
				result += std::to_wstring(ver);
				result.append(a_separator.data(), a_separator.size());
			}
			result.erase(result.size() - a_separator.size(), a_separator.size());
			return result;
		}

		[[nodiscard]] static constexpr VersionBase unpack(const std::uint32_t a_packedVersion) noexcept
		{
			return VersionBase{
				static_cast<value_type>((a_packedVersion >> T::SHL_MAJOR) & T::AND_MAJOR),
				static_cast<value_type>((a_packedVersion >> T::SHL_MINOR) & T::AND_MINOR),
				static_cast<value_type>((a_packedVersion >> T::SHL_PATCH) & T::AND_PATCH),
				static_cast<value_type>((a_packedVersion >> T::SHL_BUILD) & T::AND_BUILD)
			};
		}

		[[nodiscard]] friend constexpr bool operator==(const VersionBase& a_lhs, const VersionBase& a_rhs) noexcept
		{
			return a_lhs.compare(a_rhs) == 0;
		}

		[[nodiscard]] friend constexpr std::strong_ordering operator<=>(const VersionBase& a_lhs, const VersionBase& a_rhs) noexcept
		{
			return a_lhs.compare(a_rhs);
		}

	private:
		std::array<value_type, 4> _impl{ 0, 0, 0, 0 };
	};

	struct VersionPackInfo
	{
		static constexpr auto AND_MAJOR{ 0x0FF };
		static constexpr auto AND_MINOR{ 0x0FF };
		static constexpr auto AND_PATCH{ 0xFFF };
		static constexpr auto AND_BUILD{ 0x00F };
		static constexpr auto SHL_MAJOR{ 8 * 3 };
		static constexpr auto SHL_MINOR{ 8 * 2 };
		static constexpr auto SHL_PATCH{ 8 / 2 };
		static constexpr auto SHL_BUILD{ 8 * 0 };
	};

	using Version = VersionBase<VersionPackInfo>;

	[[nodiscard]] CKPE_API std::optional<Version> GetFileVersion(const char* a_filename) noexcept;
	[[nodiscard]] CKPE_API std::optional<Version> GetFileVersion(const wchar_t* a_filename) noexcept;
}

template <typename T, class CharT>
struct std::formatter<CKPE::VersionBase<T>, CharT> : formatter<std::string, CharT>
{
	template <class FormatContext>
	constexpr auto format(const CKPE::VersionBase<T>& a_version, FormatContext& a_ctx) const
	{
		return formatter<std::string, CharT>::format(a_version.string(), a_ctx);
	}
};
