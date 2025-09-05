// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Starfield.VersionLists.h>
#include <EditorAPI/BSFile.h>
#include <Patches/CKPE.Starfield.Patch.OptimizationLoad.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
			static HANDLE HKFindFirstFileA(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData) noexcept(true)
			{
				return FindFirstFileExA(lpFileName, FindExInfoStandard, lpFindFileData, FindExSearchNameMatch,
					NULL, FIND_FIRST_EX_LARGE_FETCH);
			}

			OptimizationLoad::OptimizationLoad() : Common::Patch()
			{
				SetName("Optimization load");
			}

			bool OptimizationLoad::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* OptimizationLoad::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool OptimizationLoad::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> OptimizationLoad::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool OptimizationLoad::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() >= VersionLists::EDITOR_STARFIELD_1_14_70_0;
			}

			bool OptimizationLoad::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Plugin loading optimizations:
				//
				// - Replacing FindFirstNextA with a more optimized function FindFirstFileExA					
				// - Skip remove failed forms (This function is more likely to result in CTD) 
				// - Eliminate millions of calls to update the statusbar
				// - Increasing the read memory buffer to reduce disk access
				// - Reducing spin time. Important: materials are loaded in the background.

				Detours::DetourIAT(base, "kernel32.dll", "FindFirstFileA", (std::uintptr_t)HKFindFirstFileA);

				{
					// Cutting a lot is faster this way
					auto stext = interface->GetApplication()->GetSegment(Segment::text);
					ScopeSafeWrite text(stext.GetAddress(), stext.GetSize());

					// If there is an ZLib error, will try to leave the function 
					text.Write(__CKPE_OFFSET(2), { 0xE9, 0x41, 0x01, 0x00, 0x00, 0x90 });
					// Skip remove failed forms
					text.Write(__CKPE_OFFSET(3), { 0xEB });
					// Spam in the status bar
					text.Write(__CKPE_OFFSET(4), { 0xC3, 0x90 });
					// Spam Primitive with zero extents encountered for reference %08X
					text.Write(__CKPE_OFFSET(6), { 0xEB });
					// Spam Demanding CPU Geometry Data for '%s' because it wasn't ready on time
					text.WriteNop(__CKPE_OFFSET(7), 5);
					// Increasing the memory buffer to read the file
					auto rva = (std::uintptr_t)__CKPE_OFFSET(8);
					text.Write(rva, { 0x04 });
					text.Write(rva + 8, { 0x86 });
					// full skip "%s: Imported file missing: %s"
					text.Write(__CKPE_OFFSET(10), { 0xEB });
				}

				// Reducing spin time.
				// While need to wait, will occupy thread with something useful, for example, message processing
				Detours::DetourCall(__CKPE_OFFSET(9), (std::uintptr_t)&ProcessMessage);
				Detours::DetourCall(__CKPE_OFFSET(11), (std::uintptr_t)&ProcessMessage);
				//Detours::DetourCall(_RELDATA_RAV(12), (std::uintptr_t)&ProcessMessage);
				//Detours::DetourCall(_RELDATA_RAV(13), (std::uintptr_t)&ProcessMessage);
				//Detours::DetourCall(_RELDATA_RAV(14), (std::uintptr_t)&ProcessMessage);

				// 2 kb -> x kb >= 256 kb
				*(std::uintptr_t*)&EditorAPI::BSFile::ICreateInstance =
					Detours::DetourClassJump(__CKPE_OFFSET(5), (std::uintptr_t)&EditorAPI::BSFile::HKCreateInstance);

				return true;
			}

			void OptimizationLoad::ProcessMessage() noexcept(true)
			{
				Common::Interface::GetSingleton()->GetApplication()->MessageProcessing();
			}
		}
	}
}