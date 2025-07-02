// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.Common.h>
#include <string>
#include <cstdint>

namespace CKPE
{
	namespace Common
	{
		class CKPE_COMMON_API Registry
		{
		public:
			enum RootKey : std::uint32_t
			{
				ClassesRoot = 0,
				CurrentUser,
				LocalMashine,
				Users,
				PerformanceData,
				CurrentConfig,
				DynData,
				CurrentUserLocalSettings,
			};

			enum RegDataType : std::uint32_t
			{
				rdUnknown = 0,
				rdString,
				rdExpandString,
				rdBinary,
				rdInteger,
				rdIntegerBigEndian,
				rdLink,
				rdMultiString,
				rdResourceList,
				rdFullResourceDescriptor,
				rdResourceRequirementList,
				rdInt64,
			};

			struct RegDataInfo
			{
				RegDataType RegData;
				std::uint32_t SizeData;
			};
		private:
			Registry(const Registry&) = delete;
			Registry& operator=(const Registry&) = delete;

			std::int32_t _lasterr{ 0 };
			void* _handle{ nullptr };
			RootKey _root{ CurrentUser };
			std::wstring* _current_path{ nullptr };

			const void* GetBaseKey() const noexcept(true);
			bool GetData(const std::string& valume_name, void* buffer, std::uint32_t size, RegDataType& type) noexcept(true);
			bool GetData(const std::wstring& valume_name, void* buffer, std::uint32_t size, RegDataType& type) noexcept(true);
			bool PutData(const std::string& valume_name, const void* buffer, std::uint32_t size, RegDataType type) noexcept(true);
			bool PutData(const std::wstring& valume_name, const void* buffer, std::uint32_t size, RegDataType type) noexcept(true);
		public:
			constexpr Registry() noexcept(true) = default;
			virtual ~Registry() noexcept(true);

			virtual void SetRootKey(RootKey root) noexcept(true);
			[[nodiscard]] constexpr virtual RootKey GetRootKey(RootKey root) const noexcept(true) { return _root; }
			[[nodiscard]] constexpr virtual void* GetCurrentKey() const noexcept(true) { return _handle; }
			[[nodiscard]] constexpr virtual const std::wstring GetCurrentPath() const noexcept(true)
			{ return _current_path ? *_current_path : L""; }

			virtual bool OpenKey(const std::string& key, bool can_create) noexcept(true);
			virtual bool OpenKey(const std::wstring& key, bool can_create) noexcept(true);
			virtual bool OpenKeyReadOnly(const std::string& key) noexcept(true);
			virtual bool OpenKeyReadOnly(const std::wstring& key) noexcept(true);
			virtual void CloseKey() noexcept(true);

			virtual bool GetDataInfo(const std::string& valume_name, RegDataInfo* value) noexcept(true);
			virtual bool GetDataInfo(const std::wstring& valume_name, RegDataInfo* value) noexcept(true);
			virtual std::uint32_t GetDataSize(const std::string& valume_name) noexcept(true);
			virtual std::uint32_t GetDataSize(const std::wstring& valume_name) noexcept(true);
			virtual RegDataType GetDataType(const std::string& valume_name) noexcept(true);
			virtual RegDataType GetDataType(const std::wstring& valume_name) noexcept(true);
			virtual std::uint32_t ReadBinaryData(const std::string& valume_name, void* buffer, std::uint32_t size) noexcept(true);
			virtual std::uint32_t ReadBinaryData(const std::wstring& valume_name, void* buffer, std::uint32_t size) noexcept(true);
			[[nodiscard]] virtual bool ReadBool(const std::string& valume_name) noexcept(true);
			[[nodiscard]] virtual bool ReadBool(const std::wstring& valume_name) noexcept(true);
			[[nodiscard]] virtual double ReadFloat(const std::string& valume_name);
			[[nodiscard]] virtual double ReadFloat(const std::wstring& valume_name);
			[[nodiscard]] virtual std::int32_t ReadInteger(const std::string& valume_name);
			[[nodiscard]] virtual std::int32_t ReadInteger(const std::wstring& valume_name);
			[[nodiscard]] virtual std::int64_t ReadInt64(const std::string& valume_name);
			[[nodiscard]] virtual std::int64_t ReadInt64(const std::wstring& valume_name);
			[[nodiscard]] virtual std::string ReadString(const std::string& valume_name);
			[[nodiscard]] virtual std::wstring ReadString(const std::wstring& valume_name);
			virtual void WriteBinaryData(const std::string& valume_name, const void* buffer, std::uint32_t size) noexcept(true);
			virtual void WriteBinaryData(const std::wstring& valume_name, const void* buffer, std::uint32_t size) noexcept(true);
			[[nodiscard]] virtual void WriteBool(const std::string& valume_name, bool val) noexcept(true);
			[[nodiscard]] virtual void WriteBool(const std::wstring& valume_name, bool val) noexcept(true);
			[[nodiscard]] virtual void WriteFloat(const std::string& valume_name, double val) noexcept(true);
			[[nodiscard]] virtual void WriteFloat(const std::wstring& valume_name, double val) noexcept(true);
			[[nodiscard]] virtual void WriteInteger(const std::string& valume_name, std::int32_t val) noexcept(true);
			[[nodiscard]] virtual void WriteInteger(const std::wstring& valume_name, std::int32_t val) noexcept(true);
			[[nodiscard]] virtual void WriteInt64(const std::string& valume_name, std::int64_t val) noexcept(true);
			[[nodiscard]] virtual void WriteInt64(const std::wstring& valume_name, std::int64_t val) noexcept(true);
			[[nodiscard]] virtual void WriteString(const std::string& valume_name, const std::string& str) noexcept(true);
			[[nodiscard]] virtual void WriteString(const std::wstring& valume_name, const std::wstring& str) noexcept(true);
			[[nodiscard]] virtual bool ValueExists(const std::string& valume_name) noexcept(true);
			[[nodiscard]] virtual bool ValueExists(const std::wstring& valume_name) noexcept(true);
		};
	}
}