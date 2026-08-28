// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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

			bool MiscMessages::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool MiscMessages::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool MiscMessages::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				if (VersionLists::GetEditorVersion() > VersionLists::EDITOR_SKYRIM_SE_1_6_438)
				{
					//
					// Add additional information to the error
					//
					auto rva = Relocation(ID(171828), 0xC54).Address();
					Detours::DetourCall(rva, (std::uintptr_t)&sub);
					SafeWrite::WriteNop(rva - 0x18, 7);
					SafeWrite::Write(rva - 0xD, { 0xC0 });
				}

				// Skip "Queued ref '%s' (%08X) of type %s"
				Relocation(ID(229599), 0x66).Write({ 0x90, 0xE9 });
				// Skip "%s took %.2f ms to PostProcess.  This may cause a hitch in segment '%s'."
				Relocation(ID{ 743240, 959457 }, Offset{ 0x1E8, 0x284 }).WriteFill(NOP, 5);
				// Skip "File '%s' is changing ref (%08X):\r\nfrom base '%s' (%08X) to base '%s' (%08X)\r\nfrom cell '%s' (%08X) to cell '%s' (%08X)."
				Relocation(ID(551172), Offset{ 0x19BA, 0x19DE }).WriteFill(NOP, 5);

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