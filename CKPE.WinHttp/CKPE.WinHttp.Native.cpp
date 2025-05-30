// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <windows.h>
#include <CKPE.WinHttp.Native.h>
#include <CKPE.h>
#include <stdexcept>

LONG(NTAPI* NtSetInformationThread)(HANDLE ThreadHandle, LONG ThreadInformationClass,
    PVOID ThreadInformation, ULONG ThreadInformationLength);

namespace CKPE
{
    namespace WinHttp
    {
        class Runner
        {
            bool _run{ false };
            std::uintptr_t _moduleBase;
            CHAR TempNTSIT[16]{ 0 };
            ULONG_PTR TempNTSITAddress{ 0 };
        public:
            constexpr Runner() noexcept(true) = default;
            Runner(const Runner&) = delete;
            Runner operator=(const Runner&) = delete;

            [[nodiscard]] constexpr inline bool HasRun() const noexcept(true) { return _run; }

            void Run();
            void Shutdown();

            void ContinueInitialize();
            void EnableBreakpoint() noexcept(true);
            void DisableBreakpoint() const noexcept(true);
        } static Runner;

        static BOOL WINAPI hk_QueryPerformanceCounter(LARGE_INTEGER* lpPerformanceCount)
        {
            // Выключение точки останова
            Runner.DisableBreakpoint();

            // Остановка для отладчика
            __try
            {
                __debugbreak();
            }
            __except (EXCEPTION_EXECUTE_HANDLER)
            {}

            Runner.ContinueInitialize();
            return QueryPerformanceCounter(lpPerformanceCount);
        }

        static LONG NTAPI hk_NtSetInformationThread(HANDLE ThreadHandle, LONG ThreadInformationClass,
            PVOID ThreadInformation, ULONG ThreadInformationLength)
        {
            // Для Steam
            if (ThreadInformationClass == 0x11)
                return 0;

            return NtSetInformationThread(ThreadHandle, ThreadInformationClass,
                ThreadInformation, ThreadInformationLength);
        }

        void Runner::ContinueInitialize()
        {
            // TODO
        }

        void Runner::EnableBreakpoint() noexcept(true)
        {
            _MESSAGE("Module base: %llX", _moduleBase);

            auto app = Application::GetSingleton();

            //_MESSAGE("Section range \".text\": (base: %p, end: %p)", Sections[SECTION_TEXT].base, Sections[SECTION_TEXT].end);
           // _MESSAGE("Section range \".rdata\": (base: %p, end: %p)", Sections[SECTION_DATA_READONLY].base, Sections[SECTION_DATA_READONLY].end);
            //_MESSAGE("Section range \".data\": (base: %p, end: %p)", Sections[SECTION_DATA].base, Sections[SECTION_DATA].end);

            // TODO

            // Установить магическое значение, которое запускает ранний вызов QueryPerformanceCounter
            auto lc = app->GetPEDirectory(PEDirectory::e_load_config);
            *(std::uint64_t*)lc.GetPointer<IMAGE_LOAD_CONFIG_DIRECTORY>()->SecurityCookie = 0x2B992DDFA232;
            Detours::DetourIAT(_moduleBase, "kernel32.dll", "QueryPerformanceCounter", (std::uintptr_t)hk_QueryPerformanceCounter);

            // Отключить вызов распаковщика steam для NtSetInformationThread(ThreadHideFromDebugger)
            TempNTSITAddress = (std::uintptr_t)GetProcAddress(GetModuleHandle("ntdll.dll"), "NtSetInformationThread");
            if (TempNTSITAddress)
            {
                SafeWrite::Write((std::uintptr_t)&TempNTSIT, (std::uint8_t*)TempNTSITAddress, sizeof(TempNTSIT));
                *(uintptr_t*)&NtSetInformationThread = Detours::DetourClassJump(TempNTSITAddress, &hk_NtSetInformationThread);
            }
        }

        void Runner::DisableBreakpoint() const noexcept(true)
        {
            // Восстановить оригинал указатель на QPC
            Detours::DetourIAT(_moduleBase, "kernel32.dll", "QueryPerformanceCounter", (std::uintptr_t)QueryPerformanceCounter);

            if (TempNTSITAddress)
                // Восстановить исходный код NtSetInformationThread
                SafeWrite::Write((std::uintptr_t)TempNTSITAddress, (std::uint8_t*)&TempNTSIT, sizeof(TempNTSIT));
        }

        void Runner::Run()
        {
            auto sname = PathUtils::GetApplicationFileName();
            auto it = sname.find_last_of(L"\\/");
            if (it != std::wstring::npos)
                sname.erase(0, it + 1);

            sname = StringUtils::ToLower(sname);
            if (sname.contains(L"creationkit"))
            {
                if (HasRun())
                    throw std::runtime_error("CKPE is already injected to the process.");

                auto osver = HardwareInfo::OS::GetVersion();
                if ((GET_EXE_VERSION_EX_MAJOR(osver) < 6) ||
                    (((GET_EXE_VERSION_EX_MAJOR(osver) == 6) && (GET_EXE_VERSION_EX_MINOR(osver) <= 2))))
                    throw std::runtime_error("CKPE supports Windows OS versions 8.1 and above.");

#if __AVX__
                if (!HardwareInfo::CPU::HasSupportAVX())
                    throw std::runtime_error("CKPE compiled using AVX instructions.\nYour processor does not support this and is outdated.");
#elif __AVX2__
                if (!HardwareInfo::CPU::HasSupportAVX2())
                    throw std::runtime_error("CKPE compiled using AVX2 instructions.\nYour processor does not support this and is outdated.");
#endif 

                auto app = Application::GetSingleton();
                app->Initialize();

                auto lc = app->GetPEDirectory(PEDirectory::e_load_config);
                if (!lc.GetAddress() || !lc.GetSize() || !(lc.GetPointer<IMAGE_LOAD_CONFIG_DIRECTORY>())->SecurityCookie)
                    throw std::runtime_error("SecurityCookie is a null pointer!");

                _moduleBase = app->GetBase();

                auto ckpe_ver = FileUtils::GetFileVersion(std::wstring(app->GetPath()) + L"CKPE.dll");
                _MESSAGE("CKPE Runtime:\n\t%u.%u build %u\n", 
                    GET_EXE_VERSION_EX_MAJOR(ckpe_ver),
                    GET_EXE_VERSION_EX_MINOR(ckpe_ver),
                    GET_EXE_VERSION_EX_BUILD(ckpe_ver));

                _MESSAGE(L"OS:\n\t%s\n\t%u.%u build %u\n", 
                    // Without COM interface (Important in this stage application)
                    HardwareInfo::OS::GetVersionByString().c_str(),
                    GET_EXE_VERSION_EX_MAJOR(osver),
                    GET_EXE_VERSION_EX_MINOR(osver),
                    GET_EXE_VERSION_EX_BUILD(osver));

                auto _hasSSE41 = HardwareInfo::CPU::HasSupportSSE41();
                auto _hasAVX2 = HardwareInfo::CPU::HasSupportAVX2();

                // Detect hyper-threads and cores
                auto _hyperThreads = HardwareInfo::CPU::HasSupportHyper();
                auto _threads = HardwareInfo::CPU::GetTotalCores();
                auto _logicalCores = _threads;
                auto _physicalCores = _threads;
                if (_hyperThreads) _physicalCores >>= 1;

                _MESSAGE("Processor:\n\t%s\n\t%s\n\n\tMHz: %u\n\tThreads: %u\n\tLogical cores: %u\n\tPhysical cores: %u\n\t"
                    "Hyper-threads technology: %s\n\tSupports the SSE 4.1: %s\n\tSupports the AVX 2: %s\n",
                    HardwareInfo::CPU::GetProduct().c_str(),
                    HardwareInfo::CPU::GetBrand().c_str(),
                    HardwareInfo::CPU::GetMHz(),
                    _threads,
                    _logicalCores,
                    _physicalCores,
                    (_hyperThreads ? "true" : "false"), 
                    (_hasSSE41 ? "true" : "false"), 
                    (_hasAVX2 ? "true" : "false"));

                //else if (VersionLists::HasOutdatedEditorVersion())
                /*    throw std::runtime_error(
                        "Deprecated version of Creation Kit.\nYou need to update the Creation Kit."
                        "\nCheck out the list of supported versions on the mod page.");*/
                        //else
                        //    throw std::runtime_error(
                        //        "Unsupported version of Creation Kit.\nYou may need to update this mod."
                        //        "\nCheck out the list of supported versions on the mod page.");

                EnableBreakpoint();
                _run = true;
            }
        }

        void Runner::Shutdown()
        {
            // The library is loaded by the loader, so can't unload it via FreeLibrary.

            if (HasRun())
            {
                // They should never get in, if the CKPE is installed correctly.

                // NO TERMINATE: Only exclude ckpe
                Application::GetSingleton()->Shutdown();

                _run = false;
            }
        }

        void Init()
        {
            try
            {
                Runner.Run();
            }
            catch (const std::exception& e)
            {
                ErrorHandler::Trigger(e.what());
            }
        }

        void Shutdown()
        {
            Runner.Shutdown();
        }
    }
}
