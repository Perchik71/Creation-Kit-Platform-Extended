// This tutorial is about using the CKPE plugin system.
// To clarify how you can use this and how to write your plugins.
// 
// LICENSE
// This file for a test project, and serves as a guide for creating plugins.
// 
// You can also use the assets of this test project, some platform classes are passed with their interfaces 
// without implementation, so that they can be used inside the plugin, while you will not necessarily need 
// to open the code to the public. But you cannot use CKPE assets, in which case you must disclose 
// the code and indicate authorship. And also you can't re-license, only GPLv3.

#include "PluginAPI.h"

using namespace CreationKitPlatformExtended;
using namespace CreationKitPlatformExtended::PluginAPI;

// At the beginning of any plugin there should be "CKPEPlugin_".
// IMPORTANT: If the name is occupied, the plugin will be skipped.
static const char szPluginName[] = "CKPEPlugin_MyFirstPlugin";
// The version of the plugin in the form of a string, any kind, is needed to display in the log.
static const char szPluginVersion[] = "0.1";

// This example is simple, and even if unload it from the process, it will not cause problems.
// It is recommended only if this library has functions that will be used in the Creation Kit itself.
// 0 - disable, 1 - enable.
#define CKPEPLUGIN_PERMANENT_LOAD 0

#ifdef __cplusplus
extern "C"
{
#endif

__declspec(dllexport)
bool __stdcall CKPEPlugin_HasCanRuntimeDisabled()
{
    // At the moment, the platform is not supported, 
    // it is left for the future if a user interface is implemented for this.
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
    // IMPORTANT: Dependency on other plugins not supported.

    // true - yes, false - no.
    return true;
}

__declspec(dllexport)
void __stdcall CKPEPlugin_GetDependencies(Array<String>* DependList)
{
    // This function is called only if the "CKPEPlugin_HasDependencies" function returned true.

    // In this list, you enter the names of the system modulesand plugins 
    // that your plugin depends on.This means that you are explicitly using their resources.
    // You need all of them to be initializedand added to the editor.
    // If the conditions are not satisfied, your plugin will be excluded.
    
    // This is just an example, in fact, this patch is not necessary.
    *DependList = { "Console" };
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
    // lpcstrPlatformRuntimeVersion - The platform version. 
    // If you are using CKPE assets, it is important for you to check the CKPE version.
    // If you don't use them, then you can omit it.

    // This plugin supports skyrim editor 1.6.1130.
    return eEditorCurrentVersion == EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_1_6_1130;
}

__declspec(dllexport)
bool __stdcall CKPEPlugin_Init(void* lpData)
{
    // This introduction is mandatory, it is more important here what is useful to use.
    auto Data = (CKPEPlugin_StructData*)lpData;
    // This is necessary for a number of functions _ERROR, _MESSAGE, etc.
    Data->Log->Instance = Data->Log;

    ////////////////////////////////////////////////
    //
    // There is already an initialization code here
    //
    ////////////////////////////////////////////////
    //
    // You have been authorized in the platform, now you can "mess up", change the code somewhere.
    //

    _MESSAGE("Hello debug log, it's me.");

    auto SomePtr = Data->MemoryManager->MemAlloc(100);
    if (!SomePtr)
        _ERROR("I couldn't allocate memory =(");
    else
    {
        _MESSAGE("I have allocated %u bytes, I am happy, I will delete it now.", 
            Data->MemoryManager->MemSize(SomePtr));
        Data->MemoryManager->MemFree(SomePtr);
    }

    Data->Console->InputLog("Hi, console");

    // Q: What is this RVA?
    // A: This is an offset from the base address of the process. 
    // In the xdbg debugger, you can get this offset. Select the desired location and copy this offset.

    // All methods of this class use RVA - remember.
    // Exclude: IsLock, Lock, Unlock
    // To get the full address: Rva2Off

    // Here is a place that I will change for fun.
    // 00000001413AFE88 | 4C:8D05 19E1B801 | lea r8,qword ptr ds:[142F3DFA8] | 0000000142F3DFA8:" 64-bit"
    // Those who know asm understand that lea is bothering us here
    // Change to: mov r8, qword ptr ds : [0x0000000142F3DFA8]
    Data->Relocator->Patch(0x13AFE89, { 0x8B });            // 8D -> 8B it will turn out 4C:8B05 19E1B801
    // This will cause CTD as mov dereferences the pointer
    // Let's add our own pointer to the string to this pointer
    char* NewFunString = new char[50];
    sprintf_s(NewFunString, 50, "Todd: Hey, buy it CK \\('-'\\)");
    Data->Relocator->Patch(0x2F3DFA8, (uint8_t*)&NewFunString, sizeof(uintptr_t));

    // True, it means everything was successful, otherwise everything is sad, 
    // and it will not be good if the code was changed somewhere.
    return true;
}

__declspec(dllexport)
bool __stdcall CKPEPlugin_Shutdown(void* lpData)
{
    // At the moment, the platform is not supported, 
    // it is left for the future if a user interface is implemented for this.
    //
    // If there is no way to disable RunTime, then it makes no sense, 
    // the whole process will die, along with everyone else, like the Titanic.
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
#if CKPEPLUGIN_PERMANENT_LOAD
        // Load library it on a permanent basis.
        // IMPORTANT: This is important because you may have global variables, 
        // and the system may well unload this library. 
        CKPEPlugin_ForceLoadLibraryPerennially(hModule);
#endif // CKPEPLUGIN_PERMANENT_LOAD

        // Here, your code, if you need it here...
    }
    
    return TRUE;
}

