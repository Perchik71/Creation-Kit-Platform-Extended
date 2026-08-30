// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Application.h>
#include <CKPE.StringUtils.h>
#include <CKPE.Common.RTTI.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.CrashHandler.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/NiAPI/NiSourceTexture.h>
#include <EditorAPI/NiAPI/NiAVObject.h>
#include <EditorAPI/Forms/TESForm.h>
#include <EditorAPI/BSResources.h>
#include <EditorAPI/BSShaderProperty.h>
#include <EditorAPI/BSFile.h>
#include <EditorAPI/BGSLocalizedString.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashDump.h>
#include <resource_version2.h>

namespace CKPE
{
	namespace SkyrimSE
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

			bool CrashDump::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool CrashDump::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashDump::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				CrashHandler::GetSingleton()->Install();
				auto base = Application::GetSingleton()->GetBase();

				auto purecallHandler = []()
					{
						RaiseException('PURE', EXCEPTION_NONCONTINUABLE, 0, nullptr);
					};

				auto terminateHandler = []()
					{
						RaiseException('TERM', EXCEPTION_NONCONTINUABLE, 0, nullptr);
					};

				auto patchIAT = [&terminateHandler, &purecallHandler, &base](const char* hmodule)
					{
						Detours::DetourIAT(base, hmodule, "_cexit", (std::uintptr_t)(void(*)())terminateHandler);
						Detours::DetourIAT(base, hmodule, "_exit", (std::uintptr_t)(void(*)())terminateHandler);
						Detours::DetourIAT(base, hmodule, "_Exit", (std::uintptr_t)(void(*)())terminateHandler);
						Detours::DetourIAT(base, hmodule, "_c_exit", (std::uintptr_t)(void(*)())terminateHandler);
						Detours::DetourIAT(base, hmodule, "exit", (std::uintptr_t)(void(*)())terminateHandler);
						Detours::DetourIAT(base, hmodule, "abort", (std::uintptr_t)(void(*)())terminateHandler);
						Detours::DetourIAT(base, hmodule, "terminate", (std::uintptr_t)(void(*)())terminateHandler);
						Detours::DetourIAT(base, hmodule, "_purecall", (std::uintptr_t)(void(*)())purecallHandler);
					};

				// Патчим все известные библы используемые в CK

				patchIAT("API-MS-WIN-CRT-RUNTIME-L1-1-0.DLL");
				patchIAT("MSVCR110.DLL");
				
				Relocation(ID{ 694424, 998237 }).Write(RET);	// StackTrace::MemoryTraceWrite
				Relocation(ID(593935), 0x2F).Write(RET);		// SetUnhandledExceptionFilter, BSWin32ExceptionHandler
				Relocation(ID(133680), 7).Write(RET);			// SetUnhandledExceptionFilter, Unknown
				Relocation(ID(218884), 0xB).WriteFill(NOP, 6);	// SetUnhandledExceptionFilter, BSWin32ExceptionHandler

				Common::CrashHandler::GetSingleton()->OnAnalyzeClassRef = DoAnalyzeClassRef;
				Common::CrashHandler::GetSingleton()->OnOutputVersion = DoOutputVersion;
				Common::CrashHandler::GetSingleton()->OnOutputCKVersion = DoOutputCKVersion;

				return true;
			}

			void CrashDump::DoOutputVersion(std::string& Result) noexcept(true)
			{
				Result = "SSE CKPE Game Library " VER_FILE_VERSION_STR " " __DATE__ " " __TIME__;
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
				else if (!strcmp(RttiName, "class BSFile"))
				{
					auto fstm = (EditorAPI::BSFile*)Address;
					if (fstm->GetFileName())
						Result = StringUtils::FormatString("\"%s\"", fstm->GetFileName());
					else
						Result = "nullptr";
				}
				else if (!strcmp(RttiName, "class NiTexture") || !strcmp(RttiName, "class NiSourceTexture"))
				{
					auto tex = (EditorAPI::NiAPI::NiTexture*)Address;
					if (tex->GetFileName())
						Result = StringUtils::FormatString("\"%s\"", tex->GetFileName());
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
						Result = StringUtils::FormatString("\"%s\"", FileName.c_str());
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
					else
					{
						auto node = _DYNAMIC_CAST((void*)Address, 0, RttiName, "class NiAVObject");
						if (node)
						{
							auto buf = std::make_unique<char[]>(256);
							auto refr = (EditorAPI::Forms::TESForm*)((EditorAPI::NiAPI::NiAVObject*)node)->GetFormRef();
							if (refr)
							{
								refr->DebugInfo(buf.get(), 256);
								Result = buf.get();
							}
						}
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