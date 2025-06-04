// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.Common.h>
#include <string>
#include <cstdint>
#include <cstdio>
#include <map>

namespace CKPE
{
	namespace Common
	{
		enum SettingOptionType : std::uint32_t
		{
			sotBool = 0,			// b
			sotChar,				// c
			sotInteger,				// i, n
			sotUnsignedInteger,		// u
			sotHexadecimal,			// h
			sotFloat,				// f
			sotColorRGB,			// r
			sotColorRGBA,			// a
			sotString,				// s, S
			sotMAX
		};

		class CKPE_COMMON_API CustomSettingCollection
		{
			CustomSettingCollection(const CustomSettingCollection&) = delete;
			CustomSettingCollection& operator=(const CustomSettingCollection&) = delete;
		protected:
			bool _needSaves{ false };
			std::wstring* _fileName{ nullptr };
		public:
			struct CKPE_COMMON_API color_value
			{
				union
				{
					std::uint32_t u32;
					struct CKPE_COMMON_API { std::uint8_t r, g, b, a; };
				};

				constexpr color_value() noexcept(true) : u32(0) {}
			};

			CustomSettingCollection() noexcept(true);
			CustomSettingCollection(const std::string& filename) noexcept(true);
			CustomSettingCollection(std::int32_t folderID, const std::string& relPath) noexcept(true);
			CustomSettingCollection(const std::wstring& filename) noexcept(true);
			CustomSettingCollection(std::int32_t folderID, const std::wstring& relPath) noexcept(true);
			virtual ~CustomSettingCollection();

			[[nodiscard]] inline virtual std::wstring GetFileName() const noexcept(true) 
			{ return _fileName ? *_fileName : L""; }
			virtual void Open(const std::string& filename) noexcept(true);
			virtual void Open(const std::wstring& filename) noexcept(true);
			virtual void OpenRelative(std::int32_t folderID, const std::string& relPath) noexcept(true);
			virtual void OpenRelative(std::int32_t folderID, const std::wstring& relPath) noexcept(true);
			virtual void Save() noexcept(true);
			virtual void Dump(std::FILE* file) const noexcept(true) = 0;

			inline virtual void MarkNeedSave(bool mark = true) noexcept(true) { _needSaves = mark; }
			inline virtual bool IsOpen() const noexcept(true) { return _fileName ? _fileName->length() != 0 : false; }
			virtual bool IsEmpty() const noexcept(true) = 0;
			virtual bool Has(const std::string& section, const std::string& option) const noexcept(true) = 0;
			virtual bool Remove(const std::string& section) noexcept(true) = 0;
			virtual bool Remove(const std::string& section, const std::string& option) noexcept(true) = 0;
			virtual bool Cleanup() noexcept(true) = 0;

			[[nodiscard]] static bool SplitOptionName(const std::string& option, std::string& section, 
				std::string& name) noexcept(true);
			[[nodiscard]] static SettingOptionType GetOptionTypeByName(const char* name) noexcept(true);
			[[nodiscard]] static SettingOptionType GetOptionTypeByName(const std::string& name) noexcept(true);

			[[nodiscard]] virtual bool ReadBool(const std::string& section, const std::string& option,
				bool def) const noexcept(true) = 0;
			[[nodiscard]] virtual char ReadChar(const std::string& section, const std::string& option, 
				char def) const noexcept(true) = 0;
			[[nodiscard]] virtual long ReadInt(const std::string& section, const std::string& option, 
				long def) const noexcept(true) = 0;
			[[nodiscard]] virtual unsigned long ReadUInt(const std::string& section, const std::string& option, 
				unsigned long def) const noexcept(true) = 0;
			[[nodiscard]] virtual unsigned long ReadHex(const std::string& section, const std::string& option, 
				unsigned long def) const noexcept(true) = 0;
			[[nodiscard]] virtual float ReadFloat(const std::string& section, const std::string& option, 
				float def) const noexcept(true) = 0;
			[[nodiscard]] virtual color_value ReadRgbColor(const std::string& section, const std::string& option, 
				color_value def) const noexcept(true) = 0;
			[[nodiscard]] virtual color_value ReadRgbaColor(const std::string& section, const std::string& option,
				color_value def) const noexcept(true) = 0;
			// Для чтения ansi строк
			[[nodiscard]] virtual std::string ReadString(const std::string& section, const std::string& option, 
				const std::string& def) const noexcept(true) = 0;
			// Для чтения utf8 строк
			[[nodiscard]] virtual std::wstring ReadUnicodeString(const std::string& section, const std::string& option,
				const std::wstring& def) const noexcept(true) = 0;

			virtual void WriteBool(const std::string& section, const std::string& option, 
				bool value) noexcept(true) = 0;
			virtual void WriteChar(const std::string& section, const std::string& option, 
				char value) noexcept(true) = 0;
			virtual void WriteInt(const std::string& section, const std::string& option, 
				long value) noexcept(true) = 0;
			virtual void WriteUInt(const std::string& section, const std::string& option, 
				unsigned long value) noexcept(true) = 0;
			virtual void WriteHex(const std::string& section, const std::string& option, 
				unsigned long value) noexcept(true) = 0;
			virtual void WriteFloat(const std::string& section, const std::string& option, 
				float value) noexcept(true) = 0;
			virtual void WriteRgbColor(const std::string& section, const std::string& option, 
				color_value value) noexcept(true) = 0;
			virtual void WriteRgbaColor(const std::string& section, const std::string& option, 
				color_value value) noexcept(true) = 0;
			// Для записи ansi строк
			virtual void WriteString(const std::string& section, const std::string& option,
				const std::string& value) noexcept(true) = 0;
			// Для записи utf8 строк
			virtual void WriteUnicodeString(const std::string& section, const std::string& option,
				const std::wstring& value) noexcept(true) = 0;
		protected:
			virtual bool DoOpen(const std::wstring& filename) noexcept(true) = 0;
			virtual bool DoSave() const noexcept(true) = 0;
		};

		class CKPE_COMMON_API TOMLSettingCollection : public CustomSettingCollection
		{
			class CKPE_COMMON_API TOMLStructure
			{
				TOMLStructure(const TOMLStructure&) = delete;
				TOMLStructure& operator=(const TOMLStructure&) = delete;
			public:
				void* data { nullptr };
				
				constexpr TOMLStructure() noexcept(true) = default;
				~TOMLStructure() noexcept(true);
			};

			TOMLSettingCollection(const TOMLSettingCollection&) = delete;
			TOMLSettingCollection& operator=(const TOMLSettingCollection&) = delete;

			TOMLStructure _tomlData;
		public:
			TOMLSettingCollection() noexcept(true) = default;
			TOMLSettingCollection(const std::string& filename) noexcept(true);
			TOMLSettingCollection(std::int32_t folderID, const std::string& relPath) noexcept(true);
			TOMLSettingCollection(const std::wstring& filename) noexcept(true);
			TOMLSettingCollection(std::int32_t folderID, const std::wstring& relPath) noexcept(true);

			virtual void Dump(FILE* file) const noexcept(true);

			virtual bool IsEmpty() const noexcept(true);
			virtual bool Has(const std::string& section, const std::string& option) const noexcept(true);
			virtual bool Remove(const std::string& section) noexcept(true);
			virtual bool Remove(const std::string& section, const std::string& option) noexcept(true);
			virtual bool Cleanup() noexcept(true);

			[[nodiscard]] virtual bool ReadBool(const std::string& section, const std::string& option, 
				bool def) const noexcept(true);
			[[nodiscard]] virtual char ReadChar(const std::string& section, const std::string& option, 
				char def) const noexcept(true);
			[[nodiscard]] virtual long ReadInt(const std::string& section, const std::string& option, 
				long def) const noexcept(true);
			[[nodiscard]] virtual unsigned long ReadUInt(const std::string& section, const std::string& option,
				unsigned long def) const noexcept(true);
			[[nodiscard]] virtual unsigned long ReadHex(const std::string& section, const std::string& option, 
				unsigned long def) const noexcept(true);
			[[nodiscard]] virtual float ReadFloat(const std::string& section, const std::string& option, 
				float def) const noexcept(true);
			[[nodiscard]] virtual color_value ReadRgbColor(const std::string& section, const std::string& option,
				color_value def) const noexcept(true);
			[[nodiscard]] virtual color_value ReadRgbaColor(const std::string& section, const std::string& option,
				color_value def) const noexcept(true);
			// Для чтения ansi строк
			[[nodiscard]] virtual std::string ReadString(const std::string& section, const std::string& option, 
				const std::string& def) const noexcept(true);
			// Для чтения utf8 строк
			[[nodiscard]] virtual std::wstring ReadUnicodeString(const std::string& section, const std::string& option, 
				const std::wstring& def) const noexcept(true);

			virtual void WriteBool(const std::string& section, const std::string& option,
				bool value) noexcept(true);
			virtual void WriteChar(const std::string& section, const std::string& option,
				char value) noexcept(true);
			virtual void WriteInt(const std::string& section, const std::string& option,
				long value) noexcept(true);
			virtual void WriteUInt(const std::string& section, const std::string& option,
				unsigned long value) noexcept(true);
			virtual void WriteHex(const std::string& section, const std::string& option,
				unsigned long value) noexcept(true);
			virtual void WriteFloat(const std::string& section, const std::string& option,
				float value) noexcept(true);
			virtual void WriteRgbColor(const std::string& section, const std::string& option,
				color_value value) noexcept(true);
			virtual void WriteRgbaColor(const std::string& section, const std::string& option,
				color_value value) noexcept(true);
			// Для записи ansi строк
			virtual void WriteString(const std::string& section, const std::string& option, 
				const std::string& value) noexcept(true);
			// Для записи utf8 строк
			virtual void WriteUnicodeString(const std::string& section, const std::string& option, 
				const std::wstring& value) noexcept(true);
		protected:
			virtual bool DoOpen(const std::wstring& filename) noexcept(true);
			virtual bool DoSave() const noexcept(true);
		};

		class CKPE_COMMON_API INISettingCollection : public CustomSettingCollection
		{
			class CKPE_COMMON_API INIStructure
			{
				INIStructure(const INIStructure&) = delete;
				INIStructure& operator=(const INIStructure&) = delete;
			public:
				using _Mbase = std::map<std::string, std::map<std::string, std::string>>;
				_Mbase* data{ nullptr };

				constexpr INIStructure() noexcept(true) = default;
				~INIStructure();
			};

			INISettingCollection(const INISettingCollection&) = delete;
			INISettingCollection& operator=(const INISettingCollection&) = delete;

			INIStructure _iniData;
		public:
			INISettingCollection() noexcept(true) = default;
			INISettingCollection(const std::string& filename) noexcept(true);
			INISettingCollection(std::int32_t folderID, const std::string& relPath) noexcept(true);
			INISettingCollection(const std::wstring& filename) noexcept(true);
			INISettingCollection(std::int32_t folderID, const std::wstring& relPath) noexcept(true);

			virtual void Dump(FILE* file) const noexcept(true);

			virtual bool IsEmpty() const noexcept(true);
			virtual bool Has(const std::string& section, const std::string& option) const noexcept(true);
			virtual bool Remove(const std::string& section) noexcept(true);
			virtual bool Remove(const std::string& section, const std::string& option) noexcept(true);
			virtual bool Cleanup() noexcept(true);

			[[nodiscard]] inline virtual INIStructure::_Mbase::const_iterator FirstUnsafe() const noexcept(true)
			{
				return _iniData.data->begin();
			}
			[[nodiscard]] inline virtual INIStructure::_Mbase::const_iterator LastUnsafe() const noexcept(true)
			{
				return _iniData.data->end();
			}

			[[nodiscard]] virtual INIStructure::_Mbase::const_iterator ReadSection
				(const std::string& section) const noexcept(true);
			[[nodiscard]] virtual bool ReadBool(const std::string& section, const std::string& option,
				bool def) const noexcept(true);
			[[nodiscard]] virtual char ReadChar(const std::string& section, const std::string& option,
				char def) const noexcept(true);
			[[nodiscard]] virtual long ReadInt(const std::string& section, const std::string& option,
				long def) const noexcept(true);
			[[nodiscard]] virtual unsigned long ReadUInt(const std::string& section, const std::string& option,
				unsigned long def) const noexcept(true);
			[[nodiscard]] virtual unsigned long ReadHex(const std::string& section, const std::string& option,
				unsigned long def) const noexcept(true);
			[[nodiscard]] virtual float ReadFloat(const std::string& section, const std::string& option, 
				float def) const noexcept(true);
			[[nodiscard]] virtual color_value ReadRgbColor(const std::string& section, const std::string& option, 
				color_value def) const noexcept(true);
			[[nodiscard]] virtual color_value ReadRgbaColor(const std::string& section, const std::string& option, 
				color_value def) const noexcept(true);
			// Для чтения ansi строк
			[[nodiscard]] virtual std::string ReadString(const std::string& section, const std::string& option, 
				const std::string& def) const noexcept(true);
			// Для чтения utf8 строк
			[[nodiscard]] virtual std::wstring ReadUnicodeString(const std::string& section, const std::string& option, 
				const std::wstring& def) const noexcept(true);

			virtual void WriteBool(const std::string& section, const std::string& option, 
				bool value) noexcept(true);
			virtual void WriteChar(const std::string& section, const std::string& option, 
				char value) noexcept(true);
			virtual void WriteInt(const std::string& section, const std::string& option, 
				long value) noexcept(true);
			virtual void WriteUInt(const std::string& section, const std::string& option, 
				unsigned long value) noexcept(true);
			virtual void WriteHex(const std::string& section, const std::string& option, 
				unsigned long value) noexcept(true);
			virtual void WriteFloat(const std::string& section, const std::string& option, 
				float value) noexcept(true);
			virtual void WriteRgbColor(const std::string& section, const std::string& option, 
				color_value def) noexcept(true);
			virtual void WriteRgbaColor(const std::string& section, const std::string& option, 
				color_value def) noexcept(true);
			// Для записи ansi строк
			virtual void WriteString(const std::string& section, const std::string& option, 
				const std::string& value) noexcept(true);
			// Для записи utf8 строк
			virtual void WriteUnicodeString(const std::string& section, const std::string& option,
				const std::wstring& value) noexcept(true);
		protected:
			virtual bool DoOpen(const std::wstring& filename) noexcept(true);
			virtual bool DoOpenSerialization(const char* text, size_t len) noexcept(true);
			virtual bool DoSave() const noexcept(true);
		};
	}
}