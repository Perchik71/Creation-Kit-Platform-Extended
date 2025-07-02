// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <Windows.h>
#include <CKPE.Common.Registry.h>
#include <CKPE.Exception.h>

#define reg_type HKEY_LOCAL_MACHINE

namespace CKPE
{
	namespace Common
	{
		const void* Registry::GetBaseKey() const noexcept(true)
		{
			return ((void*)(ULONG_PTR)((LONG)0x80000000 + (LONG)_root));
		}

		bool Registry::GetData(const std::string& valume_name, void* buffer, std::uint32_t size,
			RegDataType& type) noexcept(true)
		{
			if (!_handle || !buffer) return false;

			_lasterr = RegQueryValueExA((HKEY)_handle,
				valume_name.c_str(), NULL, (LPDWORD)&type, (LPBYTE)buffer, (LPDWORD)&size);

			if (_lasterr != ERROR_SUCCESS)
			{
				type = rdUnknown;

				return false;
			}

			if ((std::uint32_t)type > (std::uint32_t)rdInt64)
			{
				type = rdUnknown;

				return false;
			}

			return true;
		}

		bool Registry::GetData(const std::wstring& valume_name, void* buffer, std::uint32_t size,
			RegDataType& type) noexcept(true)
		{
			if (!_handle || !buffer) return false;

			_lasterr = RegQueryValueExW((HKEY)_handle,
				valume_name.c_str(), NULL, (LPDWORD)&type, (LPBYTE)buffer, (LPDWORD)&size);

			if (_lasterr != ERROR_SUCCESS)
			{
				type = rdUnknown;
				size = 0;

				return false;
			}

			if ((std::uint32_t)type > (std::uint32_t)rdInt64)
			{
				type = rdUnknown;
				size = 0;

				return false;
			}

			return true;
		}

		bool Registry::PutData(const std::string& valume_name, const void* buffer, std::uint32_t size,
			RegDataType type) noexcept(true)
		{
			if (!_handle || (type == rdUnknown) || !buffer) return false;

			_lasterr = (std::int32_t)RegSetValueExA((HKEY)_handle,
				valume_name.c_str(), 0, (DWORD)type, (const LPBYTE)buffer, (DWORD)size);
			return _lasterr == ERROR_SUCCESS;
		}

		bool Registry::PutData(const std::wstring& valume_name, const void* buffer, std::uint32_t size,
			RegDataType type) noexcept(true)
		{
			if (!_handle || (type == rdUnknown) || !buffer) return false;
			
			_lasterr = (std::int32_t)RegSetValueExW((HKEY)_handle, 
				valume_name.c_str(), 0, (DWORD)type, (const LPBYTE)buffer, (DWORD)size);
		    return _lasterr == ERROR_SUCCESS;
		}

		Registry::~Registry() noexcept(true)
		{
			CloseKey();
		}

		void Registry::SetRootKey(RootKey root) noexcept(true)
		{
			CloseKey();

			_root = root;
		}

		bool Registry::OpenKey(const std::string& key, bool can_create) noexcept(true)
		{
			if (can_create)
			{
				_lasterr = (std::int32_t)RegCreateKeyExA((HKEY)GetBaseKey(),
					key.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, (PHKEY)&_handle,
					NULL);
				if (_lasterr == ERROR_SUCCESS)
				{
					_current_path = new std::wstring(StringUtils::WinCPToUtf16(key));
					return true;
				}
			}
			else
			{
				_lasterr = (std::int32_t)RegOpenKeyExA((HKEY)GetBaseKey(),
					key.c_str(), 0, KEY_ALL_ACCESS, (PHKEY)&_handle);
				if (_lasterr == ERROR_SUCCESS)
				{
					_current_path = new std::wstring(StringUtils::WinCPToUtf16(key));
					return true;
				}
			}

			return false;
		}

		bool Registry::OpenKey(const std::wstring& key, bool can_create) noexcept(true)
		{
			if (can_create)
			{
				_lasterr = (std::int32_t)RegCreateKeyExW((HKEY)GetBaseKey(), 
					key.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, (PHKEY)&_handle,
					NULL);
				if (_lasterr == ERROR_SUCCESS)
				{
					_current_path = new std::wstring(key);
					return true;
				}
			}
			else
			{
				_lasterr = (std::int32_t)RegOpenKeyExW((HKEY)GetBaseKey(), 
					key.c_str(), 0, KEY_ALL_ACCESS, (PHKEY)&_handle);
				if (_lasterr == ERROR_SUCCESS)
				{
					_current_path = new std::wstring(key);
					return true;
				}
			}

			return false;
		}

		bool Registry::OpenKeyReadOnly(const std::string& key) noexcept(true)
		{
			_lasterr = (std::int32_t)RegOpenKeyExA((HKEY)GetBaseKey(),
				key.c_str(), 0, KEY_READ, (PHKEY)&_handle);
			if (_lasterr == ERROR_SUCCESS)
			{
				_current_path = new std::wstring(StringUtils::WinCPToUtf16(key));
				return true;
			}
			return false;
		}

		bool Registry::OpenKeyReadOnly(const std::wstring& key) noexcept(true)
		{
			_lasterr = (std::int32_t)RegOpenKeyExW((HKEY)GetBaseKey(),
				key.c_str(), 0, KEY_READ, (PHKEY)&_handle);
			if (_lasterr == ERROR_SUCCESS)
			{
				_current_path = new std::wstring(key);
				return true;
			}
			return false;
		}

		void Registry::CloseKey() noexcept(true)
		{
			if (_handle)
			{
				_lasterr = (std::int32_t)RegCloseKey((HKEY)_handle);
				_handle = nullptr;

				if (_current_path)
				{
					delete _current_path;
					_current_path = nullptr;
				}
			}
		}

		bool Registry::GetDataInfo(const std::string& valume_name, RegDataInfo* value) noexcept(true)
		{
			if (!value || !_handle) return false;

			_lasterr = RegQueryValueExA((HKEY)_handle,
				valume_name.c_str(), NULL, (LPDWORD)(&(value->RegData)), NULL, (LPDWORD)(&(value->SizeData)));

			if (_lasterr != ERROR_SUCCESS)
			{
				value->RegData = rdUnknown;
				value->SizeData = 0;

				return false;
			}

			if ((std::uint32_t)value->RegData > (std::uint32_t)rdInt64)
			{
				value->RegData = rdUnknown;
				value->SizeData = 0;

				return false;
			}

			return true;
		}

		bool Registry::GetDataInfo(const std::wstring& valume_name, RegDataInfo* value) noexcept(true)
		{
			if (!value || !_handle) return false;

			_lasterr = RegQueryValueExW((HKEY)_handle,
				valume_name.c_str(), NULL, (LPDWORD)(&(value->RegData)), NULL, (LPDWORD)(&(value->SizeData)));

			if (_lasterr != ERROR_SUCCESS)
			{
				value->RegData = rdUnknown;
				value->SizeData = 0;

				return false;
			}

			if ((std::uint32_t)value->RegData > (std::uint32_t)rdInt64)
			{
				value->RegData = rdUnknown;
				value->SizeData = 0;

				return false;
			}

			return true;
		}

		std::uint32_t Registry::GetDataSize(const std::string& valume_name) noexcept(true)
		{
			RegDataInfo value{};
			if (GetDataInfo(valume_name, &value))
				return value.SizeData;
			return 0;
		}

		std::uint32_t Registry::GetDataSize(const std::wstring& valume_name) noexcept(true)
		{
			RegDataInfo value{};
			if (GetDataInfo(valume_name, &value))
				return value.SizeData;
			return 0;
		}

		Registry::RegDataType Registry::GetDataType(const std::string& valume_name) noexcept(true)
		{
			RegDataInfo value{};
			if (GetDataInfo(valume_name, &value))
				return value.RegData;
			return rdUnknown;
		}

		Registry::RegDataType Registry::GetDataType(const std::wstring& valume_name) noexcept(true)
		{
			RegDataInfo value{};
			if (GetDataInfo(valume_name, &value))
				return value.RegData;
			return rdUnknown;
		}

		std::uint32_t Registry::ReadBinaryData(const std::string& valume_name, void* buffer, std::uint32_t size) noexcept(true)
		{
			RegDataType type;
			if (!GetData(valume_name, buffer, size, type))
				return 0;
			return size;
		}

		std::uint32_t Registry::ReadBinaryData(const std::wstring& valume_name, void* buffer, std::uint32_t size) noexcept(true)
		{
			RegDataType type;
			if (!GetData(valume_name, buffer, size, type))
				return 0;
			return size;
		}

		bool Registry::ReadBool(const std::string& valume_name) noexcept(true)
		{
			return ReadInteger(valume_name);
		}

		bool Registry::ReadBool(const std::wstring& valume_name) noexcept(true)
		{
			return ReadInteger(valume_name);
		}

		double Registry::ReadFloat(const std::string& valume_name)
		{
			double buffer{};
			if (ReadBinaryData(valume_name, &buffer, sizeof(double)) != sizeof(double))
				throw std::runtime_error("Registry::ReadFloat invalid type");
			return buffer;
		}

		double Registry::ReadFloat(const std::wstring& valume_name)
		{
			double buffer{};
			if (ReadBinaryData(valume_name, &buffer, sizeof(double)) != sizeof(double))
				throw std::runtime_error("Registry::ReadFloat invalid type");
			return buffer;
		}

		std::int32_t Registry::ReadInteger(const std::string& valume_name)
		{
			std::int32_t buffer;
			RegDataType type;
			GetData(valume_name, (void*)&buffer, sizeof(std::int32_t), type);
			if (type != rdInteger)
				throw std::runtime_error("Registry::ReadInteger invalid type");
			return buffer;
		}

		std::int32_t Registry::ReadInteger(const std::wstring& valume_name)
		{
			std::int32_t buffer;
			RegDataType type;
			GetData(valume_name, (void*)&buffer, sizeof(std::int32_t), type);
			if (type != rdInteger)
				throw std::runtime_error("Registry::ReadInteger invalid type");
			return buffer;
		}

		std::int64_t Registry::ReadInt64(const std::string& valume_name)
		{
			std::int64_t buffer;
			RegDataType type;
			GetData(valume_name, (void*)&buffer, sizeof(std::int64_t), type);
			if (type != rdInt64)
				throw std::runtime_error("Registry::ReadInt64 invalid type");
			return buffer;
		}

		std::int64_t Registry::ReadInt64(const std::wstring& valume_name)
		{
			std::int64_t buffer;
			RegDataType type;
			GetData(valume_name, (void*)&buffer, sizeof(std::int64_t), type);
			if (type != rdInt64)
				throw std::runtime_error("Registry::ReadInt64 invalid type");
			return buffer;
		}

		std::string Registry::ReadString(const std::string& valume_name)
		{
			auto size = GetDataSize(valume_name);
			if (!size) return "";

			std::wstring s;
			s.resize(size / sizeof(wchar_t));
			if (s.data() == nullptr) return "";

			RegDataType type;
			GetData(valume_name, (void*)s.data(), size, type);
			if (type != rdString)
				throw std::runtime_error("Registry::ReadString invalid type");
			return StringUtils::Utf16ToUtf8(s);
		}

		std::wstring Registry::ReadString(const std::wstring& valume_name)
		{
			auto size = GetDataSize(valume_name);
			if (!size) return L"";

			std::wstring s;
			s.resize(size / sizeof(wchar_t));
			if (s.data() == nullptr) return L"";

			RegDataType type;
			GetData(valume_name, (void*)s.data(), size, type);
			if (type != rdString)
				throw std::runtime_error("Registry::ReadString invalid type");
			return s;
		}

		void Registry::WriteBinaryData(const std::string& valume_name, const void* buffer, std::uint32_t size) noexcept(true)
		{
			PutData(valume_name, buffer, size, rdBinary);
		}

		void Registry::WriteBinaryData(const std::wstring& valume_name, const void* buffer, std::uint32_t size) noexcept(true)
		{
			PutData(valume_name, buffer, size, rdBinary);
		}

		void Registry::WriteBool(const std::string& valume_name, bool val) noexcept(true)
		{
			WriteInteger(valume_name, val);
		}

		void Registry::WriteBool(const std::wstring& valume_name, bool val) noexcept(true)
		{
			WriteInteger(valume_name, val);
		}

		void Registry::WriteFloat(const std::string& valume_name, double val) noexcept(true)
		{
			WriteBinaryData(valume_name, &val, (std::uint32_t)sizeof(double));
		}

		void Registry::WriteFloat(const std::wstring& valume_name, double val) noexcept(true)
		{
			WriteBinaryData(valume_name, &val, (std::uint32_t)sizeof(double));
		}

		void Registry::WriteInteger(const std::string& valume_name, std::int32_t val) noexcept(true)
		{
			PutData(valume_name, &val, (std::uint32_t)sizeof(std::int32_t), rdInteger);
		}

		void Registry::WriteInteger(const std::wstring& valume_name, std::int32_t val) noexcept(true)
		{
			PutData(valume_name, &val, (std::uint32_t)sizeof(std::int32_t), rdInteger);
		}

		void Registry::WriteInt64(const std::string& valume_name, std::int64_t val) noexcept(true)
		{
			PutData(valume_name, &val, (std::uint32_t)sizeof(std::int64_t), rdInt64);
		}

		void Registry::WriteInt64(const std::wstring& valume_name, std::int64_t val) noexcept(true)
		{
			PutData(valume_name, &val, (std::uint32_t)sizeof(std::int64_t), rdInt64);
		}

		void Registry::WriteString(const std::string& valume_name, const std::string& str) noexcept(true)
		{
			auto s = StringUtils::Utf8ToUtf16(str);
			PutData(valume_name, s.c_str(), (std::uint32_t)(s.length() * sizeof(wchar_t)), rdString);
		}

		void Registry::WriteString(const std::wstring& valume_name, const std::wstring& str) noexcept(true)
		{
			PutData(valume_name, str.c_str(), (std::uint32_t)(str.length() * sizeof(wchar_t)), rdString);
		}

		bool Registry::ValueExists(const std::string& valume_name) noexcept(true)
		{
			RegDataInfo value{};
			return GetDataInfo(valume_name, &value);
		}

		bool Registry::ValueExists(const std::wstring& valume_name) noexcept(true)
		{
			RegDataInfo value{};
			return GetDataInfo(valume_name, &value);
		}
	}
}