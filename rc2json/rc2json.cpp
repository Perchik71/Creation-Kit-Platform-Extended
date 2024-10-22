// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <windows.h>
#include <stdint.h>

#include <map>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <iostream>

static const std::map<std::string, std::string> mapStdControlType = {
    { "LISTVIEW", "SysListView32" },
    { "TREEVIEW", "SysTreeView32" },
    { "TABVIEW", "SysTabControl32" },
    { "REBAR", "ReBarWindow32" },
    { "HOTKEY", "msctls_hotkey32" },
    { "IPADDRESS", "SysIPAddress32" },
    { "MONTHCAL", "SysMonthCal32" },
    { "STATUSBAR", "msctls_statusbar32" },
    { "PROGRESSBAR", "msctls_progress32" },
    { "ANIMATE", "SysAnimate32" },
    { "HEADER", "SysHeader32" },
    { "RICHEDIT", "RichEdit20A" },
    { "DATETIMEPICKER", "SysDateTimePick32" }
};

static std::string change_file_ext(const std::string& a_filename, const std::string& a_ext)
{
    std::string result = a_filename;
    auto it_sep = result.find_last_of('.');
    if (it_sep != std::string::npos)
        result.erase(result.begin() + it_sep, result.end());
    return result.append(a_ext);
}

inline static std::string remove_quote(const std::string& str)
{
    std::string s = str;
    if (s[0] == '"') s.erase(0, 1);
    if (s[s.length() - 1] == '"') s.erase(s.length() - 1, 1);
    return s;
}

inline static std::string& trim(std::string& str)
{
    static const char* whitespaceDelimiters = " \t\n\r\f\v,";

    str.erase(str.find_last_not_of(whitespaceDelimiters) + 1);
    str.erase(0, str.find_first_not_of(whitespaceDelimiters));

    return str;
}

inline static std::string trim(const char* s)
{
    std::string str(s);
    return trim(str);
}

static bool rc2json(const char* a_filename)
{
    std::string in_file(a_filename);
    if (!std::filesystem::exists(a_filename)) return false;

    std::string out_file(change_file_ext(a_filename, ".json"));
    if (std::filesystem::exists(out_file) && !std::filesystem::remove(out_file))
    {
        std::cout << "ERROR: can't remove old file json\n";

        return false;
    }

    // Examples .rc file:
    // 134 DIALOGEX 0, 0, 211, 47, 0
    // STYLE DS_SETFONT | DS_MODALFRAME | DS_FIXEDSYS | DS_CENTER | WS_POPUPWINDOW | WS_DLGFRAME
    //     CAPTION "Select Audio Capture Device"
    //     LANGUAGE LANG_ENGLISH, SUBLANG_ENGLISH_US
    //     FONT 8, "MS Shell Dlg"
    // {
    //     DEFPUSHBUTTON    "OK", 1, 7, 26, 50, 14
    //     PUSHBUTTON       "Cancel", 2, 154, 26, 50, 14
    //     LTEXT            "Capture Device:", 65535, 7, 9, 52, 8
    //     COMBOBOX         1011, 62, 7, 142, 49, CBS_DROPDOWNLIST | WS_VSCROLL
    // }

    // {
    //     "ExStyle": 0,
    //         "Style" : ["DS_FIXEDSYS", "WS_POPUPWINDOW", "WS_THICKFRAME", "WS_DLGFRAME"] ,
    //         "Title" : "Layers",
    //         "Width" : 326,
    //         "Height" : 540,
    //         "FontName" : "Microsoft Sans Serif",
    //         "FontSize" : 8,
    //         "FontWeight" : 400,
    //         "FontItalic" : 0,
    //         "Controls" : [
    //     { "Type": "PUSHBUTTON", "Title" : "Add selection form", "Id" : 3682, "x" : 113, "y" : 4, "Width" : 105, "Height" : 14 },
    //     { "Type": "PUSHBUTTON", "Title" : "New layer", "Id" : 1124, "x" : 4, "y" : 4, "Width" : 105, "Height" : 14 },
    //     { "Type": "EDITTEXT", "Id" : 2581, "x" : 32, "y" : 22, "Style" : ["ES_LEFT", "ES_AUTOHSCROLL"] , "Width" : 150, "Height" : 12 },
    //     { "Type": "CONTROL", "ClassName" : "STATIC", "Id" : 6086, "Style" : ["SS_BLACKFRAME", "NOT WS_VISIBLE", "NOT WS_GROUP"] , "x" : 4, "y" : 65, "Width" : 310, "Height" : 62 },
    //     { "Type": "LTEXT", "Title" : "Filter:", "x" : 8, "y" : 25, "Width" : 24, "Height" : 8 },
    //     { "Type": "PUSHBUTTON", "Title" : "+", "Id" : 5593, "x" : 190, "y" : 22, "Width" : 10, "Height" : 10 },
    //     { "Type": "PUSHBUTTON", "Title" : "-", "Id" : 5594, "x" : 218, "y" : 22, "Width" : 10, "Height" : 10 },
    //     { "Type": "CONTROL", "ClassName" : "msctls_trackbar32", "Id" : 7121, "Style" : ["0x18"] , "x" : 86, "y" : 36, "Width" : 88, "Height" : 15 },
    //     { "Type": "AUTOCHECKBUTTON", "Id" : 1007, "Title" : "Overlay Color", "x" : 7, "y" : 39, "Width" : 70, "Height" : 10 },
    //     { "Type": "AUTOCHECKBUTTON", "Id" : 41584, "Title" : "Maintain Ref Layer", "x" : 7, "y" : 52, "Width" : 76, "Height" : 10 }
    //     ]
    // }

    std::ifstream in_stream(in_file, std::ios::in);
    std::ofstream out_stream(out_file, std::ios::out);
    if (!in_stream.is_open() || !out_stream.is_open())
    {
        std::cout << "ERROR: I/O error fatal\n";

        return false;
    }

    bool need_del_sep = false;

    in_stream.seekg(0);

    uint32_t dialog_index = 0;
    in_stream >> dialog_index;

    std::string buffer;
    in_stream >> buffer;
 
    if (!in_stream.good() || _stricmp(buffer.c_str(), "DIALOGEX"))
    {
        std::cout << "ERROR: no dialog rcdata (" << buffer.c_str() << ")\n";

        return false;
    }

    struct controlinfo_tag
    {
        std::vector<std::string> style, exstyle;
        char type[100];
        char classname[100];
        char title[100];
        uint32_t id;
        uint32_t left, top, width, height;
    };

    struct dialoginfo_tag
    {
        std::vector<std::string> style, exstyle;
        char title[100];
        uint32_t width, height;
        char fontname[100];
        uint32_t fontsize, weight, italic;
        std::vector<controlinfo_tag> controls;
    } dialoginfo;
    ZeroMemory(&dialoginfo, sizeof(dialoginfo_tag));

    in_stream >> buffer >> buffer;
    in_stream >> buffer;
    dialoginfo.width = strtoul(buffer.c_str(), nullptr, 10);
    in_stream >> buffer;
    dialoginfo.height = strtoul(buffer.c_str(), nullptr, 10);
    in_stream >> buffer >> buffer;

	if (!_stricmp(buffer.c_str(), "STYLE"))
	{
		for (;;)
		{
			in_stream >> buffer;

            if (!_stricmp(buffer.c_str(), "NOT"))
            {
                in_stream >> buffer;
                dialoginfo.style.push_back((std::string("NOT ") + buffer).c_str());
            }
            else
            {
                if (strncmp(buffer.c_str(), "0x", 2))
                    dialoginfo.style.push_back(buffer.c_str());
                else
                {
                    if (strtoul(buffer.c_str(), nullptr, 16))
                        dialoginfo.style.push_back(buffer.c_str());
                }
            }

			in_stream >> buffer;

			if (buffer[0] != '|') break;
		}
	}

	if (!_stricmp(buffer.c_str(), "EXSTYLE"))
	{
		for (;;)
		{
			in_stream >> buffer;

            if (!_stricmp(buffer.c_str(), "NOT"))
            {
                in_stream >> buffer;
                dialoginfo.exstyle.push_back((std::string("NOT ") + buffer).c_str());
            }
            else
            {
                if (strncmp(buffer.c_str(), "0x", 2))
                    dialoginfo.exstyle.push_back(buffer.c_str());
                else
                {
                    if (strtoul(buffer.c_str(), nullptr, 16))
                        dialoginfo.exstyle.push_back(buffer.c_str());
                }
            }

			in_stream >> buffer;

			if (buffer[0] != '|') break;
		}
	}

    if (!_stricmp(buffer.c_str(), "CLASS"))
    {
        in_stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        in_stream >> buffer;
    }

    if (!_stricmp(buffer.c_str(), "CAPTION"))
    {
        in_stream.getline(dialoginfo.title, _ARRAYSIZE(dialoginfo.title));
        auto s = trim(dialoginfo.title);
        ZeroMemory(dialoginfo.title, _ARRAYSIZE(dialoginfo.title));
        memcpy_s(dialoginfo.title, _ARRAYSIZE(dialoginfo.title), s.c_str(), s.length());

        in_stream >> buffer;
    }

    if (!_stricmp(buffer.c_str(), "LANGUAGE"))
    {
        in_stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        in_stream >> buffer;
    }

    if (!_stricmp(buffer.c_str(), "FONT"))
    {
        dialoginfo.weight = 400;
        in_stream >> buffer;
        dialoginfo.fontsize = strtoul(buffer.c_str(), nullptr, 10);
        in_stream.getline(dialoginfo.fontname, _ARRAYSIZE(dialoginfo.fontname));
        auto s = trim(dialoginfo.fontname);
        ZeroMemory(dialoginfo.fontname, _ARRAYSIZE(dialoginfo.fontname));
        memcpy_s(dialoginfo.fontname, _ARRAYSIZE(dialoginfo.fontname), s.c_str(), s.length());
    }
    else
    {
        std::cout << "ERROR: invalid rcdata  (" << buffer.c_str() << ")\n";
        return false;
    }

    in_stream >> buffer;
    if (buffer[0] != '{')
    {
        std::cout << "ERROR: invalid rcdata  (" << buffer.c_str() << ")\n";
        return false;
    }

    controlinfo_tag controlinfo;
    in_stream >> buffer;

    for (;;)
    {
        if (buffer[0] == '}') break;

        ZeroMemory(&controlinfo, sizeof(controlinfo_tag));

        if (!_stricmp(buffer.c_str(), "CONTROL"))
        {
            strcpy_s(controlinfo.type, buffer.c_str());

            auto pos = in_stream.tellg();
            in_stream >> buffer;
            if (buffer[0] == '"')
            {
                in_stream.seekg(pos, std::ios::beg);

                char buf[100];
                in_stream.getline(buf, _ARRAYSIZE(buf), ',');
                strcpy_s(controlinfo.title, trim(buf).c_str());
      
                in_stream >> buffer;
                controlinfo.id = strtoul(buffer.c_str(), nullptr, 10);
            }
            else
                controlinfo.id = strtoul(buffer.c_str(), nullptr, 10);

            in_stream >> buffer;
            strcpy_s(controlinfo.classname, trim(buffer).c_str());

            pos = in_stream.tellg();
            in_stream >> buffer;
            if (!strncmp(buffer.c_str(), "0x", 2) || (std::isalpha(buffer[0])))
            {
                in_stream.seekg(pos, std::ios::beg);

                for (;;)
                {
                    in_stream >> buffer;
                    buffer = trim(buffer);

                    if (!_stricmp(buffer.c_str(), "NOT"))
                    {
                        in_stream >> buffer;
                        controlinfo.style.push_back((std::string("NOT ") + buffer).c_str());
                    }
                    else
                    {
                        if (strncmp(buffer.c_str(), "0x", 2))
                            controlinfo.style.push_back(buffer.c_str());
                        else
                        {
                            if (strtoul(buffer.c_str(), nullptr, 16))
                                controlinfo.style.push_back(buffer.c_str());
                        }
                    }

                    in_stream >> buffer;
                    if (buffer[0] != '|') break;
                }
            }

            controlinfo.left = strtoul(buffer.c_str(), nullptr, 10);
            in_stream >> buffer;
            controlinfo.top = strtoul(buffer.c_str(), nullptr, 10);
            in_stream >> buffer;
            controlinfo.width = strtoul(buffer.c_str(), nullptr, 10);
            in_stream >> buffer;
            controlinfo.height = strtoul(buffer.c_str(), nullptr, 10);

            auto it_sep = buffer.find_last_of(',');
            if (it_sep != std::string::npos)
            {
                for (;;)
                {
                    in_stream >> buffer;

                    if (!_stricmp(buffer.c_str(), "NOT"))
                    {
                        in_stream >> buffer;
                        controlinfo.exstyle.push_back((std::string("NOT ") + buffer).c_str());
                    }
                    else
                    {
                        if (strncmp(buffer.c_str(), "0x", 2))
                            controlinfo.exstyle.push_back(buffer.c_str());
                        else
                        {
                            if (strtoul(buffer.c_str(), nullptr, 16))
                                controlinfo.exstyle.push_back(buffer.c_str());
                        }
                    }

                    in_stream >> buffer;
                    if (buffer[0] != '|') break;
                }
            }
            else in_stream >> buffer;

            dialoginfo.controls.push_back(controlinfo);
        }
        else
        {
            strcpy_s(controlinfo.type, buffer.c_str());

            auto pos = in_stream.tellg();
            in_stream >> buffer;
            if (buffer[0] == '"')
            {
                in_stream.seekg(pos, std::ios::beg);

                char buf[100];
                in_stream.getline(buf, _ARRAYSIZE(buf), ',');
                strcpy_s(controlinfo.title, trim(buf).c_str());

                in_stream >> buffer;
                controlinfo.id = strtoul(buffer.c_str(), nullptr, 10);
            }
            else
                controlinfo.id = strtoul(buffer.c_str(), nullptr, 10);

            in_stream >> buffer;
            controlinfo.left = strtoul(buffer.c_str(), nullptr, 10);
            in_stream >> buffer;
            controlinfo.top = strtoul(buffer.c_str(), nullptr, 10);
            in_stream >> buffer;
            controlinfo.width = strtoul(buffer.c_str(), nullptr, 10);
            in_stream >> buffer;
            controlinfo.height = strtoul(buffer.c_str(), nullptr, 10);

            if (buffer.find_last_of(',') != std::string::npos)
            {
                need_del_sep = false;
                
                for (;;)
                {
                    in_stream >> buffer;

                    auto it_sep = buffer.find_last_of(',');
                    if (it_sep != std::string::npos)
                    {
                        need_del_sep = true;
                        buffer.erase(it_sep);
                    }

                    if (!_stricmp(buffer.c_str(), "NOT"))
                    {
                        in_stream >> buffer;
                        controlinfo.style.push_back((std::string("NOT ") + buffer).c_str());
                    }
                    else
                    {
                        if(strncmp(buffer.c_str(), "0x", 2))
                            controlinfo.style.push_back(buffer.c_str());
                        else
                        {
                            if (strtoul(buffer.c_str(), nullptr, 16))
                                controlinfo.style.push_back(buffer.c_str());
                        }
                    }

                    if (need_del_sep) break;

                    in_stream >> buffer;
                    if (buffer[0] != '|') break;
                }

                if (need_del_sep)
                {
                    for (;;)
                    {
                        in_stream >> buffer;

                        if (!_stricmp(buffer.c_str(), "NOT"))
                        {
                            in_stream >> buffer;
                            controlinfo.exstyle.push_back((std::string("NOT ") + buffer).c_str());
                        }
                        else
                        {
                            if (strncmp(buffer.c_str(), "0x", 2))
                                controlinfo.exstyle.push_back(buffer.c_str());
                            else
                            {
                                if (strtoul(buffer.c_str(), nullptr, 16))
                                    controlinfo.exstyle.push_back(buffer.c_str());
                            }
                        }

                        in_stream >> buffer;

                        if (buffer[0] != '|') break;
                    }
                }

                need_del_sep = false;
                dialoginfo.controls.push_back(controlinfo);
            }
            else
            {
                dialoginfo.controls.push_back(controlinfo);
                in_stream >> buffer;
            }
        }
    }

    if (buffer[0] != '}')
    {
        std::cout << "ERROR: invalid rcdata  (" << buffer.c_str() << ")\n";
        return false;
    }

    out_stream << "{\n";

    if (dialoginfo.exstyle.size() > 0)
    {
        out_stream << "\t\"ExStyle\": [";
        for (auto it_str = dialoginfo.exstyle.begin(); it_str != dialoginfo.exstyle.end(); it_str++)
        {
            out_stream << "\"" << *it_str << "\"";
            if ((it_str + 1) != dialoginfo.exstyle.end()) out_stream << ", ";
        }
        out_stream << "],\n";
    }

    if (dialoginfo.style.size() > 0)
    {
        out_stream << "\t\"Style\": [";
        for (auto it_str = dialoginfo.style.begin(); it_str != dialoginfo.style.end(); it_str++)
        {
            out_stream << "\"" << *it_str << "\"";
            if ((it_str + 1) != dialoginfo.style.end()) out_stream << ", ";
        }
        out_stream << "],\n";
    }

    out_stream << "\t\"Title\": " << dialoginfo.title << "\n";
    out_stream << "\t\"Width\": " << dialoginfo.width << "\n";
    out_stream << "\t\"Height\": " << dialoginfo.height << "\n";
    out_stream << "\t\"FontName\": " << dialoginfo.fontname << "\n";
    out_stream << "\t\"FontSize\": " << dialoginfo.fontsize << "\n";
    out_stream << "\t\"FontWeight\": " << dialoginfo.weight << "\n";
    out_stream << "\t\"FontItalic\": " << dialoginfo.italic << "\n";

    if (dialoginfo.controls.size() > 0)
    {
        out_stream << "\t\"Controls\": [\n";
        for (auto it_str = dialoginfo.controls.begin(); it_str != dialoginfo.controls.end(); it_str++)
        {
            if (!_stricmp(it_str->type, "CONTROL"))
            {
                for (auto it_ctr = mapStdControlType.begin(); it_ctr != mapStdControlType.end(); it_ctr++)
                {
                    if (!_stricmp(it_ctr->second.c_str(), remove_quote(it_str->classname).c_str()))
                    {
                        strcpy_s(it_str->type, it_ctr->first.c_str());
                        goto scontrol;
                    }
                }

                //{ "ClassName": "SysListView32", "Title" : "List1", "Style" : ["LVS_SINGLESEL", "LVS_REPORT"] , "ExStyle" : ["LVS_EX_GRIDLINES"] , "x" : 10, "y" : 10, "Width" : 400, "Height" : 300 },

                out_stream << "\t\t{ \"ClassName\": " << it_str->classname;

                if (it_str->title[0] != '\0')
                    out_stream << ", \"Title\": " << it_str->title;

                if (it_str->style.size() > 0)
                {
                    out_stream << ", \"Style\": [";

                    for (auto it_str2 = it_str->style.begin(); it_str2 != it_str->style.end(); it_str2++)
                    {
                        out_stream << "\"" << *it_str2 << "\"";
                        if ((it_str2 + 1) != it_str->style.end()) out_stream << ", ";
                    }

                    out_stream << "]";
                }

                if (it_str->exstyle.size() > 0)
                {
                    out_stream << ", \"ExStyle\": [";

                    for (auto it_str2 = it_str->exstyle.begin(); it_str2 != it_str->exstyle.end(); it_str2++)
                    {
                        out_stream << "\"" << *it_str2 << "\"";
                        if ((it_str2 + 1) != it_str->exstyle.end()) out_stream << ", ";
                    }

                    out_stream << "]";
                }

                if (it_str->id != 0xFFFF)
                    out_stream << ", \"Id\": " << it_str->id;

                out_stream << ", \"x\": " << it_str->left;
                out_stream << ", \"y\": " << it_str->top;
                out_stream << ", \"Width\": " << it_str->width;
                out_stream << ", \"Height\": " << it_str->height << " }";
            }
            else
            {
                scontrol:

                // { "Type": "EDITTEXT", "Style": [ "ES_AUTOHSCROLL" ], "Id": 5500, "Title": "", "x": 58, "y": 5, "Width": 176, "Height": 11 },

                out_stream << "\t\t{ \"Type\": \"" << it_str->type << "\"";
                    
                if (it_str->title[0] != '\0')
                    out_stream << ", \"Title\": " << it_str->title;
                
                if (it_str->style.size() > 0)
                {
                    out_stream << ", \"Style\": [";

                    for (auto it_str2 = it_str->style.begin(); it_str2 != it_str->style.end(); it_str2++)
                    {
                        out_stream << "\"" << *it_str2 << "\"";
                        if ((it_str2 + 1) != it_str->style.end()) out_stream << ", ";
                    }

                    out_stream << "]";
                }
                
                if (it_str->exstyle.size() > 0)
                {
                    out_stream << ", \"ExStyle\": [";

                    for (auto it_str2 = it_str->exstyle.begin(); it_str2 != it_str->exstyle.end(); it_str2++)
                    {
                        out_stream << "\"" << *it_str2 << "\"";
                        if ((it_str2 + 1) != it_str->exstyle.end()) out_stream << ", ";
                    }

                    out_stream << "]";
                }

                if (it_str->id != 0xFFFF)
                    out_stream << ", \"Id\": " << it_str->id;

                out_stream << ", \"x\": " << it_str->left;
                out_stream << ", \"y\": " << it_str->top;
                out_stream << ", \"Width\": " << it_str->width;
                out_stream << ", \"Height\": " << it_str->height << " }";
            }

            if ((it_str + 1) != dialoginfo.controls.end()) out_stream << ",\n";
            else out_stream << "\n";
        }
        out_stream << "\t]\n";
    }

    out_stream << "}\n";

    return true;
}

static bool get_file_version(const char* a_filename, char* a_buffer, size_t a_buffersize)
{
	uint32_t dwSize = 0;
	VS_FIXEDFILEINFO* pFileInfo = nullptr;
	uint32_t puLenFileInfo = 0;

	dwSize = GetFileVersionInfoSizeA(a_filename, nullptr);
	if (dwSize == 0)
		return false;

	auto pbVersionInfo = std::make_unique<uint8_t[]>(dwSize);
	if (!pbVersionInfo)
		return false;

	if (!GetFileVersionInfoA(a_filename, 0, dwSize, pbVersionInfo.get()))
		return false;

	if (!VerQueryValueA(pbVersionInfo.get(), "\\", (LPVOID*)&pFileInfo, &puLenFileInfo))
		return false;

	sprintf_s(a_buffer, a_buffersize, "n%u.%u-%u", (pFileInfo->dwFileVersionMS >> 16) & 0xFFFF,
		(pFileInfo->dwFileVersionMS) & 0xFFFF, pFileInfo->dwFileVersionLS);
	
	return true;
}

static void hello()
{
    char modulePath[MAX_PATH];
    GetModuleFileNameA((HMODULE)GetModuleHandleA(nullptr), modulePath, MAX_PATH);

    char szBuffer[100];
    if (get_file_version(modulePath, szBuffer, _ARRAYSIZE(szBuffer)))
    {
        std::cout << "rc2json version " << szBuffer << " copyright (c) 2024 the CKPE developers.\n";
        std::cout << "converts RCDATA of the dialog to json for jDialog, support only ANSI codepage.\n\n\n";
    }
}

static void example()
{
    std::cout << "usage: rc2json [infile]...\n";
}

static void run_unsafe(int a_argc, char* a_argv[])
{
    for (int i = 1; i < a_argc; ++i)
        std::cout << (rc2json(a_argv[i]) ? "[SUCCEEDED] " : "[FAILED] ") << "convert file \"" << a_argv[i] <<
        "\" to \"" << change_file_ext(a_argv[i], ".json").c_str() << "\".\n";
}

int main(int a_argc, char* a_argv[])
{
    if (a_argc == 1)
    {
        hello();
        example();

        return 0;
    }

    hello();

    __try
    {
        run_unsafe(a_argc, a_argv);

        return 0;
    }
    __except (1)
    {
        std::cout << "FATAL ERROR!!!\n";
        return -1;
    }
}