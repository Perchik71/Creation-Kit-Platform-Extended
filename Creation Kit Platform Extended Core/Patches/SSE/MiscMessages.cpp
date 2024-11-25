// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/SSE/TESObjectREFR.h"
#include "Editor API/SSE/TESObjectCELL.h"
#include "MiscMessages.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		extern LPCSTR typeListSkyrim[];

		namespace SkyrimSpectialEdition
		{
			MiscMessagesPatch::MiscMessagesPatch() : Module(GlobalEnginePtr)
			{}

			bool MiscMessagesPatch::HasOption() const
			{
				return false;
			}

			bool MiscMessagesPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* MiscMessagesPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* MiscMessagesPatch::GetName() const
			{
				return "Misc Messages";
			}

			bool MiscMessagesPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> MiscMessagesPatch::GetDependencies() const
			{
				return {};
			}

			bool MiscMessagesPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_1_6_1130;
			}

			bool MiscMessagesPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// Add additional information to the error
					auto rva = (uintptr_t)_RELDATA_RAV(0);
					lpRelocator->DetourCall(rva, (uintptr_t)&sub_LightRadiusError);
					lpRelocator->PatchNop(rva - 0x18, 7);
					lpRelocator->Patch(rva - 0xD, { 0xC0 });
		
					return true;
				}

				return false;
			}

			bool MiscMessagesPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void MiscMessagesPatch::sub_LightRadiusError(__int64 id_system, const char* format_message, ...)
			{
				// It is necessary to get the stack of the calling function.
				auto rsp = (uintptr_t)_AddressOfReturnAddress() + 8;
				
				va_list ap;
				va_start(ap, format_message);
				{
					auto form = *(EditorAPI::SkyrimSpectialEdition::TESObjectREFR**)(rsp + 0xDC0);
					if (form)
					{
						auto editorID = form->GetEditorID_NoVTable();
						auto cell = form->GetParentCell();
						if (cell)
						{
							auto cellEditorID = cell->GetEditorID_NoVTable();

							_CONSOLE("%s: Ref \"%s\" (0x%08X) in Cell \"%s\" (0x%08X) light radius %.2f is less than minimum of 20.", 
								typeListSkyrim[id_system], editorID ? editorID : "", form->FormID, 
								cellEditorID ? cellEditorID : "", cell->FormID, va_arg(ap, float));
						}
						else
							_CONSOLE("%s: Ref \"%s\" (0x%08X) light radius %.2f is less than minimum of 20.", typeListSkyrim[id_system],
								editorID ? editorID : "", form->FormID, va_arg(ap, float));
					}
					else
						_CONSOLE("%s: Ref UNKNOWN light radius %.2f is less than minimum of 20.", typeListSkyrim[id_system],
							va_arg(ap, float));
				}
				va_end(ap);
			}
		}
	}
}