// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		// CustomSettingCollection

		CustomSettingCollection::CustomSettingCollection() : _needSaves(false)
		{}

		CustomSettingCollection::CustomSettingCollection(const char* filename) : _needSaves(false)
		{
			Open(filename);
		}

		CustomSettingCollection::CustomSettingCollection(int folderID, const char* relPath) : _needSaves(false)
		{
			OpenRelative(folderID, relPath);
		}

		CustomSettingCollection::CustomSettingCollection(const wchar_t* filename) : _needSaves(false)
		{
			Open(filename);
		}

		CustomSettingCollection::CustomSettingCollection(int folderID, const wchar_t* relPath) : _needSaves(false)
		{
			OpenRelative(folderID, relPath);
		}

		CustomSettingCollection::~CustomSettingCollection()
		{
			// --- Crashes
			//if (_needSaves)
			//	Save();
		}

		void CustomSettingCollection::Open(const char* filename) noexcept(true)
		{
			Open(Conversion::AnsiToWide(filename).c_str());
		}

		void CustomSettingCollection::Open(const wchar_t* filename) noexcept(true)
		{
			DoOpen(filename);
		}

		void CustomSettingCollection::OpenRelative(int folderID, const char* relPath) noexcept(true)
		{
			OpenRelative(folderID, Conversion::AnsiToWide(relPath).c_str());
		}

		void CustomSettingCollection::Save() noexcept(true)
		{
			DoSave();
			MarkNeedSave(false);
		}

		void CustomSettingCollection::OpenRelative(int folderID, const wchar_t* relPath) noexcept(true)
		{
			wchar_t path[MAX_PATH];
			HRESULT err = SHGetFolderPathW(NULL, folderID | CSIDL_FLAG_CREATE,
				NULL, SHGFP_TYPE_CURRENT, path);
			if (SUCCEEDED(err))
			{
				wcscat_s(path, relPath);
				PathRemoveFileSpecW(path);
				auto attr = GetFileAttributesW(path);
				if ((attr == INVALID_FILE_ATTRIBUTES) ||
					((attr & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY))
					SHCreateDirectoryExW(0, path, nullptr);
				Open(path);
			}
		}

		bool CustomSettingCollection::SplitOptionName(const char* option, String& section, String& name) noexcept(true)
		{
			if (!option)
				return false;

			String opt = option;
			if (opt.length() > 0)
			{
				auto it = opt.find_last_of(':');
				if (it != std::string::npos)
				{
					section = opt.substr(0, it);
					name = opt.substr(it + 1);
				}
				else
				{
					section = "";
					name = opt;
				}

				return true;
			}

			return false;
		}

		SettingOptionType CustomSettingCollection::GetOptionTypeByName(const char* name) noexcept(true)
		{
			if (!name)
				return sotMAX;

			switch (name[0])
			{
			case 'b': return sotBool;
			case 'c': return sotChar;
			case 'i':		 
			case 'n': return sotInteger;
			case 'u': return sotUnsignedInteger;
			case 'h': return sotHexadecimal;
			case 'f': return sotFloat;
			case 'r': return sotColorRGB;
			case 'a': return sotColorRGBA;
			case 's':		 
			case 'S': return sotString;
			default:
				return sotMAX;
			}
		}

		// TOMLSettingCollection

		TOMLSettingCollection* GlobalCKPEConfigPtr = nullptr;

		TOMLSettingCollection::TOMLSettingCollection() : CustomSettingCollection()
		{}

		TOMLSettingCollection::TOMLSettingCollection(const char* filename) : CustomSettingCollection()
		{
			Open(filename);
		}

		TOMLSettingCollection::TOMLSettingCollection(int folderID, const char* relPath) : CustomSettingCollection()
		{
			OpenRelative(folderID, relPath);
		}

		TOMLSettingCollection::TOMLSettingCollection(const wchar_t* filename) : CustomSettingCollection()
		{
			Open(filename);
		}

		TOMLSettingCollection::TOMLSettingCollection(int folderID, const wchar_t* relPath) : CustomSettingCollection()
		{
			OpenRelative(folderID, relPath);
		}

		void TOMLSettingCollection::Dump(FILE* file) const noexcept(true)
		{
			if (!IsOpen() || !file)
				return;

			auto& TOMLData = _tomlData->data.unwrap();	
			auto& TOMLSections = TOMLData.as_table();

			for (auto& TOMLSection : TOMLSections)
			{
				if (!TOMLSection.second.is_table() || !TOMLSection.second.size() || !_stricmp(TOMLSection.first.c_str(), "hotkeys"))
					continue;

				fprintf(file, "\t[%s]\n", TOMLSection.first.c_str());

				for (auto& TOMLValue : TOMLSection.second.as_table())
				{
					if (!TOMLValue.first.length())
						continue;

					switch (CustomSettingCollection::GetOptionTypeByName(TOMLValue.first.c_str()))
					{
					case sotBool:
						fprintf(file, "\t\t%s: %s\n", TOMLValue.first.c_str(), 
							(TOMLValue.second.is_boolean() ? (TOMLValue.second.as_boolean() ? "true" : "false") : "false"));
						break;
					case sotChar:
						fprintf(file, "\t\t%s: %c\n", TOMLValue.first.c_str(),
							(TOMLValue.second.is_string() ? TOMLValue.second.as_string()[0] : ' '));
						break;
					case sotInteger:
						fprintf(file, "\t\t%s: %d\n", TOMLValue.first.c_str(),
							(TOMLValue.second.is_integer() ? (long)TOMLValue.second.as_integer() : 0l));
						break;
					case sotUnsignedInteger:
						fprintf(file, "\t\t%s: %u\n", TOMLValue.first.c_str(),
							(TOMLValue.second.is_integer() ? (unsigned long)TOMLValue.second.as_integer() : 0ul));
						break;
					case sotHexadecimal:
						fprintf(file, "\t\t%s: %X\n", TOMLValue.first.c_str(),
							(TOMLValue.second.is_integer() ? (unsigned long)TOMLValue.second.as_integer() : 0ul));
						break;
					case sotFloat:
						fprintf(file, "\t\t%s: %f\n", TOMLValue.first.c_str(),
							(TOMLValue.second.is_floating() ? (float)TOMLValue.second.as_floating() : .0f));
						break;
					case sotColorRGB:
						if (TOMLValue.second.is_array() && (TOMLValue.second.size() == 3))
						{
							auto& a = TOMLValue.second.as_array();
							fprintf(file, "\t\t%s: %d, %d, %d\n", TOMLValue.first.c_str(),
								(long)a[0].as_integer(), (long)a[1].as_integer(), (long)a[2].as_integer());
						}
						else
							fprintf(file, "\t\t%s: 0, 0, 0\n", TOMLValue.first.c_str());
						break;
					case sotColorRGBA:
						if (TOMLValue.second.is_array() && (TOMLValue.second.size() == 4))
						{
							auto& a = TOMLValue.second.as_array();
							fprintf(file, "\t\t%s: %d, %d, %d, %d\n", TOMLValue.first.c_str(),
								(long)a[0].as_integer(), (long)a[1].as_integer(), (long)a[2].as_integer(), (long)a[3].as_integer());
						}
						else
							fprintf(file, "\t\t%s: 0, 0, 0, 0\n", TOMLValue.first.c_str());
						break;
					case sotString:
						fprintf(file, "\t\t%s: %s\n", TOMLValue.first.c_str(),
							(TOMLValue.second.is_string() ? TOMLValue.second.as_string().c_str() : ""));
						break;
					}
				}
			}
		}

		bool TOMLSettingCollection::IsEmpty() const noexcept(true)
		{
			return !IsOpen() && _tomlData->data.unwrap().is_empty();
		}

		bool TOMLSettingCollection::Has(const char* section, const char* option) const noexcept(true)
		{
			if (!IsOpen())
				return false;

			auto& TOMLData = _tomlData->data.unwrap();
			if (!TOMLData.contains(section))
				return false;

			auto& TOMLSection = TOMLData.at(section);
			if (!TOMLSection.contains(option))
				return false;

			return true;
		}

		bool TOMLSettingCollection::Remove(const char* section) noexcept(true)
		{
			if (!IsOpen())
				return false;

			auto& TOMLData = _tomlData->data.unwrap();
			if (!TOMLData.is_table())
				return false;

			auto& TOMLTable = TOMLData.as_table();
			auto it = TOMLTable.find(section);
			if (it == TOMLTable.end())
				return false;

			TOMLTable.erase(it);

			return true;
		}

		bool TOMLSettingCollection::Remove(const char* section, const char* option) noexcept(true)
		{
			if (!IsOpen())
				return false;

			auto& TOMLData = _tomlData->data.unwrap();
			if (!TOMLData.contains(section))
				return false;

			auto& TOMLSection = TOMLData.at(section);
			if (!TOMLSection.is_table())
				return false;

			auto& TOMLTable = TOMLSection.as_table();
			auto it = TOMLTable.find(option);
			if (it == TOMLTable.end())
				return false;

			TOMLTable.erase(it);

			return true;
		}

		bool TOMLSettingCollection::Cleanup() noexcept(true)
		{
			if (!IsOpen())
				return false;

			auto& TOMLData = _tomlData->data.unwrap();
			if (!TOMLData.is_table())
				return false;

			TOMLData.as_table().clear();

			return true;
		}

		bool TOMLSettingCollection::ReadBool(const char* section, const char* option, bool def) const noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotBool))
				return def;

			auto& TOMLData = _tomlData->data.unwrap();
			if (!TOMLData.contains(section))
				return def;

			auto& TOMLSection = TOMLData.at(section);
			if (!TOMLSection.contains(option))
				return def;

			auto& TOMLValue = TOMLSection.at(option);
			if (!TOMLValue.is_boolean())
				return def;

			return TOMLValue.as_boolean();
		}

		char TOMLSettingCollection::ReadChar(const char* section, const char* option, char def) const noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotChar))
				return def;

			auto& TOMLData = _tomlData->data.unwrap();
			if (!TOMLData.contains(section))
				return def;

			auto& TOMLSection = TOMLData.at(section);
			if (!TOMLSection.contains(option))
				return def;

			auto& TOMLValue = TOMLSection.at(option);
			if (!TOMLValue.is_string() || !TOMLValue.as_string().length())
				return def;

			return TOMLValue.as_string()[0];
		}

		long TOMLSettingCollection::ReadInt(const char* section, const char* option, long def) const noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotInteger))
				return def;

			auto& TOMLData = _tomlData->data.unwrap();
			if (!TOMLData.contains(section))
				return def;

			auto& TOMLSection = TOMLData.at(section);
			if (!TOMLSection.contains(option))
				return def;

			auto& TOMLValue = TOMLSection.at(option);
			if (!TOMLValue.is_integer())
				return def;

			return (long)TOMLValue.as_integer();
		}

		unsigned long TOMLSettingCollection::ReadUInt(const char* section, const char* option, unsigned long def) const noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotUnsignedInteger))
				return def;

			auto& TOMLData = _tomlData->data.unwrap();
			if (!TOMLData.contains(section))
				return def;

			auto& TOMLSection = TOMLData.at(section);
			if (!TOMLSection.contains(option))
				return def;

			auto& TOMLValue = TOMLSection.at(option);
			if (!TOMLValue.is_integer())
				return def;

			return (unsigned long)TOMLValue.as_integer();
		}

		unsigned long TOMLSettingCollection::ReadHex(const char* section, const char* option, unsigned long def) const noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotHexadecimal))
				return def;

			auto& TOMLData = _tomlData->data.unwrap();
			if (!TOMLData.contains(section))
				return def;

			auto& TOMLSection = TOMLData.at(section);
			if (!TOMLSection.contains(option))
				return def;

			auto& TOMLValue = const_cast<toml::value&>(TOMLSection.at(option));
			if (!TOMLValue.is_integer())
				return def;

			TOMLValue.as_integer_fmt().fmt = toml::integer_format::hex;
			return (unsigned long)TOMLValue.as_integer();
		}

		float TOMLSettingCollection::ReadFloat(const char* section, const char* option, float def) const noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotFloat))
				return def;

			auto& TOMLData = _tomlData->data.unwrap();
			if (!TOMLData.contains(section))
				return def;

			auto& TOMLSection = TOMLData.at(section);
			if (!TOMLSection.contains(option))
				return def;

			auto& TOMLValue = TOMLSection.at(option);
			if (!TOMLValue.is_floating())
				return def;

			return (float)TOMLValue.as_floating();
		}

		TOMLSettingCollection::color_value TOMLSettingCollection::ReadRgbColor(const char* section, const char* option, color_value def) const noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotColorRGB))
				return def;

			auto& TOMLData = _tomlData->data.unwrap();
			if (!TOMLData.contains(section))
				return def;

			auto& TOMLSection = TOMLData.at(section);
			if (!TOMLSection.contains(option))
				return def;

			auto& TOMLValue = TOMLSection.at(option);
			if (!TOMLValue.is_array())
				return def;

			auto& TOMLArray = TOMLValue.as_array();
			if((TOMLArray.size() != 3) || !TOMLArray[0].is_integer() || !TOMLArray[1].is_integer() || !TOMLArray[2].is_integer())
				return def;

			color_value c;
			c.r = (uint8_t)std::max(0ll, std::min(TOMLArray[0].as_integer(), 255ll));
			c.g = (uint8_t)std::max(0ll, std::min(TOMLArray[1].as_integer(), 255ll));
			c.b = (uint8_t)std::max(0ll, std::min(TOMLArray[2].as_integer(), 255ll));
			c.a = (uint8_t)255;
			return c;
		}

		TOMLSettingCollection::color_value TOMLSettingCollection::ReadRgbaColor(const char* section, const char* option, color_value def) const noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotColorRGBA))
				return def;

			auto& TOMLData = _tomlData->data.unwrap();
			if (!TOMLData.contains(section))
				return def;

			auto& TOMLSection = TOMLData.at(section);
			if (!TOMLSection.contains(option))
				return def;

			auto& TOMLValue = TOMLSection.at(option);
			if (!TOMLValue.is_array())
				return def;

			auto& TOMLArray = TOMLValue.as_array();
			if ((TOMLArray.size() != 4) || !TOMLArray[0].is_integer() || !TOMLArray[1].is_integer() || !TOMLArray[2].is_integer() || 
				!TOMLArray[3].is_integer())
				return def;

			color_value c;
			c.r = (uint8_t)std::max(0ll, std::min(TOMLArray[0].as_integer(), 255ll));
			c.g = (uint8_t)std::max(0ll, std::min(TOMLArray[1].as_integer(), 255ll));
			c.b = (uint8_t)std::max(0ll, std::min(TOMLArray[2].as_integer(), 255ll));
			c.a = (uint8_t)std::max(0ll, std::min(TOMLArray[3].as_integer(), 255ll));
			return c;
		}
		
		String TOMLSettingCollection::ReadString(const char* section, const char* option, const char* def) const noexcept(true)
		{
			if (!IsOpen() /*|| (GetOptionTypeByName(option) != sotString)*/)
				return def;

			auto& TOMLData = _tomlData->data.unwrap();
			if (!TOMLData.contains(section))
				return def;

			auto& TOMLSection = TOMLData.at(section);
			if (!TOMLSection.contains(option))
				return def;

			auto& TOMLValue = TOMLSection.at(option);
			if (!TOMLValue.is_string())
				return def;

			return TOMLValue.as_string().c_str();
		}
		
		WideString TOMLSettingCollection::ReadUnicodeString(const char* section, const char* option, const wchar_t* def) const noexcept(true)
		{
			if (!IsOpen()/* || (GetOptionTypeByName(option) != sotString)*/)
				return def;

			auto& TOMLData = _tomlData->data.unwrap();
			if (!TOMLData.contains(section))
				return def;

			auto& TOMLSection = TOMLData.at(section);
			if (!TOMLSection.contains(option))
				return def;

			auto& TOMLValue = TOMLSection.at(option);
			if (!TOMLValue.is_string())
				return def;

			return Conversion::Utf8ToUtf16(TOMLValue.as_string().c_str());
		}

		void TOMLSettingCollection::WriteBool(const char* section, const char* option, bool value) noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotBool))
				return;

			bool error = false;
			auto& TOMLData = _tomlData->data.unwrap();
			if (TOMLData.is_table())
			{
			find_section:
				if (TOMLData.contains(section))
				{
					auto& TOMLSection = TOMLData[section];
					if (TOMLSection.is_table())
					{
						if (TOMLSection.contains(option))
							TOMLSection[option] = value;
						else
						{
							toml::value v(value);
							TOMLSection.as_table().insert(std::make_pair(option, std::move(v)));
						}
					}
					else return;
				}
				else
				{
					if (error) return;
					error = true;
					toml::value v(toml::table{});
					TOMLData.as_table().insert(std::make_pair(option, std::move(v)));
					goto find_section;
				}
			}
			else return;

			MarkNeedSave();
		}

		void TOMLSettingCollection::WriteChar(const char* section, const char* option, char value) noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotChar))
				return;

			bool error = false;
			auto& TOMLData = _tomlData->data.unwrap();
			if (TOMLData.is_table())
			{
			find_section:
				if (TOMLData.contains(section))
				{
					auto& TOMLSection = TOMLData[section];
					if (TOMLSection.is_table())
					{
						char ch[2] = { value, '\0' };

						if (TOMLSection.contains(option))
							TOMLSection[option] = ch;
						else
						{
							toml::value v(ch);
							TOMLSection.as_table().insert(std::make_pair(option, std::move(v)));
						}
					}
					else return;
				}
				else
				{
					if (error) return;
					error = true;
					toml::value v(toml::table{});
					TOMLData.as_table().insert(std::make_pair(option, std::move(v)));
					goto find_section;
				}
			}
			else return;

			MarkNeedSave();
		}

		void TOMLSettingCollection::WriteInt(const char* section, const char* option, long value) noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotInteger))
				return;

			bool error = false;
			auto& TOMLData = _tomlData->data.unwrap();
			if (TOMLData.is_table())
			{
			find_section:
				if (TOMLData.contains(section))
				{
					auto& TOMLSection = TOMLData[section];
					if (TOMLSection.is_table())
					{
						if (TOMLSection.contains(option))
							TOMLSection[option] = value;
						else
						{
							toml::value v(value);
							TOMLSection.as_table().insert(std::make_pair(option, std::move(v)));
						}
					}
					else return;
				}
				else
				{
					if (error) return;
					error = true;
					toml::value v(toml::table{});
					TOMLData.as_table().insert(std::make_pair(option, std::move(v)));
					goto find_section;
				}
			}
			else return;

			MarkNeedSave();
		}

		void TOMLSettingCollection::WriteUInt(const char* section, const char* option, unsigned long value) noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotUnsignedInteger))
				return;

			bool error = false;
			auto& TOMLData = _tomlData->data.unwrap();
			if (TOMLData.is_table())
			{
			find_section:
				if (TOMLData.contains(section))
				{
					auto& TOMLSection = TOMLData[section];
					if (TOMLSection.is_table())
					{
						if (TOMLSection.contains(option))
							TOMLSection[option] = value;
						else
						{
							toml::value v(value);
							TOMLSection.as_table().insert(std::make_pair(option, std::move(v)));
						}
					}
					else return;
				}
				else
				{
					if (error) return;
					error = true;
					toml::value v(toml::table{});
					TOMLData.as_table().insert(std::make_pair(option, std::move(v)));
					goto find_section;
				}
			}
			else return;

			MarkNeedSave();
		}

		void TOMLSettingCollection::WriteHex(const char* section, const char* option, unsigned long value) noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotHexadecimal))
				return;

			bool error = false;
			auto& TOMLData = _tomlData->data.unwrap();
			if (TOMLData.is_table())
			{
			find_section:
				if (TOMLData.contains(section))
				{
					auto& TOMLSection = TOMLData[section];
					if (TOMLSection.is_table())
					{
						if (TOMLSection.contains(option))
						{
							auto& TOMLValue = TOMLData[section][option];
							TOMLValue.as_integer_fmt().fmt = toml::integer_format::hex;
							TOMLValue.as_integer() = value;
						}
						else
						{
							toml::value v(value);
							v.as_integer_fmt().fmt = toml::integer_format::hex;
							v.as_integer() = value;

							TOMLSection.as_table().insert(std::make_pair(option, std::move(v)));
						}
					}
					else return;
				}
				else
				{
					if (error) return;
					error = true;
					toml::value v(toml::table{});
					TOMLData.as_table().insert(std::make_pair(option, std::move(v)));
					goto find_section;
				}
			}
			else return;

			MarkNeedSave();
		}

		void TOMLSettingCollection::WriteFloat(const char* section, const char* option, float value) noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotFloat))
				return;

			bool error = false;
			auto& TOMLData = _tomlData->data.unwrap();
			if (TOMLData.is_table())
			{
			find_section:
				if (TOMLData.contains(section))
				{
					auto& TOMLSection = TOMLData[section];
					if (TOMLSection.is_table())
					{
						if (TOMLSection.contains(option))
							TOMLSection[option] = value;
						else
						{
							toml::value v(value);
							TOMLSection.as_table().insert(std::make_pair(option, std::move(v)));
						}
					}
					else return;
				}
				else
				{
					if (error) return;
					error = true;
					toml::value v(toml::table{});
					TOMLData.as_table().insert(std::make_pair(option, std::move(v)));
					goto find_section;
				}
			}
			else return;

			MarkNeedSave();
		}

		void TOMLSettingCollection::WriteRgbColor(const char* section, const char* option, color_value value) noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotColorRGB))
				return;

			bool error = false;
			auto& TOMLData = _tomlData->data.unwrap();
			if (TOMLData.is_table())
			{
			find_section:
				if (TOMLData.contains(section))
				{
					auto& TOMLSection = TOMLData[section];
					if (TOMLSection.is_table())
					{
						auto v = toml::array{ (unsigned long)value.r, (unsigned long)value.g, (unsigned long)value.b };
						if (TOMLSection.contains(option))
							TOMLSection[option] = std::move(v);
						else
							TOMLSection.as_table().insert(std::make_pair(option, std::move(v)));
					}
					else return;
				}
				else
				{
					if (error) return;
					error = true;
					toml::value v(toml::table{});
					TOMLData.as_table().insert(std::make_pair(option, std::move(v)));
					goto find_section;
				}
			}
			else return;
			
			MarkNeedSave();
		}

		void TOMLSettingCollection::WriteRgbaColor(const char* section, const char* option, color_value value) noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotColorRGBA))
				return;

			bool error = false;
			auto& TOMLData = _tomlData->data.unwrap();
			if (TOMLData.is_table())
			{
			find_section:
				if (TOMLData.contains(section))
				{
					auto& TOMLSection = TOMLData[section];
					if (TOMLSection.is_table())
					{
						auto v = toml::array{ (unsigned long)value.r, (unsigned long)value.g,
								(unsigned long)value.b, (unsigned long)value.a };
						if (TOMLSection.contains(option))
							TOMLSection[option] = std::move(v);
						else
							TOMLSection.as_table().insert(std::make_pair(option, std::move(v)));
					}
					else return;
				}
				else
				{
					if (error) return;
					error = true;
					toml::value v(toml::table{});
					TOMLData.as_table().insert(std::make_pair(option, std::move(v)));
					goto find_section;
				}
			}
			else return;

			MarkNeedSave();
		}

		void TOMLSettingCollection::WriteString(const char* section, const char* option, const char* value) noexcept(true)
		{
			if (!IsOpen()/* || (GetOptionTypeByName(option) != sotString)*/)
				return;

			bool error = false;
			auto& TOMLData = _tomlData->data.unwrap();
			if (TOMLData.is_table())
			{
			find_section:
				if (TOMLData.contains(section))
				{
					auto& TOMLSection = TOMLData[section];
					if (TOMLSection.is_table())
					{
						if (TOMLData.contains(option))
							TOMLSection[option] = Utils::Trim(value).c_str();
						else
						{
							toml::value v(Utils::Trim(value).c_str());
							TOMLSection.as_table().insert(std::make_pair(option, std::move(v)));
						}
					}
					else return;
				}
				else
				{
					if (error) return;
					error = true;
					toml::value v(toml::table{});
					TOMLData.as_table().insert(std::make_pair(option, std::move(v)));
					goto find_section;
				}
			}
			else return;

			MarkNeedSave();
		}

		void TOMLSettingCollection::WriteUnicodeString(const char* section, const char* option, const wchar_t* value) noexcept(true)
		{
			if (!IsOpen()/* || (GetOptionTypeByName(option) != sotString)*/)
				return;

			bool error = false;
			auto& TOMLData = _tomlData->data.unwrap();
			if (TOMLData.is_table())
			{
			find_section:
				if (TOMLData.contains(section))
				{
					auto& TOMLSection = TOMLData[section];
					if (TOMLSection.is_table())
					{
						if (TOMLData.contains(option))
							TOMLSection[option] = Utils::Trim(Conversion::Utf16ToUtf8(value).c_str()).c_str();
						else
						{
							toml::value v(Utils::Trim(Conversion::Utf16ToUtf8(value).c_str()).c_str());
							TOMLSection.as_table().insert(std::make_pair(option, std::move(v)));
						}
					}
					else return;
				}
				else
				{
					if (error) return;
					error = true;
					toml::value v(toml::table{});
					TOMLData.as_table().insert(std::make_pair(option, std::move(v)));
					goto find_section;
				}
			}
			else return;

			MarkNeedSave();
		}

		bool TOMLSettingCollection::DoOpen(const wchar_t* filename) noexcept(true)
		{
			if (!Utils::FileExists(filename))
			{
				_ERROR("Error the settings file \"%s\" no found.", Conversion::WideToAnsi(filename).c_str());
				return false;
			}

			auto file = _wfsopen(filename, L"rb", _SH_DENYWR);
			if (!file)
			{
				_ERROR("Error opening the settings file \"%s\".", Conversion::WideToAnsi(filename).c_str());
				return false;
			}

			fseek(file, 0, SEEK_END);
			auto buffSize = ftell(file);
			auto buff = std::make_unique<char[]>(buffSize + 1);
			fseek(file, 0, SEEK_SET);
			fread(buff.get(), 1, buffSize, file);
			fclose(file);

			// TOML fixed CR\LF mode
			if ((buff[(size_t)buffSize - 2] == '\r') && (buff[(size_t)buffSize - 1] == '\n'))
				buff[(size_t)buffSize - 2] = '\0';
			else
				buff[buffSize] = '\0';

			_tomlData = MakeSmartPointer<TOMLStructure>();
			_tomlData->data = toml::try_parse_str(buff.get(), toml::spec::v(1, 1, 0));
			if (!_tomlData->data.is_ok())
			{
				_ERROR("Error reading the settings file \"%s\".", Conversion::WideToAnsi(filename).c_str());

				if (_tomlData->data.is_err())
				{
					auto& err = _tomlData->data.as_err();
					for (size_t i = 0; i < err.size(); i++)
						_ERROR("\t\"%s\".", err.at(i).title().c_str());
				}

				return false;
			}

			_fileName = filename;
			_needSaves = false;

			return true;
		}

		bool TOMLSettingCollection::DoSave() const noexcept(true)
		{
			if (!IsOpen())
				return false;

			auto file = _wfopen(_fileName.c_str(), L"wb");
			if (!file)
			{
				_ERROR("Error writing the settings file \"%s\".", Conversion::WideToAnsi(_fileName).c_str());
				return false;
			}

			auto& TOMLData = _tomlData->data.unwrap();
			auto DataStr = toml::format(TOMLData, toml::spec::v(1, 1, 0));
			fwrite(DataStr.c_str(), 1, DataStr.length(), file);
			fflush(file);
			fclose(file);

			return true;
		}

		// INISettingCollection

		INISettingCollection::INISettingCollection() : CustomSettingCollection()
		{}

		INISettingCollection::INISettingCollection(const char* filename) : CustomSettingCollection()
		{
			Open(filename);
		}

		INISettingCollection::INISettingCollection(int folderID, const char* relPath) : CustomSettingCollection()
		{
			OpenRelative(folderID, relPath);
		}

		INISettingCollection::INISettingCollection(const wchar_t* filename) : CustomSettingCollection()
		{
			Open(filename);
		}

		INISettingCollection::INISettingCollection(int folderID, const wchar_t* relPath) : CustomSettingCollection()
		{
			OpenRelative(folderID, relPath);
		}

		void INISettingCollection::Dump(FILE* file) const noexcept(true)
		{
			if (!IsOpen() || !file)
				return;

			for (auto& INISection : _iniData->data)
			{
				if (!INISection.first.length() || !INISection.first.size() || !_stricmp(INISection.first.c_str(), "hotkeys"))
					continue;

				fprintf(file, "\t[%s]\n", INISection.first.c_str());

				for (auto& INIValue : INISection.second)
				{
					if (!INIValue.first.length())
						continue;

					fprintf(file, "\t\t%s: %s\n", INIValue.first.c_str(),
						(!INIValue.second.empty() ? INIValue.second.c_str() : ""));
				}
			}
		}

		bool INISettingCollection::IsEmpty() const noexcept(true)
		{
			return !IsOpen() || (!_iniData->data.size());
		}

		bool INISettingCollection::Has(const char* section, const char* option) const noexcept(true)
		{
			if (!IsOpen())
				return false;

			auto ip = ReadSection(section);
			if (ip == _iniData->data.end())
				return false;

			for (auto it = ip->second.begin(); it != ip->second.end(); it++)
			{
				if (_stricmp(it->first.c_str(), option))
					continue;

				return true;
			}

			return false;
		}

		bool INISettingCollection::Remove(const char* section) noexcept(true)
		{
			if (!IsOpen())
				return false;

			auto it = ReadSection(section);
			if (it == _iniData->data.end())
				return false;

			_iniData->data.erase(it);

			return true;
		}

		bool INISettingCollection::Remove(const char* section, const char* option) noexcept(true)
		{
			if (!IsOpen())
				return false;

			for (auto ip = _iniData->data.begin(); ip != _iniData->data.end(); ip++)
			{
				if (_stricmp(ip->first.c_str(), section))
					continue;

				for (auto it = ip->second.begin(); it != ip->second.end(); it++)
				{
					if (_stricmp(it->first.c_str(), option))
						continue;

					ip->second.erase(it);
					return true;
				}
			}

			return false;
		}

		bool INISettingCollection::Cleanup() noexcept(true)
		{
			if (!IsOpen())
				return false;

			_iniData->data.clear();

			return true;
		}

		Map<String, Map<String, String>>::const_iterator INISettingCollection::ReadSection(const char* section) const noexcept(true)
		{
			for (auto it = _iniData->data.begin(); it != _iniData->data.end(); it++)
			{
				if (_stricmp(it->first.c_str(), section))
					continue;

				return it;
			}

			return _iniData->data.end();
		}

		bool INISettingCollection::ReadBool(const char* section, const char* option, bool def) const noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotBool))
				return def;

			auto it = ReadSection(section);
			if (it == _iniData->data.end())
				return def;

			for (auto& INIValue : it->second)
			{
				if (_stricmp(INIValue.first.c_str(), option))
					continue;

				return !strcmp(INIValue.second.c_str(), "1") || !_stricmp(INIValue.second.c_str(), "true");
			}

			return def;
		}

		char INISettingCollection::ReadChar(const char* section, const char* option, char def) const noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotChar))
				return def;

			auto it = ReadSection(section);
			if (it == _iniData->data.end())
				return def;

			for (auto& INIValue : it->second)
			{
				if (_stricmp(INIValue.first.c_str(), option))
					continue;

				return INIValue.second.empty() ? ' ' : INIValue.second.c_str()[0];
			}

			return def;
		}

		long INISettingCollection::ReadInt(const char* section, const char* option, long def) const noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotInteger))
				return def;

			auto it = ReadSection(section);
			if (it == _iniData->data.end())
				return def;

			for (auto& INIValue : it->second)
			{
				if (_stricmp(INIValue.first.c_str(), option))
					continue;

				return INIValue.second.empty() ? 0l : strtol(INIValue.second.c_str(), nullptr, 10);
			}

			return def;
		}

		unsigned long INISettingCollection::ReadUInt(const char* section, const char* option, unsigned long def) const noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotUnsignedInteger))
				return def;

			auto it = ReadSection(section);
			if (it == _iniData->data.end())
				return def;

			for (auto& INIValue : it->second)
			{
				if (_stricmp(INIValue.first.c_str(), option))
					continue;

				return INIValue.second.empty() ? 0ul : strtoul(INIValue.second.c_str(), nullptr, 10);
			}

			return def;
		}

		unsigned long INISettingCollection::ReadHex(const char* section, const char* option, unsigned long def) const noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotHexadecimal))
				return def;

			auto it = ReadSection(section);
			if (it == _iniData->data.end())
				return def;

			for (auto& INIValue : it->second)
			{
				if (_stricmp(INIValue.first.c_str(), option))
					continue;

				return INIValue.second.empty() ? 0ul : strtoul(INIValue.second.c_str(), nullptr, 16);
			}

			return def;
		}

		float INISettingCollection::ReadFloat(const char* section, const char* option, float def) const noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotFloat))
				return def;

			auto it = ReadSection(section);
			if (it == _iniData->data.end())
				return def;

			for (auto& INIValue : it->second)
			{
				if (_stricmp(INIValue.first.c_str(), option))
					continue;

				return INIValue.second.empty() ? .0f : strtof(INIValue.second.c_str(), nullptr);
			}

			return def;
		}

		INISettingCollection::color_value INISettingCollection::ReadRgbColor(const char* section, const char* option, color_value def) const noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotColorRGB))
				return def;

			auto it = ReadSection(section);
			if (it == _iniData->data.end())
				return def;

			for (auto& INIValue : it->second)
			{
				if (_stricmp(INIValue.first.c_str(), option))
					continue;

				INISettingCollection::color_value c;
				if (!INIValue.second.empty())
					return c;

				uint32_t a[3]{};
				if (sscanf(INIValue.second.c_str(), "%u,%u,%u", &a[0], &a[1], &a[2]) != 3)
					return c;

				c.r = a[0];
				c.g = a[1];
				c.b = a[2];
				c.a = 255;
				return c;
			}

			return def;
		}

		INISettingCollection::color_value INISettingCollection::ReadRgbaColor(const char* section, const char* option, color_value def) const noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotColorRGBA))
				return def;

			auto it = ReadSection(section);
			if (it == _iniData->data.end())
				return def;

			for (auto& INIValue : it->second)
			{
				if (_stricmp(INIValue.first.c_str(), option))
					continue;

				INISettingCollection::color_value c;
				if (!INIValue.second.empty())
					return c;

				uint32_t a[4]{};
				if (sscanf(INIValue.second.c_str(), "%u,%u,%u,%u", &a[0], &a[1], &a[2], &a[3]) != 4)
					return c;

				c.r = a[0];
				c.g = a[1];
				c.b = a[2];
				c.a = a[3];
				return c;
			}

			return def;
		}

		String INISettingCollection::ReadString(const char* section, const char* option, const char* def) const noexcept(true)
		{
			if (!IsOpen()/* || (GetOptionTypeByName(option) != sotString)*/)
				return def;

			auto it = ReadSection(section);
			if (it == _iniData->data.end())
				return def;

			for (auto& INIValue : it->second)
			{
				if (_stricmp(INIValue.first.c_str(), option))
					continue;

				return INIValue.second.empty() ? "" : Utils::QuoteRemove(INIValue.second.c_str()).c_str();
			}

			return def;
		}

		WideString INISettingCollection::ReadUnicodeString(const char* section, const char* option, const wchar_t* def) const noexcept(true)
		{
			if (!IsOpen()/* || (GetOptionTypeByName(option) != sotString)*/)
				return def;

			auto it = ReadSection(section);
			if (it == _iniData->data.end())
				return def;

			for (auto& INIValue : it->second)
			{
				if (_stricmp(INIValue.first.c_str(), option))
					continue;

				return INIValue.second.empty() ? L"" : Conversion::Utf8ToUtf16(Utils::QuoteRemove(INIValue.second.c_str())).c_str();
			}

			return def;
		}

		void INISettingCollection::WriteBool(const char* section, const char* option, bool value) noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotBool))
				return;

			for (auto& INISection : _iniData->data)
			{
				if (_stricmp(INISection.first.c_str(), section))
					continue;

				for (auto& INIValue : INISection.second)
				{
					if (_stricmp(INIValue.first.c_str(), option))
						continue;

					INIValue.second = value ? "true" : "false";
					MarkNeedSave();
					return;
				}
			}
		}

		void INISettingCollection::WriteChar(const char* section, const char* option, char value) noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotChar))
				return;

			for (auto& INISection : _iniData->data)
			{
				if (_stricmp(INISection.first.c_str(), section))
					continue;

				for (auto& INIValue : INISection.second)
				{
					if (_stricmp(INIValue.first.c_str(), option))
						continue;

					INIValue.second.resize(1);
					INIValue.second[0] = value;
					MarkNeedSave();
					return;
				}
			}
		}

		void INISettingCollection::WriteInt(const char* section, const char* option, long value) noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotInteger))
				return;

			for (auto& INISection : _iniData->data)
			{
				if (_stricmp(INISection.first.c_str(), section))
					continue;

				for (auto& INIValue : INISection.second)
				{
					if (_stricmp(INIValue.first.c_str(), option))
						continue;

					char buff[64];
					sprintf_s(buff, "%d", value);
					INIValue.second = buff;
					MarkNeedSave();
					return;
				}
			}
		}

		void INISettingCollection::WriteUInt(const char* section, const char* option, unsigned long value) noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotUnsignedInteger))
				return;

			for (auto& INISection : _iniData->data)
			{
				if (_stricmp(INISection.first.c_str(), section))
					continue;

				for (auto& INIValue : INISection.second)
				{
					if (_stricmp(INIValue.first.c_str(), option))
						continue;

					char buff[64];
					sprintf_s(buff, "%u", value);
					INIValue.second = buff;
					MarkNeedSave();
					return;
				}
			}
		}

		void INISettingCollection::WriteHex(const char* section, const char* option, unsigned long value) noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotHexadecimal))
				return;

			for (auto& INISection : _iniData->data)
			{
				if (_stricmp(INISection.first.c_str(), section))
					continue;

				for (auto& INIValue : INISection.second)
				{
					if (_stricmp(INIValue.first.c_str(), option))
						continue;

					char buff[64];
					sprintf_s(buff, "%X", value);
					INIValue.second = buff;
					MarkNeedSave();
					return;
				}
			}
		}

		void INISettingCollection::WriteFloat(const char* section, const char* option, float value) noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotFloat))
				return;

			for (auto& INISection : _iniData->data)
			{
				if (_stricmp(INISection.first.c_str(), section))
					continue;

				for (auto& INIValue : INISection.second)
				{
					if (_stricmp(INIValue.first.c_str(), option))
						continue;

					char buff[64];
					sprintf_s(buff, "%f", value);
					INIValue.second = buff;
					MarkNeedSave();
					return;
				}
			}
		}

		void INISettingCollection::WriteRgbColor(const char* section, const char* option, color_value value) noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotColorRGB))
				return;

			for (auto& INISection : _iniData->data)
			{
				if (_stricmp(INISection.first.c_str(), section))
					continue;

				for (auto& INIValue : INISection.second)
				{
					if (_stricmp(INIValue.first.c_str(), option))
						continue;

					char buff[64];
					sprintf_s(buff, "%u,%u,%u", value.r, value.g, value.b);
					INIValue.second = buff;
					MarkNeedSave();
					return;
				}
			}
		}

		void INISettingCollection::WriteRgbaColor(const char* section, const char* option, color_value value) noexcept(true)
		{
			if (!IsOpen() || (GetOptionTypeByName(option) != sotColorRGBA))
				return;

			for (auto& INISection : _iniData->data)
			{
				if (_stricmp(INISection.first.c_str(), section))
					continue;

				for (auto& INIValue : INISection.second)
				{
					if (_stricmp(INIValue.first.c_str(), option))
						continue;

					char buff[64];
					sprintf_s(buff, "%u,%u,%u,%u", value.r, value.g, value.b, value.a);
					INIValue.second = buff;
					MarkNeedSave();
					return;
				}
			}
		}

		void INISettingCollection::WriteString(const char* section, const char* option, const char* value) noexcept(true)
		{
			if (!IsOpen()/* || (GetOptionTypeByName(option) != sotString)*/)
				return;

			for (auto& INISection : _iniData->data)
			{
				if (_stricmp(INISection.first.c_str(), section))
					continue;

				for (auto& INIValue : INISection.second)
				{
					if (_stricmp(INIValue.first.c_str(), option))
						continue;

					INIValue.second = value ? Utils::Trim(value) : "";
					MarkNeedSave();
					return;
				}
			}
		}

		void INISettingCollection::WriteUnicodeString(const char* section, const char* option, const wchar_t* value) noexcept(true)
		{
			if (!IsOpen()/* || (GetOptionTypeByName(option) != sotString)*/)
				return;

			for (auto& INISection : _iniData->data)
			{
				if (_stricmp(INISection.first.c_str(), section))
					continue;

				for (auto& INIValue : INISection.second)
				{
					if (_stricmp(INIValue.first.c_str(), option))
						continue;

					INIValue.second = value ? Conversion::Utf16ToUtf8(value) : "";
					Utils::Trim(INIValue.second);
					MarkNeedSave();
					return;
				}
			}
		}

		bool INISettingCollection::DoOpen(const wchar_t* filename) noexcept(true)
		{
			if (!Utils::FileExists(filename))
			{
				_ERROR("Error the settings file \"%s\" no found.", Conversion::WideToAnsi(filename).c_str());
				return false;
			}

			auto file = _wfsopen(filename, L"rb", _SH_DENYWR);
			if (!file)
			{
				_ERROR("Error opening the settings file \"%s\".", Conversion::WideToAnsi(filename).c_str());
				return false;
			}

			fseek(file, 0, SEEK_END);
			auto buffSize = ftell(file);
			auto buff = std::make_unique<char[]>(buffSize);
			fseek(file, 0, SEEK_SET);
			fread(buff.get(), 1, buffSize, file);
			fclose(file);

			_iniData = MakeSmartPointer<INIStructure>();
			if (!DoOpenSerialization(buff.get(), (size_t)buffSize))
			{
				_ERROR("Error reading the settings file \"%s\".", Conversion::WideToAnsi(filename).c_str());
				return false;
			}

			_fileName = filename;
			_needSaves = false;

			return true;
		}

		bool INISettingCollection::DoOpenSerialization(const char* text, size_t len) noexcept(true)
		{
			if (!text || !len)
				return false;

			String currentSection, nameOption, valueOption;

			char* next_line = nullptr, *line = strtok_s(const_cast<char*>(text), "\n", &next_line);
			do
			{
				if (!line)
					break;

				auto lineS = Utils::Trim(line);

				switch (lineS[0])
				{
				// Skips comments
				case ';':
				case '#':
					break;
				// Sections
				case '[':	
					currentSection = lineS.substr(1, lineS.length() - 2);
					break;
				default:
					auto sep = lineS.find_first_of("=:");
					if (sep == String::npos) break;
					nameOption = lineS.substr(0, sep);
					Utils::Trim(nameOption);
					valueOption = lineS.substr(sep + 1);
					Utils::Trim(valueOption);
					
					if (!currentSection.empty() && !nameOption.empty())
						_iniData->data[currentSection][nameOption] = valueOption;
					else
						_WARNING("The option has an incorrect name \"%s:%s\".",
							(nameOption.empty() ? "" : nameOption.c_str()),
							(currentSection.empty() ? "" : currentSection.c_str()));
					break;
				}
			} while (line = strtok_s(nullptr, "\n", &next_line));

			return true;
		}

		bool INISettingCollection::DoSave() const noexcept(true)
		{
			if (!IsOpen())
				return false;

			auto file = _wfopen(_fileName.c_str(), L"wb");
			if (!file)
			{
				_ERROR("Error writing the settings file \"%s\".", Conversion::WideToAnsi(_fileName).c_str());
				return false;
			}

			for (auto& INISection : _iniData->data)
			{
				if (!INISection.first.length() || !INISection.first.size())
					continue;

				fprintf(file, "[%s]\n", INISection.first.c_str());

				for (auto& INIValue : INISection.second)
				{
					if (!INIValue.first.length())
						continue;

					fprintf(file, "%s=%s\n", INIValue.first.c_str(),
						(!INIValue.second.empty() ? INIValue.second.c_str() : ""));
				}
			}
	
			fflush(file);
			fclose(file);

			return true;
		}
	}
}