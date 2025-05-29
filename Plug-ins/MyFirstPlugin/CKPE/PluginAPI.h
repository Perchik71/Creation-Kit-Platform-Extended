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

#pragma once

constexpr static uint32_t PLUGINAPI_CURRENT_VERSION = 4;

#ifndef _CKPE_MAIN

namespace CreationKitPlatformExtended
{
	namespace PluginAPI
	{
		struct Section
		{
			uintptr_t base;
			uintptr_t end;
		};

		struct OsVersion
		{
			uint32_t MajorVersion;
			uint32_t MinorVersion;
			uint32_t BuildNubmer;
		};

		enum EDITOR_EXECUTABLE_TYPE {
			EDITOR_UNKNOWN,
			EDITOR_SKYRIM_SE_1_5_3,
			EDITOR_SKYRIM_SE_1_5_73,
			EDITOR_SKYRIM_SE_1_6_438,
			EDITOR_SKYRIM_SE_1_6_1130,
			EDITOR_SKYRIM_SE_1_6_1378_1,
			EDITOR_SKYRIM_SE_LAST = EDITOR_SKYRIM_SE_1_6_1378_1,
			EDITOR_FALLOUT_C4_1_10_162_0,
			EDITOR_FALLOUT_C4_1_10_943_1,
			EDITOR_FALLOUT_C4_1_10_982_3,
			EDITOR_FALLOUT_C4_LAST = EDITOR_FALLOUT_C4_1_10_982_3,
			EDITOR_STARFIELD_1_13_61_0,
			EDITOR_STARFIELD_1_14_70_0,
			EDITOR_STARFIELD_1_14_74_0,
			EDITOR_STARFIELD_1_14_78_0,
			EDITOR_STARFIELD_1_15_216_0,
			EDITOR_STARFIELD_LAST = EDITOR_STARFIELD_1_15_216_0,
		};

		typedef int32_t IResult;

		constexpr static uint32_t SECTION_TEXT = 0;
		constexpr static uint32_t SECTION_DATA_READONLY = 1;
		constexpr static uint32_t SECTION_DATA = 2;

		typedef void(__stdcall* OnMenuItemEvent)(uint32_t u32MenuID);
		bool __stdcall AppendMenuItem(HMENU hMenu, uint32_t u32MenuID,
			String Caption, OnMenuItemEvent Function);
		bool __stdcall AppendMenuSeperator(HMENU hMenu, uint32_t u32MenuID);

		static inline bool __stdcall IsEditorFallout4(EDITOR_EXECUTABLE_TYPE editor)
		{
			return (editor >= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_1_10_162_0) && (editor <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST);
		}

		static inline bool __stdcall IsEditorSkyrimSpecialEdition(EDITOR_EXECUTABLE_TYPE editor)
		{
			return editor <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
		}

		static inline bool __stdcall IsEditorStarfield(EDITOR_EXECUTABLE_TYPE editor)
		{
			return (editor >= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_1_13_61_0) && (editor <= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_LAST);
		}

		static inline bool __stdcall CheckRuntimePlatformVersion(const char* platform_ver, 
			uint32_t major_rv, uint32_t minor_rv, uint32_t bug_fixes, uint32_t build)
		{
			uint32_t ver[4] = { 0 };
			if (sscanf(platform_ver, "%u.%u.%u.%u", &ver[0], &ver[1], &ver[2], &ver[3]) != 4)
				return false;

			return (ver[0] >= major_rv) && (ver[1] >= minor_rv) && (ver[2] >= bug_fixes) && (ver[3] >= build);
		}

		class MemoryManager
		{
		public:
			virtual void* MemAlloc(size_t size, size_t alignment = 0, bool aligned = false, bool zeroed = false);
			virtual void MemFree(void* block);
			virtual size_t MemSize(void* block);

			inline static MemoryManager* Instance;
		};

		class DialogManager
		{
		public:
			virtual bool HasDialog(const LONG_PTR uid) const;
			virtual bool AddDialog(const String& json_file, const ULONG_PTR uid);
			virtual bool AddDialogByCode(const String& json_code, const ULONG_PTR uid);
			virtual void* GetDialog(const ULONG_PTR uid);
			virtual bool Empty() const;

			virtual void LoadFromFilePackage(const char* fname);
			virtual void PackToFilePackage(const char* fname, const char* dir);

			inline static DialogManager* Instance;
		};

		class Engine
		{
		public:
			virtual uintptr_t GetModuleBase() const;
			virtual uint64_t GetModuleSize() const;
			virtual EDITOR_EXECUTABLE_TYPE GetEditorVersion() const;
			virtual bool HasAVX2() const;
			virtual bool HasSSE41() const;
			virtual bool HasCommandRun() const;
			// Indexes: SECTION_TEXT, SECTION_DATA_READONLY, SECTION_DATA
			virtual Section GetSection(uint32_t nIndex) const;
			virtual OsVersion GetSystemVersion() const;
			virtual bool HasPatch(const char* lpstrName) const;
			virtual bool HasPlugin(const char* lpstrName) const;
			virtual bool HasHyperThreads() const noexcept;
			virtual unsigned char GetTotalThreadsProcessor() const noexcept;
			virtual unsigned char GetTotalLogicalCores() const noexcept;
			virtual unsigned char GetTotalPhysicalCores() const noexcept;

			inline static Engine* Instance;
		};

		class Relocator
		{
		public:
			virtual uintptr_t Rav2Off(uintptr_t rav) const;

			virtual void Patch(uintptr_t rav, uint8_t * bytes, uint32_t size) const;
			virtual void Patch(uintptr_t rav, std::initializer_list<uint8_t> data) const;
			virtual void PatchNop(uintptr_t rav, uint32_t size) const;
			virtual void PatchJump(uintptr_t rav_from, uintptr_t rav_to) const;
			virtual void PatchCall(uintptr_t rav_from, uintptr_t rav_to) const;
			virtual void PatchMovFromRax(uintptr_t rav_from, uintptr_t rav_to) const;

			virtual bool DetourJump(uintptr_t rav, uintptr_t function) const;
			virtual bool DetourCall(uintptr_t rav, uintptr_t function) const;

			template<class T> bool DetourJump(uintptr_t rav, T function) const
			{
				return DetourJump(rav, *(uintptr_t*)&function);
			}

			template<class T> bool DetourCall(uintptr_t rav, T function) const
			{
				return DetourCall(rav, *(uintptr_t*)&function);
			}

			virtual uintptr_t DetourFunction(uintptr_t rav, uintptr_t function) const;

			template<class T> uintptr_t DetourFunctionClass(uintptr_t rav, T function) const
			{
				return DetourFunction(rav, *(uintptr_t*)&function);
			}

			virtual void Unlock();
			virtual void Unlock(uintptr_t base, uint64_t size);
			virtual void Lock(uintptr_t base, uint64_t size);
			virtual bool IsLock(uintptr_t base, uint64_t size) const;
		};

		class ConsoleWindow
		{
		public:
			virtual void InputLog(const char* Format, ...);
			virtual void InputLogVa(const char* Format, va_list Va);

			inline static ConsoleWindow* Instance;
		};

		enum DebugLogMessageLevel
		{
			vmlFatalError = 0,
			vmlError,
			vmlWarning,
			vmlMessage,
			vmlDebugMessage
		};

		class DebugLog
		{
		public:
			virtual void Message(const char* message, bool newLine = true);
			virtual void Message(const wchar_t* message, bool newLine = true);
			virtual void FormattedMessage(const char* fmt, ...);
			virtual void FormattedMessage(const wchar_t* fmt, ...);
			virtual void FormattedMessageVa(const char* fmt, va_list args);
			virtual void FormattedMessageVa(const wchar_t* fmt, va_list args);
			virtual void LogVa(DebugLogMessageLevel level, const char* fmt, va_list args);
			virtual void LogVa(DebugLogMessageLevel level, const wchar_t* fmt, va_list args);
			virtual void Log(DebugLogMessageLevel level, const char* fmt, ...);
			virtual void Log(DebugLogMessageLevel level, const wchar_t* fmt, ...);

			inline static DebugLog* Instance;
		};

		class DynamicCast
		{
		public:
			virtual bool Dump(const char* FileName) const;
			virtual void* Cast(const void* InPtr, long VfDelta,
				const char* lpstrFromType, const char* lpstrTargetType, bool isReference = false);
			virtual void* CastNoCache(const void* InPtr, long VfDelta,
				const char* lpstrFromType, const char* lpstrTargetType, bool isReference = false);

			inline static DynamicCast* Instance;
		};

		class RegistratorWindow
		{
		public:
			virtual bool Has(HWND hWnd) const;
			virtual bool HasMajor(HWND hWnd) const;	
			virtual HWND Get(uint32_t Id) const;
			virtual HWND GetMajor(const char* sName) const;
		};
	}
}

struct CKPEPlugin_StructData
{
	// The version of this structure to be defined in your plugin
	// Should be equal PLUGINAPI_CURRENT_VERSION
	uint32_t Version;

	CreationKitPlatformExtended::PluginAPI::MemoryManager* MemoryManager;
	CreationKitPlatformExtended::PluginAPI::Relocator* Relocator;
	CreationKitPlatformExtended::PluginAPI::ConsoleWindow* Console;
	CreationKitPlatformExtended::PluginAPI::DebugLog* Log;
	CreationKitPlatformExtended::PluginAPI::Engine* Engine;
	CreationKitPlatformExtended::PluginAPI::DialogManager* DialogManager;

	// Your menu, 5 indexes are reserved for each plugin
	uint32_t MenuYourStartId;
	uint32_t MenuYourEndId;
	HMENU* SubMenu;
	char szSubMenuName[33];

	// SSE or FO4 Interface (NEED CKPE RE)
	void* DataHandler;

	CreationKitPlatformExtended::PluginAPI::DynamicCast* DynamicCast;
	CreationKitPlatformExtended::PluginAPI::RegistratorWindow* RegistratorWindow;
};

#else

bool __stdcall CKPEPlugin_HasCanRuntimeDisabled();
bool __stdcall CKPEPlugin_GetName(char* szBuffer, uint32_t u32Size);
bool __stdcall CKPEPlugin_HasDependencies();
void __stdcall CKPEPlugin_GetDependencies(CreationKitPlatformExtended::Array<CreationKitPlatformExtended::String>* DependList);
bool __stdcall CKPEPlugin_GetVersion(char* szBuffer, uint32_t u32Size);
bool __stdcall CKPEPlugin_Query(CreationKitPlatformExtended::Core::EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
	const char* lpcstrPlatformRuntimeVersion);
bool __stdcall CKPEPlugin_Init(void* lpData);
bool __stdcall CKPEPlugin_Shutdown(void* lpData);

#endif // _CKPE_MAIN
