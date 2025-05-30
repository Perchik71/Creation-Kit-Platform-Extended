// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <CKPE.WinHttp.Native.h>
#include <CKPE.h>

#include <stdexcept>

namespace CKPE
{
    namespace WinHttp
    {
        class Runner
        {
            bool _run{false};
        public:
            constexpr Runner() noexcept(true) = default;
            Runner(const Runner&) = delete;
            Runner operator=(const Runner&) = delete;

            [[nodiscard]] constexpr inline bool HasRun() const noexcept(true) { return _run; }

            void Run();
            void Shutdown();
        } static Runner;

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
                if ((GET_EXE_VERSION_MAJOR(osver) < 6) || 
                    (((GET_EXE_VERSION_MAJOR(osver) == 6) && (GET_EXE_VERSION_MINOR(osver) <= 2))))
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

                auto ckpe_ver = FileUtils::GetFileVersion(std::wstring(app->GetPath()) + L"CKPE.dll");
                _MESSAGE("CKPE Runtime:\n\t%u.%u build %u\n", 
                    GET_EXE_VERSION_MAJOR(ckpe_ver),
                    GET_EXE_VERSION_MINOR(ckpe_ver),
                    GET_EXE_VERSION_BUILD(ckpe_ver));

                _MESSAGE(L"OS:\n\t%s\n\t%u.%u build %u\n", 
                    // Without COM interface (Important in this stage application)
                    HardwareInfo::OS::GetVersionByString().c_str(),
                    GET_EXE_VERSION_MAJOR(osver),
                    GET_EXE_VERSION_MINOR(osver),
                    GET_EXE_VERSION_BUILD(osver));

                auto _hasSSE41 = HardwareInfo::CPU::HasSupportSSE41();
                auto _hasAVX2 = HardwareInfo::CPU::HasSupportAVX2();

                // Detect hyper-threads and cores
                auto _hyperThreads = HardwareInfo::CPU::HasSupportHyper();
                auto _threads = HardwareInfo::CPU::GetTotalCores();
                auto _logicalCores = _threads;
                auto _physicalCores = _threads;
                if (_hyperThreads) _physicalCores >>= 1;

                _MESSAGE("Processor:\n\t%s\n\t%s\n\n\tMHz: %u\n\tThreads: %u\n\tLogical cores: %u\n\tPhysical cores: %u\n\t"
                    "Hyper-threads technology: %s\n\tSupports the SSE 4.1: %s\n\tSupports the AVX 2: %s",
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
            }
        }

        void Runner::Shutdown()
        {
            if (HasRun())
            {
                // TODO

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
