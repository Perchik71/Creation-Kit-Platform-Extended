// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/Forms/TESObjectREFR.h>
#include <Patches/CKPE.SkyrimSE.Patch.Console.h>
#include <Patches/CKPE.SkyrimSE.Patch.MiscMessages.h>


namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			MiscMessages::MiscMessages() : Common::Patch()
			{
				SetName("Misc Messages");
			}

			bool MiscMessages::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* MiscMessages::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool MiscMessages::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> MiscMessages::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool MiscMessages::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool MiscMessages::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				if (VersionLists::GetEditorVersion() > VersionLists::EDITOR_SKYRIM_SE_1_6_438)
				{
					//
					// Add additional information to the error
					//
					auto rva = (std::uintptr_t)__CKPE_OFFSET(0);
					Detours::DetourCall(rva, (std::uintptr_t)&sub);
					SafeWrite::WriteNop(rva - 0x18, 7);
					SafeWrite::Write(rva - 0xD, { 0xC0 });
				}

				// Skip "Queued ref '%s' (%08X) of type %s"
				SafeWrite::Write(__CKPE_OFFSET(1), { 0x90, 0xE9 });
				// Skip "%s took %.2f ms to PostProcess.  This may cause a hitch in segment '%s'."
				SafeWrite::WriteNop(__CKPE_OFFSET(2), 5);

				return true;
			}

			void MiscMessages::sub(std::int64_t type_error, const char* format_message, ...) noexcept(true)
			{
				// It is necessary to get the stack of the calling function.
				auto rsp = (std::uintptr_t)_AddressOfReturnAddress() + 8;

				va_list ap;
				va_start(ap, format_message);
				{
					auto form = *(EditorAPI::Forms::TESObjectREFR**)(rsp + 0xDC0);
					if (form)
					{
						auto editorID = form->EditorID;
						auto cell = form->GetParentCell();
						if (cell)
						{
							auto cellEditorID = cell->EditorID;
							Console::LogWarning((Console::MsgType)type_error,
								"Ref \"%s\" (0x%08X) in Cell \"%s\" (0x%08X) light radius %.2f is less than minimum of 20.",
								editorID ? editorID : "", form->FormID, cellEditorID ? cellEditorID : "",
								cell->FormID, va_arg(ap, float));
						}
						else
							Console::LogWarning((Console::MsgType)type_error,
								"Ref \"%s\" (0x%08X) light radius %.2f is less than minimum of 20.",
								editorID ? editorID : "", form->FormID, va_arg(ap, float));
					}
					else
						Console::LogWarning((Console::MsgType)type_error,
							"Ref UNKNOWN light radius %.2f is less than minimum of 20.", va_arg(ap, float));
				}
				va_end(ap);
			}
		}
	}
}