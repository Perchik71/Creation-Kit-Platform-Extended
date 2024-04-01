// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "UITheme/Colored.h"

#include "Editor API/BSString.h"
#include "Editor API/SSE/TESDataHandler.h"
#include "Editor API/FO4/TESDataHandler.h"
#include "Editor API/EditorUI.h"
#include "Editor API/BSTArray.h"

using namespace CreationKitPlatformExtended;
using namespace CreationKitPlatformExtended::PluginAPI;

const char szProjectName[] = "QTableInfos";
const char szClassWndName[] = "QTableInfosCLASS";

static uintptr_t module_base = 0;
static CKPEPlugin_StructData Platform;
static HBRUSH AppBackground;
// At the beginning of any plugin there should be "CKPEPlugin_".
// IMPORTANT: If the name is occupied, the plugin will be skipped.
static const char szPluginName[] = "CKPEPlugin_QTableInfos";
// The version of the plugin in the form of a string, any kind, is needed to display in the log.
static const char szPluginVersion[] = "0.1";
// The full path to this dll.
static char szPluginDllFileName[MAX_PATH];

#define UC_LIST 100
#define UC_EXPORT 101
#define UC_CLOSE 102

#define ERR_STATUS_OK 0
#define ERR_STATUS_NOAUDIO 1

enum GameVersionT : uint8_t
{
    SKYRIM,
    FALLOUT_4
};
static GameVersionT GameVersion;

struct ExportDialogInfoT
{
    uint32_t Status;
    EditorAPI::BSString PluginName;
    EditorAPI::BSString AudioName;
    EditorAPI::BSString VoiceType;
    EditorAPI::BSString DialogText;

    void *rcForm, *VtForm;
};

// Main window this plugin
class QPluginWindow
{
public:
    inline QPluginWindow() : hWnd(NULL), hListView(NULL), hExportBtn(NULL), hCloseBtn(NULL) {}
    inline ~QPluginWindow() { Destroy(); }

    inline void Show() const { ShowWindow(hWnd, SW_NORMAL); BringWindowToTop(hWnd); }
    inline void Hide() const { ShowWindow(hWnd, SW_HIDE); }

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    bool Create();
    void Destroy();

    void CleanData();

    static EditorAPI::BSString GenAudionName(EditorAPI::BSString& QuestEditID, EditorAPI::BSString& TopicEditID, 
        uint32_t FormID, uint32_t ResponseID);
private:
    HWND hWnd;
    HWND hListView;
    HWND hExportBtn;
    HWND hCloseBtn;
    HINSTANCE hInst;
    EditorAPI::BSTArray<ExportDialogInfoT*, 128, 128> aExportDialogInfo;

    union
    {
        EditorAPI::SkyrimSpectialEdition::TESDataHandler_CKPEIntf* DataHandler_Intf_SSE;
        EditorAPI::Fallout4::TESDataHandler_CKPEIntf* DataHandler_Intf_FO4;
    };
};
static QPluginWindow* Window = NULL;

void QPluginWindow::CleanData()
{
    if (!aExportDialogInfo.QEmpty())
    {
        for (uint32_t i = 0; i < aExportDialogInfo.QSize(); i++)
        {
            if (aExportDialogInfo[i])
                delete aExportDialogInfo[i];
        }

        aExportDialogInfo.Clear();
    }
}

EditorAPI::BSString QPluginWindow::GenAudionName(EditorAPI::BSString& QuestEditID, EditorAPI::BSString& TopicEditID,
    uint32_t FormID, uint32_t ResponseID)
{
   // EditorAPI::BSString Ret(QuestEditID, 10);
   // /(
    return "";
}

bool QPluginWindow::Create()
{
    hInst = GetModuleHandle(NULL);

    bool DarkTheme = Utils::GetProfileValue("CreationKitPlatformExtended.ini", "CreationKit", "bUIDarkTheme", false);
    uint32_t DarkThemeId = Utils::GetProfileValue("CreationKitPlatformExtended.ini", "CreationKit", "uUIDarkThemeId", 0);

    AppBackground = GetSysColorBrush(COLOR_BTNFACE);
    if (DarkTheme)
    {
        switch (DarkThemeId)
        {
        case 0:
            AppBackground = CreateSolidBrush(UITheme::GetThemeSysColor(UITheme::Theme_DarkGray, UITheme::ThemeColor_Default));
            break;
        case 1:
            AppBackground = CreateSolidBrush(UITheme::GetThemeSysColor(UITheme::Theme_Dark, UITheme::ThemeColor_Default));
            break;
        default:
            String str = Utils::Trim(Utils::GetProfileValue("CreationKitPlatformExtendedCustomTheme.ini", "Colors", 
                "rDefault", "NONE").c_str());
            if (str != "NONE")
            {
                str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
               
                uint32_t c[3] = { 0 };
                if (sscanf(str.c_str(), "%u,%u,%u", &c[0], &c[1], &c[2]) == 3)
                    AppBackground = CreateSolidBrush(RGB(c[0], c[1], c[2]));
            }
            break;
        }
    }

    WNDCLASSEX wc;
    memset(&wc, 0, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(wc.hInstance, MAKEINTRESOURCEA(318));
    wc.hIconSm = wc.hIcon;
    wc.hbrBackground = AppBackground;
    wc.lpszClassName = szClassWndName;
    wc.style = CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW;
    wc.lpfnWndProc = WndProc;

    if (!RegisterClassEx(&wc))
        return false;

    int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
    int nWidth = 1024;
    int nHeight = 728;
    int nLeft = (nScreenWidth - nWidth) >> 1;
    int nTop = (nScreenHeight - nHeight) >> 1;

    hWnd = CreateWindowEx(WS_EX_TOPMOST, wc.lpszClassName, "QTable Info's",
        WS_OVERLAPPED | WS_CAPTION | WS_BORDER | WS_SYSMENU, nLeft, nTop, 
        nWidth, nHeight, NULL, NULL, wc.hInstance, this);
    if (!hWnd)
    {
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return false;
    }

    char szStProgressBuf[60];

    if (GameVersion == SKYRIM)
    {
        DataHandler_Intf_SSE = (EditorAPI::SkyrimSpectialEdition::TESDataHandler_CKPEIntf*)Platform.DataHandler;
        auto DataHandler = DataHandler_Intf_SSE->GetSingleton();

        // So, TESDataHandler has a very strange filling, many arrays are just empty, 
        // INFO is always empty, but DIAL has a lot of part quests.
        //_MESSAGE("Total forms: DIAL <%u>", DataHandler->arrTopic.QSize());

        if (DataHandler->arrTopic.QSize())
        {
            auto MainWnd = Platform.RegistratorWindow->GetMajor("MainWindow");     
            auto StatusbarWnd = GetDlgItem(MainWnd, EditorAPI::EditorUI::UI_EDITOR_STATUSBAR);

            SendMessage(StatusbarWnd, SB_SETTEXT, 1, (LPARAM)"Load TopicInfo Forms... 0%");

            auto ProgressU = DataHandler->arrTopic.QSize() / 100;
            uint32_t ProgressC = 0, Progress = 0;
            CleanData();

            for (uint32_t i = 0; i < DataHandler->arrTopic.QSize(); i++)
            {
                auto Topic = DataHandler->arrTopic[i];
                if (Topic)
                {
                    for (uint32_t i = 0; i < Topic->GetInfoCount(); i++)
                    {
                        auto Info = Topic->GetInfoAt(i);
                        if (Info)
                        {
                            for (auto itResp = Info->GetFirstResponseIterator(); !itResp.End(); itResp++)
                            {
                                ExportDialogInfoT* esxp = new ExportDialogInfoT;
                                memset(esxp, 0, sizeof(ExportDialogInfoT));
                                



                                esxp->AudioName.Format("%08X_%u.fuz", Info->FormID, itResp->ResponseId);

                                Topic->Quest->EditorID

                                /*if (!Utils::FileExists(esxp->AudioName.c_str()))
                                {
                                    auto fileAudio = EditorAPI::BSString::Utils::ChangeFileExt(esxp->AudioName, ".wav");
                                    if (!Utils::FileExists(fileAudio.c_str()))
                                        esxp->Status = ERR_STATUS_NOAUDIO;
                                }

                                if (!Utils::FileExists(EditorAPI::BSString::Utils::ChangeFileExt(esxp->AudioName, ".lip").c_str()))
                                {
                                    auto fileAudio = ;
                                    if (!Utils::FileExists(fileAudio.c_str()))
                                        esxp->Status = ERR_STATUS_NOAUDIO;
                                }*/


                              //  esxp->DialogText = itResp->ResponseText.c_str();
                                esxp->rcForm = Info;

                                _MESSAGE("%p %s %s", esxp->rcForm, esxp->AudioName.c_str(), esxp->DialogText.c_str());

                                aExportDialogInfo.Push(esxp);
                            }
                        }
                        else
                            _WARNING("TopicInfo: Nullptr");
                    }
                }
                else
                    _WARNING("Topic: Nullptr");

                ProgressC++;

                if (ProgressC >= ProgressU)
                {
                    ProgressC = 0;
                    sprintf_s(szStProgressBuf, "Load TopicInfo Forms... %u%%", ++Progress);
                    SendMessage(StatusbarWnd, SB_SETTEXT, 1, (LPARAM)szStProgressBuf);
                }
            }

            SendMessage(StatusbarWnd, SB_SETTEXT, 1, (LPARAM)"");
        }
    }
    else
    {
        DataHandler_Intf_FO4 = (EditorAPI::Fallout4::TESDataHandler_CKPEIntf*)Platform.DataHandler;
        auto DataHandler = DataHandler_Intf_FO4->GetSingleton();

        // So, TESDataHandler has a very strange filling, many arrays are just empty, 
        // INFO is always empty, but DIAL has a lot of part quests.
        _MESSAGE("Total forms: DIAL <%u>, INFO <%u>", DataHandler->arrDIAL.QSize(), DataHandler->arrINFO.QSize());
    } 

    Show();
    return true;
}

void QPluginWindow::Destroy()
{ 
    CleanData();

    if (hListView) DestroyWindow(hListView);
    if (hExportBtn) DestroyWindow(hExportBtn);
    if (hCloseBtn) DestroyWindow(hCloseBtn);
    if (hWnd)
    {
        DestroyWindow(hWnd);
        UnregisterClass(szClassWndName, hInst);
    }

    hWnd = NULL;
    hListView = NULL;
    hExportBtn = NULL;
    hCloseBtn = NULL;
    hInst = NULL;
}

LRESULT CALLBACK QPluginWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
    {
        QPluginWindow* This = (QPluginWindow*)((LPCREATESTRUCT)lParam)->lpCreateParams;
        SetProp(hWnd, "PluginClass", (HANDLE)This);

        RECT rcWnd;
        GetClientRect(hWnd, &rcWnd);

        This->hListView = CreateWindowEx(0, WC_LISTVIEW, "",
            LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_AUTOARRANGE | WS_CLIPSIBLINGS | 
            LVS_OWNERDATA | WS_VISIBLE | WS_CHILD | WS_TABSTOP,
            4, 4, rcWnd.right - 8, rcWnd.bottom - 42, hWnd, (HMENU)UC_LIST, NULL, NULL);

        ListView_SetExtendedListViewStyleEx(This->hListView, LVS_EX_GRIDLINES, LVS_EX_GRIDLINES);
        ListView_SetExtendedListViewStyleEx(This->hListView, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
        ListView_SetExtendedListViewStyleEx(This->hListView, LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);
        
        std::string header[5] = { "State", "Plugin", "File Name", "Voice Type", "Dialog" };

        LVCOLUMN ListColumn[5];
        memset(ListColumn, 0, sizeof(LVCOLUMN) * 5);

        auto PluginPref = "CKPEPlugins\\QTableInfosPref.ini";

        ListColumn[0].mask = LVCF_TEXT | LVCF_WIDTH;
        ListColumn[0].cchTextMax = 50;
        ListColumn[0].cx = Utils::GetProfileValue(PluginPref, "General", "Info List Column 0 width", 96);
        ListColumn[0].pszText = header[0].data();
        ListColumn[0].iSubItem = 0;
        ListColumn[1].mask = LVCF_TEXT | LVCF_WIDTH;
        ListColumn[1].cchTextMax = MAX_PATH;
        ListColumn[1].cx = Utils::GetProfileValue(PluginPref, "General", "Info List Column 1 width", 180);
        ListColumn[1].pszText = header[1].data();
        ListColumn[1].iSubItem = 1;
        ListColumn[2].mask = LVCF_TEXT | LVCF_WIDTH;
        ListColumn[2].cchTextMax = MAX_PATH;
        ListColumn[2].cx = Utils::GetProfileValue(PluginPref, "General", "Info List Column 2 width", 210);
        ListColumn[2].pszText = header[2].data();
        ListColumn[2].iSubItem = 2;
        ListColumn[3].mask = LVCF_TEXT | LVCF_WIDTH;
        ListColumn[3].cchTextMax = 70;
        ListColumn[3].cx = Utils::GetProfileValue(PluginPref, "General", "Info List Column 3 width", 120);
        ListColumn[3].pszText = header[3].data();
        ListColumn[3].iSubItem = 3;
        ListColumn[4].mask = LVCF_TEXT | LVCF_WIDTH;
        ListColumn[4].cchTextMax = 300;
        ListColumn[4].cx = Utils::GetProfileValue(PluginPref, "General", "Info List Column 4 width", 300);
        ListColumn[4].pszText = header[4].data();
        ListColumn[4].iSubItem = 4;

        for (int i = 0; i < 5; i++)
            ListView_InsertColumn(This->hListView, i, &ListColumn[i]);

        This->hCloseBtn = CreateWindowEx(0, WC_BUTTON, "Export",
            WS_VISIBLE | WS_CHILD | WS_TABSTOP, rcWnd.right - 168, rcWnd.bottom - 34, 80, 30, hWnd, (HMENU)UC_EXPORT, NULL, NULL);
        This->hExportBtn = CreateWindowEx(0, WC_BUTTON, "Close",
            WS_VISIBLE | WS_CHILD | WS_TABSTOP, rcWnd.right - 84, rcWnd.bottom - 34, 80, 30, hWnd, (HMENU)UC_CLOSE, NULL, NULL);

        UpdateWindow(This->hWnd);

        return S_OK;
    }
    case WM_CLOSE:
    {
        QPluginWindow* This = (QPluginWindow*)GetProp(hWnd, "PluginClass");
        delete This;
        return S_OK;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC dc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        return S_OK;
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case UC_CLOSE:
            SendMessage(hWnd, WM_CLOSE, 0, 0);
            return S_OK;
        case UC_EXPORT:
            // nope
            return S_OK;
        }
    }
    case WM_NOTIFY:
    {
        if (((LPNMHDR)lParam)->code == HDN_ITEMCHANGED)
        {
            QPluginWindow* This = (QPluginWindow*)GetProp(hWnd, "PluginClass");
            LPNMHEADERA pNMHeader = (LPNMHEADERA)lParam;
            
            if (This)
            {
                auto ColWidth = ListView_GetColumnWidth(This->hListView, pNMHeader->iItem);
 
                EditorAPI::BSString OptionName;
                OptionName.Format("Info List Column %d width", pNMHeader->iItem);

                Utils::UpdateProfileValue("CKPEPlugins\\QTableInfosPref.ini",
                    "General", OptionName.c_str(), EditorAPI::BSString::Transforms::IntToStr(ColWidth).c_str());
            
                return S_OK;
            }
        }
    }
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void __stdcall CKPEPlugin_MenuItemHandler(uint32_t u32MenuID)
{
    Window = new QPluginWindow();
    if (Window) Window->Create();
}

bool __stdcall CKPEPlugin_CreateSubMenu(CKPEPlugin_StructData* Data)
{
    if (Data->MenuYourEndId == Data->MenuYourStartId)
        // The platform's reserve has been completely used up
        return false;

    ////////////////////////////////////
    // Creating a menu for this plugin
    ////////////////////////////////////

    // The name of the menu item itself.
    strcpy_s(Data->szSubMenuName, szProjectName);

    // Plug-in have only 5 free indexes (reserved)
    auto Menu = CreateMenu();
    _ASSERTMSG(Menu, "Failed to create plug-in submenu");
    
    // You should only use "AppendMenuItem" or "AppendMenuSeperator" function to create a menu with a function binding.
    // It is technically connected to communicate with the platform.

    // The platform and CK itself is an ANSI project, so unicode should not be used.

    _ASSERT(AppendMenuItem(Menu, Data->MenuYourStartId + 0, "Show Info's", CKPEPlugin_MenuItemHandler));

    // Send menu for CKPE
    *Data->SubMenu = Menu;

    return true;
}

bool __stdcall CKPEPlugin_Main(CKPEPlugin_StructData* Data)
{
    // ------------ Check Platform ------------ //

    if (Platform.Engine->HasCommandRun())
    {
        _MESSAGE("[PLUGIN: %s] This plugin requires default mode run.", szProjectName);
        return false;
    }

    // ------------ Init Common ------------ //

    // Create menu plugin
    if (!CKPEPlugin_CreateSubMenu(Data))
    {
        _ERROR("[PLUGIN: %s] The platform's reserve has been completely used up. And you can't create a menu anymore.", 
            szProjectName);
        return false;
    }

    // True, it means everything was successful, otherwise everything is sad, 
    // and it will not be good if the code was changed somewhere.
    return true;
}

// This example is simple, and even if unload it from the process, it will not cause problems.
// It is recommended only if this library has functions that will be used in the Creation Kit itself.
// 0 - disable, 1 - enable.
#define CKPEPLUGIN_PERMANENT_LOAD 1

#ifdef __cplusplus
extern "C"
{
#endif

__declspec(dllexport)
bool __stdcall CKPEPlugin_HasCanRuntimeDisabled()
{
    return false;
}

__declspec(dllexport)
bool __stdcall CKPEPlugin_GetName(char* szBuffer, uint32_t u32Size)
{
    // Copying a string to the CKPE buffer
    return !strncpy_s(szBuffer, u32Size, szPluginName, ARRAYSIZE(szPluginName));
}

__declspec(dllexport)
bool __stdcall CKPEPlugin_HasDependencies()
{
    // Is there a dependency on some system module.
    return true;
}

__declspec(dllexport)
void __stdcall CKPEPlugin_GetDependencies(Array<String>* DependList)
{
    *DependList = { "TESDataHandler", "Main Window" };
}

__declspec(dllexport)
bool __stdcall CKPEPlugin_GetVersion(char* szBuffer, uint32_t u32Size)
{
    // Copying a string to the CKPE buffer
    return !strncpy_s(szBuffer, u32Size, szPluginVersion, ARRAYSIZE(szPluginVersion));
}

__declspec(dllexport)
bool __stdcall CKPEPlugin_Query(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
    const char* lpcstrPlatformRuntimeVersion)
{
    if (!CheckRuntimePlatformVersion(lpcstrPlatformRuntimeVersion, 0, 2, 0, 895))
        return false;

    if (IsEditorSkyrimSpecialEdition(eEditorCurrentVersion))
        GameVersion = GameVersionT::SKYRIM;
    else if (IsEditorFallout4(eEditorCurrentVersion))
        GameVersion = GameVersionT::FALLOUT_4;
    else
        return false;

    return true;
}

__declspec(dllexport)
bool __stdcall CKPEPlugin_Init(void* lpData)
{
    // This introduction is mandatory, it is more important here what is useful to use.
    auto Data = (CKPEPlugin_StructData*)lpData;
    
    // Checking the API version
    if (Data->Version < PLUGINAPI_CURRENT_VERSION)
    {
        char* szMessage = new char[1024];
        sprintf_s(szMessage, 1024,
            "FileName: %s\n"
            "This plugin \"%s\" ver. \"%s\" was built in an excellent version of the API plugin.\n"
            "The plugin cannot be started.\n"
            "Cancel the plugin launch and continue working without it?",
            PathFindFileNameA(szPluginDllFileName), szPluginName, szPluginVersion);

        if (MessageBoxA(0, szMessage, "Error", MB_YESNO | MB_ICONERROR) == IDNO)
            TerminateProcess(GetCurrentProcess(), 1);
       
        delete[] szMessage;
        return false;
    }

    // This is necessary for a number of functions _ERROR, _MESSAGE, etc.
    Data->Log->Instance = Data->Log;
    // This is necessary for a number of functions _DYNAMIC_CAST, _DYNAMIC_CAST2.
    Data->DynamicCast->Instance = Data->DynamicCast;
    // This is necessary for a number of functions _CONSOLE, _CONSOLEVA.
    Data->Console->Instance = Data->Console;

    // Unicode plugin init
    Conversion::LazUnicodePluginInit();
    // Memory manager set global for CKPE modules
    Core::GlobalMemoryManagerPtr = Data->MemoryManager;  
    // Save data
    Platform = *Data;
    // Calling a custom main function
    return CKPEPlugin_Main(Data);
}

__declspec(dllexport)
bool __stdcall CKPEPlugin_Shutdown(void* lpData)
{
    return false;
}

#ifdef __cplusplus
} // __cplusplus defined.
#endif

#if CKPEPLUGIN_PERMANENT_LOAD

void WINAPI CKPEPlugin_ForceLoadLibraryPerennially(HMODULE hModule)
{
    HMODULE temp;
    GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_PIN | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
        (LPCSTR)hModule, &temp);
}

#endif // CKPEPLUGIN_PERMANENT_LOAD

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        // Getting the file name of this dll
        GetModuleFileNameA(hModule, szPluginDllFileName, MAX_PATH);

#if CKPEPLUGIN_PERMANENT_LOAD
        // Load library it on a permanent basis.
        // IMPORTANT: This is important because you may have global variables, 
        // and the system may well unload this library. 
        CKPEPlugin_ForceLoadLibraryPerennially(hModule);
#endif // CKPEPLUGIN_PERMANENT_LOAD

        module_base = (uintptr_t)hModule;
    }
    
    return TRUE;
}

