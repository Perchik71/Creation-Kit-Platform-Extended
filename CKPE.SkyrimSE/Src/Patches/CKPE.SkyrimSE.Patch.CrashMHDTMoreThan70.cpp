// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <mmsystem.h>
#include <CKPE.Utils.h>
#include <CKPE.MessageBox.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/Forms/TESObjectCELL.h>
#include <EditorAPI/NiAPI/NiTypes.h>
#include <Patches/CKPE.SkyrimSE.Patch.Console.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashMHDTMoreThan70.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			std::uintptr_t pointer_CrashMHDTMoreThan70Patch_sub1 = 0;
			std::uintptr_t pointer_CrashMHDTMoreThan70Patch_sub2 = 0;
			std::uintptr_t pointer_CrashMHDTMoreThan70Patch_sub3 = 0;

			CrashMHDTMoreThan70::CrashMHDTMoreThan70() : Common::Patch()
			{
				SetName("Crash MHDT More Than 70");
			}

			bool CrashMHDTMoreThan70::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* CrashMHDTMoreThan70::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool CrashMHDTMoreThan70::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> CrashMHDTMoreThan70::GetDependencies() const noexcept(true)
			{
				return { "Console" };
			}

			bool CrashMHDTMoreThan70::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool CrashMHDTMoreThan70::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashMHDTMoreThan70::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				// This patch adds additional information to the log, nothing more.

				pointer_CrashMHDTMoreThan70Patch_sub1 = ID(571009).Address();
				pointer_CrashMHDTMoreThan70Patch_sub2 = ID(347773).Address();

				Relocation(ID(236902), 0x476).WriteCall(&sub1);
				Relocation(ID(402801), 0x57E).WriteCall(&sub2);

				// Added save and terminate process to end gen MHDT
					
				pointer_CrashMHDTMoreThan70Patch_sub3 = ID(197933).Address();
				Relocation(ID(465745), Offset{ 0x1E57, 0x1E2F }).WriteCall(&sub3_additional);
				
				return true;
			}

			void* CrashMHDTMoreThan70::sub1(void* world, int x, int y) noexcept(true)
			{
				auto worldForm = (EditorAPI::Forms::TESForm*)world;
				if (!worldForm || worldForm->Type != EditorAPI::Forms::TESForm::ftWorldSpace)
					return nullptr;

				auto cellForm = 
					((EditorAPI::Forms::TESObjectCELL*(__fastcall*)(void*, int, int))pointer_CrashMHDTMoreThan70Patch_sub1)
					(world, x, y);
				
				if (cellForm)
				{
					if (cellForm->EditorID && cellForm->EditorID[0])
						Console::LogWarning(Console::CELLS, "Processing of the height map of the \"%s\" (0x%08X) cell according to the coordinates (%d, %d) of the \"%s\" world",
							cellForm->EditorID, cellForm->FormID, x, y, worldForm->FullName);
					else
						Console::LogWarning(Console::CELLS, "Processing of the height map of the (0x%08X) cell according to the coordinates (%d, %d) of the \"%s\" world",
							cellForm->FormID, x, y, worldForm->FullName);
				}

				return (void*)cellForm;
			}

			void CrashMHDTMoreThan70::sub2(void* shape, void* vec, void* point, void* unk1, int unk2) noexcept(true)
			{
				auto pp = (EditorAPI::NiAPI::NiPoint2*)point;
				auto pv = (EditorAPI::NiAPI::NiPoint3*)vec;

				// It is necessary to get the stack of the calling function.
				auto rsp = (std::uintptr_t)_AddressOfReturnAddress() + 8;

				auto coord_x = *(float*)(rsp + 0x64);
				auto coord_y = *(float*)(rsp + 0x60);

				__try
				{
					((void(__fastcall*)(void*, void*, void*, void*, int))pointer_CrashMHDTMoreThan70Patch_sub2)
						(shape, vec, point, unk1, unk2);
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					_CONSOLE("ASSERTION: Fatal calculating the height for the point (%.0f, %.0f) of (%.3f, %.3f)", 
						coord_x, coord_y, pp->x, pp->y);
				}
			}

			void CrashMHDTMoreThan70::sub3_additional() noexcept(true)
			{
				ReleaseCapture();

				auto sub = []() {
					__try
					{
						fast_call<void>(pointer_CrashMHDTMoreThan70Patch_sub3);
					}
					__except (1)
					{
					}
					};
				sub();
				
				MessageBox::OpenInfo("Done");
			}
		}
	}
}