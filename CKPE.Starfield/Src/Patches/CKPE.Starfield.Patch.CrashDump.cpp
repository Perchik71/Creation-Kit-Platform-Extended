// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.StringUtils.h>
#include <CKPE.Common.RTTI.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.CrashHandler.h>
#include <CKPE.Starfield.VersionLists.h>
#include <EditorAPI/Forms/TESForm.h>
#include <EditorAPI/BSResources.h>
#include <EditorAPI/BSFile.h>
#include <EditorAPI/BGSLocalizedString.h>
#include <Patches/CKPE.Starfield.Patch.CrashDump.h>
#include "..\..\CKPE.Starfield\resource_version2.h"

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
			CrashDump::CrashDump() : Common::Patch()
			{
				SetName("Crash Dump");
			}

			bool CrashDump::HasOption() const noexcept(true)
			{
				return true;
			}

			const char* CrashDump::GetOptionName() const noexcept(true)
			{
				return "CreationKit:bGenerateCrashdumps";
			}

			bool CrashDump::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> CrashDump::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool CrashDump::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_STARFIELD_LAST;
			}

			bool CrashDump::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 2)
					return false;

				CommandLineParser CommandLine;
				if (CommandLine.HasCommandRun())
				{
					auto Cmd = CommandLine.GetCommand();
					auto Sep = Cmd.find_first_of(':');
					if (Sep != std::string::npos)
					{
						Cmd = Cmd.substr(0, Sep);

						if (!_wcsicmp(Cmd.c_str(), L"-GenerateInventoryIcons") ||
							!_wcsicmp(Cmd.c_str(), L"-GenerateWorkshopIcons") ||
							!_wcsicmp(Cmd.c_str(), L"-GenerateShipBuilderIcons"))
						{
							_ERROR("CrashDump patch is not compatible with \"%s\" command.", Cmd.c_str());
							// break
							return true;
						}
					}
				}

				Common::CrashHandler::GetSingleton()->Install();

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				auto purecallHandler = []()
					{
						RaiseException('PURE', EXCEPTION_NONCONTINUABLE, 0, NULL);
					};

				auto terminateHandler = []()
					{
						RaiseException('TERM', EXCEPTION_NONCONTINUABLE, 0, NULL);
					};

				auto patchIAT = [&terminateHandler, &purecallHandler, &base](const char* module)
					{
						Detours::DetourIAT(base, module, "_cexit", (uintptr_t)(void(*)())terminateHandler);
						Detours::DetourIAT(base, module, "_exit", (uintptr_t)(void(*)())terminateHandler);
						Detours::DetourIAT(base, module, "_Exit", (uintptr_t)(void(*)())terminateHandler);
						Detours::DetourIAT(base, module, "_c_exit", (uintptr_t)(void(*)())terminateHandler);
						Detours::DetourIAT(base, module, "exit", (uintptr_t)(void(*)())terminateHandler);
						Detours::DetourIAT(base, module, "abort", (uintptr_t)(void(*)())terminateHandler);
						Detours::DetourIAT(base, module, "terminate", (uintptr_t)(void(*)())terminateHandler);
						Detours::DetourIAT(base, module, "_purecall", (uintptr_t)(void(*)())purecallHandler);
					};

				// Патчим все известные библы используемые в CK

				patchIAT("API-MS-WIN-CRT-RUNTIME-L1-1-0.DLL");
				patchIAT("MSVCR110.DLL");

				for (uint32_t i = 0; i < db->GetCount(); i++)
					SafeWrite::Write(__CKPE_OFFSET(i), { 0xC3 });

				Common::CrashHandler::GetSingleton()->OnAnalyzeClassRef = DoAnalyzeClassRef;
				Common::CrashHandler::GetSingleton()->OnOutputVersion = DoOutputVersion;
				Common::CrashHandler::GetSingleton()->OnOutputCKVersion = DoOutputCKVersion;

				return true;
			}

			void CrashDump::DoOutputVersion(std::string& Result) noexcept(true)
			{
				Result = "SF CKPE Game Library " VER_FILE_VERSION_STR " " __DATE__ " " __TIME__;
			}

			void CrashDump::DoAnalyzeClassRef(std::uintptr_t Address, const char* RttiName, std::string& Result)
			{
				if (!Address) return;

				if (!strcmp(RttiName, "class BGSLocalizedString"))
				{
					auto str = (EditorAPI::BGSLocalizedString*)Address;
					if (str->data())
						Result = StringUtils::FormatString("\"%s\"", str->c_str());
					else
						Result = "nullptr";
				}
				else if (!strcmp(RttiName, "class BGSLocalizedStringDL"))
				{
					auto str = (EditorAPI::BGSLocalizedStringDL*)Address;
					if (str->data())
						Result = StringUtils::FormatString("\"%s\"", str->c_str());
					else
						Result = "nullptr";
				}
				else if (!strcmp(RttiName, "class BGSLocalizedStringIL"))
				{
					auto str = (EditorAPI::BGSLocalizedStringIL*)Address;
					if (str->data())
						Result = StringUtils::FormatString("\"%s\"", str->c_str());
					else
						Result = "nullptr";
				}
				else if (!strcmp(RttiName, "class NiFile"))
				{
					auto fstm = (EditorAPI::NiAPI::NiFile*)Address;
					if (fstm->GetFileName())
						Result = StringUtils::FormatString("\"%s\"", fstm->GetFileName());
					else
						Result = "nullptr";
				}
				else if (!strcmp(RttiName, "class BSFile"))
				{
					auto fstm = (EditorAPI::BSFile*)Address;
					if (fstm->GetFileName2())
						Result = StringUtils::FormatString("\"%s\"", fstm->GetFileName2());
					else
						Result = "nullptr";
				}
				else if (!strcmp(RttiName, "class BSResourceNiBinaryStream"))
				{
					auto resource = (EditorAPI::BSResourceNiBinaryStream*)Address;
					auto stream = resource->GetStream();
					if (stream)
					{
						EditorAPI::BSFixedString FileName;
						stream->GetFileName(FileName);
						Result = StringUtils::FormatString("\"%s\"", FileName.c_str<char>());
					}
					else
						Result = "nullptr";
				}
				else
				{
					auto form = _DYNAMIC_CAST((void*)Address, 0, RttiName, "class TESForm");
					if (form)
					{
						auto buf = std::make_unique<char[]>(256);
						((EditorAPI::Forms::TESForm*)form)->DebugInfo(buf.get(), 256);
						Result = buf.get();
					}
				}
			}

			void CrashDump::DoOutputCKVersion(std::string& Result) noexcept(true)
			{
				Result = StringUtils::Utf16ToWinCP(VersionLists::GetEditorVersionByString());
			}
		}
	}
}