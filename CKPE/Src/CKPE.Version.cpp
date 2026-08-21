#include <CKPE.Version.h>
#include <sstream>
#include <vector>

#include <Windows.h>

namespace CKPE
{
	CKPE_API std::optional<Version> GetFileVersion(const char* a_filename) noexcept
	{
		DWORD dummy{};
		std::vector<char> buf(GetFileVersionInfoSizeA(a_filename, std::addressof(dummy)));
		if (buf.empty()) return std::nullopt;

		if (!GetFileVersionInfoA(a_filename, 0, static_cast<DWORD>(buf.size()), buf.data()))
			return std::nullopt;

		void*         verBuf{ nullptr };
		std::uint32_t verLen{ 0 };
		if (!VerQueryValueA(buf.data(), "\\StringFileInfo\\040904B0\\ProductVersion", 
			std::addressof(verBuf), std::addressof(verLen)))
			return std::nullopt;

		Version            version;
		std::istringstream ss(std::string(static_cast<const char*>(verBuf), verLen));
		std::string        token;
		for (std::size_t i = 0; i < 4 && std::getline(ss, token, '.'); ++i)
			version[i] = static_cast<std::uint16_t>(std::stoi(token));
		return version;
	}

	CKPE_API std::optional<Version> GetFileVersion(const wchar_t* a_filename) noexcept
	{
		DWORD dummy{};
		std::vector<char> buf(GetFileVersionInfoSizeW(a_filename, std::addressof(dummy)));
		if (buf.empty()) return std::nullopt;

		if (!GetFileVersionInfoW(a_filename, 0, static_cast<DWORD>(buf.size()), buf.data()))
			return std::nullopt;

		void* verBuf{ nullptr };
		std::uint32_t verLen{ 0 };
		if (!VerQueryValueW(buf.data(), L"\\StringFileInfo\\040904B0\\ProductVersion",
			std::addressof(verBuf), std::addressof(verLen)))
			return std::nullopt;

		Version             version;
		std::wistringstream ss(std::wstring(static_cast<const wchar_t*>(verBuf), verLen));
		std::wstring        token;
		for (std::size_t i = 0; i < 4 && std::getline(ss, token, L'.'); ++i)
			version[i] = static_cast<std::uint16_t>(std::stoi(token));
		return version;
	}
}
