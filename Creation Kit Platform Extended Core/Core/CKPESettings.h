// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		enum SettingOptionType : int
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

		class CustomSettingCollection
		{
			CustomSettingCollection(const CustomSettingCollection&) = delete;
			CustomSettingCollection& operator=(const CustomSettingCollection&) = delete;
		protected:
			bool _needSaves;
			WideString _fileName;
		public:
			struct color_value
			{
				union
				{
					uint32_t u32;
					struct { uint8_t r, g, b, a; };
				};

				color_value() : u32(0) {}
			};

			CustomSettingCollection();
			CustomSettingCollection(const char* filename);
			CustomSettingCollection(int folderID, const char* relPath);
			CustomSettingCollection(const wchar_t* filename);
			CustomSettingCollection(int folderID, const wchar_t* relPath);
			virtual ~CustomSettingCollection();

			virtual void Open(const char* filename) noexcept(true);
			virtual void Open(const wchar_t* filename) noexcept(true);
			virtual void OpenRelative(int folderID, const char* relPath) noexcept(true);
			virtual void OpenRelative(int folderID, const wchar_t* relPath) noexcept(true);
			virtual void Save() noexcept(true);
			virtual void Dump(FILE* file) const noexcept(true) = 0;

			inline virtual void MarkNeedSave(bool mark = true) noexcept(true) { _needSaves = mark; }
			inline virtual bool IsOpen() const noexcept(true) { return _fileName.length() != 0; }
			virtual bool IsEmpty() const noexcept(true) = 0;
			virtual bool Has(const char* section, const char* option) const noexcept(true) = 0;
			virtual bool Remove(const char* section) noexcept(true) = 0;
			virtual bool Remove(const char* section, const char* option) noexcept(true) = 0;
			virtual bool Cleanup() noexcept(true) = 0;

			[[nodiscard]] static bool SplitOptionName(const char* option, String& section, String& name) noexcept(true);
			[[nodiscard]] static SettingOptionType GetOptionTypeByName(const char* name) noexcept(true);

			[[nodiscard]] virtual bool ReadBool(const char* section, const char* option, bool def) const noexcept(true) = 0;
			[[nodiscard]] virtual char ReadChar(const char* section, const char* option, char def) const noexcept(true) = 0;
			[[nodiscard]] virtual long ReadInt(const char* section, const char* option, long def) const noexcept(true) = 0;
			[[nodiscard]] virtual unsigned long ReadUInt(const char* section, const char* option, unsigned long def) const noexcept(true) = 0;
			[[nodiscard]] virtual unsigned long ReadHex(const char* section, const char* option, unsigned long def) const noexcept(true) = 0;
			[[nodiscard]] virtual float ReadFloat(const char* section, const char* option, float def) const noexcept(true) = 0;
			[[nodiscard]] virtual color_value ReadRgbColor(const char* section, const char* option, color_value def) const noexcept(true) = 0;
			[[nodiscard]] virtual color_value ReadRgbaColor(const char* section, const char* option, color_value def) const noexcept(true) = 0;
			// Для чтения ansi строк
			[[nodiscard]] virtual String ReadString(const char* section, const char* option, const char* def) const noexcept(true) = 0;
			// Для чтения utf8 строк
			[[nodiscard]] virtual WideString ReadUnicodeString(const char* section, const char* option, const wchar_t* def) const noexcept(true) = 0;

			virtual void WriteBool(const char* section, const char* option, bool value) noexcept(true) = 0;
			virtual void WriteChar(const char* section, const char* option, char value) noexcept(true) = 0;
			virtual void WriteInt(const char* section, const char* option, long value) noexcept(true) = 0;
			virtual void WriteUInt(const char* section, const char* option, unsigned long value) noexcept(true) = 0;
			virtual void WriteHex(const char* section, const char* option, unsigned long value) noexcept(true) = 0;
			virtual void WriteFloat(const char* section, const char* option, float value) noexcept(true) = 0;
			virtual void WriteRgbColor(const char* section, const char* option, color_value value) noexcept(true) = 0;
			virtual void WriteRgbaColor(const char* section, const char* option, color_value value) noexcept(true) = 0;
			// Для записи ansi строк
			virtual void WriteString(const char* section, const char* option, const char* value) noexcept(true) = 0;
			// Для записи utf8 строк
			virtual void WriteUnicodeString(const char* section, const char* option, const wchar_t* value) noexcept(true) = 0;
		protected:
			virtual bool DoOpen(const wchar_t* filename) noexcept(true) = 0;
			virtual bool DoSave() const noexcept(true) = 0;
		};

		class TOMLSettingCollection : public CustomSettingCollection
		{
			class TOMLStructure
			{
				TOMLStructure(const TOMLStructure&) = delete;
				TOMLStructure& operator=(const TOMLStructure&) = delete;
			public:
				toml::result<toml::basic_value<toml::type_config>, std::vector<toml::error_info>> data;

				TOMLStructure() : data(toml::failure<std::vector<toml::error_info>>(std::vector<toml::error_info>{})) {}
				~TOMLStructure() {}
			};

			TOMLSettingCollection(const TOMLSettingCollection&) = delete;
			TOMLSettingCollection& operator=(const TOMLSettingCollection&) = delete;

			SmartPointer<TOMLStructure> _tomlData;
		public:
			TOMLSettingCollection();
			TOMLSettingCollection(const char* filename);
			TOMLSettingCollection(int folderID, const char* relPath);
			TOMLSettingCollection(const wchar_t* filename);
			TOMLSettingCollection(int folderID, const wchar_t* relPath);

			virtual void Dump(FILE* file) const noexcept(true);

			virtual bool IsEmpty() const noexcept(true);
			virtual bool Has(const char* section, const char* option) const noexcept(true);
			virtual bool Remove(const char* section) noexcept(true);
			virtual bool Remove(const char* section, const char* option) noexcept(true);
			virtual bool Cleanup() noexcept(true);

			[[nodiscard]] virtual bool ReadBool(const char* section, const char* option, bool def) const noexcept(true);
			[[nodiscard]] virtual char ReadChar(const char* section, const char* option, char def) const noexcept(true);
			[[nodiscard]] virtual long ReadInt(const char* section, const char* option, long def) const noexcept(true);
			[[nodiscard]] virtual unsigned long ReadUInt(const char* section, const char* option, unsigned long def) const noexcept(true);
			[[nodiscard]] virtual unsigned long ReadHex(const char* section, const char* option, unsigned long def) const noexcept(true);
			[[nodiscard]] virtual float ReadFloat(const char* section, const char* option, float def) const noexcept(true);
			[[nodiscard]] virtual color_value ReadRgbColor(const char* section, const char* option, color_value def) const noexcept(true);
			[[nodiscard]] virtual color_value ReadRgbaColor(const char* section, const char* option, color_value def) const noexcept(true);
			// Для чтения ansi строк
			[[nodiscard]] virtual String ReadString(const char* section, const char* option, const char* def) const noexcept(true);
			// Для чтения utf8 строк
			[[nodiscard]] virtual WideString ReadUnicodeString(const char* section, const char* option, const wchar_t* def) const noexcept(true);

			virtual void WriteBool(const char* section, const char* option, bool value) noexcept(true);
			virtual void WriteChar(const char* section, const char* option, char value) noexcept(true);
			virtual void WriteInt(const char* section, const char* option, long value) noexcept(true);
			virtual void WriteUInt(const char* section, const char* option, unsigned long value) noexcept(true);
			virtual void WriteHex(const char* section, const char* option, unsigned long value) noexcept(true);
			virtual void WriteFloat(const char* section, const char* option, float value) noexcept(true);
			virtual void WriteRgbColor(const char* section, const char* option, color_value value) noexcept(true);
			virtual void WriteRgbaColor(const char* section, const char* option, color_value value) noexcept(true);
			// Для записи ansi строк
			virtual void WriteString(const char* section, const char* option, const char* value) noexcept(true);
			// Для записи utf8 строк
			virtual void WriteUnicodeString(const char* section, const char* option, const wchar_t* value) noexcept(true);
		protected:
			virtual bool DoOpen(const wchar_t* filename) noexcept(true);
			virtual bool DoSave() const noexcept(true);
		};

		// Необходимо создать, чтобы использовать нижние функции

		extern TOMLSettingCollection* GlobalCKPEConfigPtr;

		class INISettingCollection : public CustomSettingCollection
		{
			class INIStructure
			{
				INIStructure(const INIStructure&) = delete;
				INIStructure& operator=(const INIStructure&) = delete;
			public:
				Map<String, Map<String, String>> data;

				INIStructure() {}
				~INIStructure() {}
			};

			INISettingCollection(const INISettingCollection&) = delete;
			INISettingCollection& operator=(const INISettingCollection&) = delete;

			SmartPointer<INIStructure> _iniData;
		public:
			INISettingCollection();
			INISettingCollection(const char* filename);
			INISettingCollection(int folderID, const char* relPath);
			INISettingCollection(const wchar_t* filename);
			INISettingCollection(int folderID, const wchar_t* relPath);

			virtual void Dump(FILE* file) const noexcept(true);

			virtual bool IsEmpty() const noexcept(true);
			virtual bool Has(const char* section, const char* option) const noexcept(true);
			virtual bool Remove(const char* section) noexcept(true);
			virtual bool Remove(const char* section, const char* option) noexcept(true);
			virtual bool Cleanup() noexcept(true);

			[[nodiscard]] inline virtual Map<String, Map<String, String>>::const_iterator FirstUnsafe() const noexcept(true)
			{ 
				return _iniData->data.begin(); 
			}
			[[nodiscard]] inline virtual Map<String, Map<String, String>>::const_iterator LastUnsafe() const noexcept(true)
			{
				return _iniData->data.end();
			}

			[[nodiscard]] virtual Map<String, Map<String, String>>::const_iterator ReadSection(const char* section) const noexcept(true);
			[[nodiscard]] virtual bool ReadBool(const char* section, const char* option, bool def) const noexcept(true);
			[[nodiscard]] virtual char ReadChar(const char* section, const char* option, char def) const noexcept(true);
			[[nodiscard]] virtual long ReadInt(const char* section, const char* option, long def) const noexcept(true);
			[[nodiscard]] virtual unsigned long ReadUInt(const char* section, const char* option, unsigned long def) const noexcept(true);
			[[nodiscard]] virtual unsigned long ReadHex(const char* section, const char* option, unsigned long def) const noexcept(true);
			[[nodiscard]] virtual float ReadFloat(const char* section, const char* option, float def) const noexcept(true);
			[[nodiscard]] virtual color_value ReadRgbColor(const char* section, const char* option, color_value def) const noexcept(true);
			[[nodiscard]] virtual color_value ReadRgbaColor(const char* section, const char* option, color_value def) const noexcept(true);
			// Для чтения ansi строк
			[[nodiscard]] virtual String ReadString(const char* section, const char* option, const char* def) const noexcept(true);
			// Для чтения utf8 строк
			[[nodiscard]] virtual WideString ReadUnicodeString(const char* section, const char* option, const wchar_t* def) const noexcept(true);

			virtual void WriteBool(const char* section, const char* option, bool value) noexcept(true);
			virtual void WriteChar(const char* section, const char* option, char value) noexcept(true);
			virtual void WriteInt(const char* section, const char* option, long value) noexcept(true);
			virtual void WriteUInt(const char* section, const char* option, unsigned long value) noexcept(true);
			virtual void WriteHex(const char* section, const char* option, unsigned long value) noexcept(true);
			virtual void WriteFloat(const char* section, const char* option, float value) noexcept(true);
			virtual void WriteRgbColor(const char* section, const char* option, color_value def) noexcept(true);
			virtual void WriteRgbaColor(const char* section, const char* option, color_value def) noexcept(true);
			// Для записи ansi строк
			virtual void WriteString(const char* section, const char* option, const char* value) noexcept(true);
			// Для записи utf8 строк
			virtual void WriteUnicodeString(const char* section, const char* option, const wchar_t* value) noexcept(true);
		protected:
			virtual bool DoOpen(const wchar_t* filename) noexcept(true);
			virtual bool DoOpenSerialization(const char* text, size_t len) noexcept(true);
			virtual bool DoSave() const noexcept(true);
		};
	}

	inline bool _READ_OPTION_BOOL(const char* section, const char* option, bool def)
	{
		return Core::GlobalCKPEConfigPtr->ReadBool(section, option, def);
	}

	inline char _READ_OPTION_CHAR(const char* section, const char* option, char def)
	{
		return Core::GlobalCKPEConfigPtr->ReadChar(section, option, def);
	}

	inline long _READ_OPTION_INT(const char* section, const char* option, long def)
	{
		return Core::GlobalCKPEConfigPtr->ReadInt(section, option, def);
	}

	inline unsigned long _READ_OPTION_UINT(const char* section, const char* option, unsigned long def)
	{
		return Core::GlobalCKPEConfigPtr->ReadUInt(section, option, def);
	}

	inline unsigned long _READ_OPTION_HEX(const char* section, const char* option, unsigned long def)
	{
		return Core::GlobalCKPEConfigPtr->ReadHex(section, option, def);
	}

	inline float _READ_OPTION_FLOAT(const char* section, const char* option, float def)
	{
		return Core::GlobalCKPEConfigPtr->ReadFloat(section, option, def);
	}

	inline Core::CustomSettingCollection::color_value _READ_OPTION_RGB(const char* section, const char* option, 
		Core::CustomSettingCollection::color_value def)
	{
		return Core::GlobalCKPEConfigPtr->ReadRgbColor(section, option, def);
	}

	inline Core::CustomSettingCollection::color_value _READ_OPTION_RGBA(const char* section, const char* option, 
		Core::CustomSettingCollection::color_value def)
	{
		return Core::GlobalCKPEConfigPtr->ReadRgbaColor(section, option, def);
	}

	inline String _READ_OPTION_STR(const char* section, const char* option, const char* def)
	{
		return Core::GlobalCKPEConfigPtr->ReadString(section, option, def);
	}

	inline WideString _READ_OPTION_USTR(const char* section, const char* option, const wchar_t* def)
	{
		return Core::GlobalCKPEConfigPtr->ReadUnicodeString(section, option, def);
	}

	inline void _WRITE_OPTION_BOOL(const char* section, const char* option, bool value)
	{
		Core::GlobalCKPEConfigPtr->WriteBool(section, option, value);
	}

	inline void _WRITE_OPTION_CHAR(const char* section, const char* option, char value)
	{
		Core::GlobalCKPEConfigPtr->WriteChar(section, option, value);
	}

	inline void _WRITE_OPTION_INT(const char* section, const char* option, long value)
	{
		Core::GlobalCKPEConfigPtr->WriteInt(section, option, value);
	}

	inline void _WRITE_OPTION_UINT(const char* section, const char* option, unsigned long value)
	{
		Core::GlobalCKPEConfigPtr->WriteUInt(section, option, value);
	}

	inline void _WRITE_OPTION_HEX(const char* section, const char* option, unsigned long value)
	{
		Core::GlobalCKPEConfigPtr->WriteHex(section, option, value);
	}

	inline float _WRITE_OPTION_FLOAT(const char* section, const char* option, float value)
	{
		Core::GlobalCKPEConfigPtr->WriteFloat(section, option, value);
	}

	inline void _WRITE_OPTION_RGB(const char* section, const char* option,
		Core::CustomSettingCollection::color_value value)
	{
		Core::GlobalCKPEConfigPtr->WriteRgbColor(section, option, value);
	}

	inline void _WRITE_OPTION_RGBA(const char* section, const char* option,
		Core::CustomSettingCollection::color_value value)
	{
		Core::GlobalCKPEConfigPtr->WriteRgbaColor(section, option, value);
	}

	inline void _WRITE_OPTION_STR(const char* section, const char* option, const char* value)
	{
		Core::GlobalCKPEConfigPtr->WriteString(section, option, value);
	}

	inline void _WRITE_OPTION_USTR(const char* section, const char* option, const wchar_t* value)
	{
		Core::GlobalCKPEConfigPtr->WriteUnicodeString(section, option, value);
	}
}
