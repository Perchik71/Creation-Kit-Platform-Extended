// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/BSPointerHandleManager.h>
#include <Patches/CKPE.SkyrimSE.Patch.BSPointerHandleManager.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			BSPointerHandleManager::BSPointerHandleManager() : Common::Patch()
			{
				SetName("Replace BSPointerHandle And Manager");
			}

			bool BSPointerHandleManager::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* BSPointerHandleManager::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool BSPointerHandleManager::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> BSPointerHandleManager::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool BSPointerHandleManager::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool BSPointerHandleManager::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				bool ExtremlyMode = _READ_OPTION_BOOL("CreationKit", "bBSPointerHandleExtremly", false);
				if (ExtremlyMode)
				{
					_CONSOLE("[WARNING] An extended set of refs has been included. You use it at your own risk.");

					Detours::DetourJump(__CKPE_OFFSET(17),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManager_Extended_Extremly::InitSDM);
					Detours::DetourJump(__CKPE_OFFSET(18),
						(std::uintptr_t)&EditorAPI::HandleManager_Extended_Extremly::KillSDM);
					Detours::DetourJump(__CKPE_OFFSET(19),
						(std::uintptr_t)&EditorAPI::HandleManager_Extended_Extremly::WarnForUndestroyedHandles);
					Detours::DetourJump(__CKPE_OFFSET(20),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManagerInterface_Extended_Extremly::GetCurrentHandle);
					Detours::DetourJump(__CKPE_OFFSET(21),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManagerInterface_Extended_Extremly::CreateHandle);
					Detours::DetourJump(__CKPE_OFFSET(22),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManagerInterface_Extended_Extremly::Destroy1);
					Detours::DetourJump(__CKPE_OFFSET(23),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManagerInterface_Extended_Extremly::Destroy2);
					Detours::DetourJump(__CKPE_OFFSET(24),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManagerInterface_Extended_Extremly::GetSmartPointer1);
					Detours::DetourJump(__CKPE_OFFSET(25),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManagerInterface_Extended_Extremly::GetSmartPointer2);
					Detours::DetourJump(__CKPE_OFFSET(26),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManagerInterface_Extended_Extremly::IsValid);

					EditorAPI::BSPointerHandleManagerCurrent::PointerHandleManagerCurrentId = 2;
				}
				else
				{
					_CONSOLE("An slightly expanded set of refs has been included.");

					Detours::DetourJump(__CKPE_OFFSET(17),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManager_Extended::InitSDM);
					Detours::DetourJump(__CKPE_OFFSET(18),
						(std::uintptr_t)&EditorAPI::HandleManager_Extended::KillSDM);
					Detours::DetourJump(__CKPE_OFFSET(19),
						(std::uintptr_t)&EditorAPI::HandleManager_Extended::WarnForUndestroyedHandles);
					Detours::DetourJump(__CKPE_OFFSET(20),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManagerInterface_Extended::GetCurrentHandle);
					Detours::DetourJump(__CKPE_OFFSET(21),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManagerInterface_Extended::CreateHandle);
					Detours::DetourJump(__CKPE_OFFSET(22),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManagerInterface_Extended::Destroy1);
					Detours::DetourJump(__CKPE_OFFSET(23),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManagerInterface_Extended::Destroy2);
					Detours::DetourJump(__CKPE_OFFSET(24),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManagerInterface_Extended::GetSmartPointer1);
					Detours::DetourJump(__CKPE_OFFSET(25),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManagerInterface_Extended::GetSmartPointer2);
					Detours::DetourJump(__CKPE_OFFSET(26),
						(std::uintptr_t)&EditorAPI::BSPointerHandleManagerInterface_Extended::IsValid);

					EditorAPI::BSPointerHandleManagerCurrent::PointerHandleManagerCurrentId = 1;
				}

				// Cutting a lot is faster this way
				auto stext = interface->GetApplication()->GetSegment(Segment::text);
				ScopeSafeWrite text(stext.GetAddress(), stext.GetSize());

				// Stub out the rest of the functions which shouldn't ever be called now
				text.Write(__CKPE_OFFSET(0), { 0xCC });		// BSUntypedPointerHandle::Clear
				text.Write(__CKPE_OFFSET(1), { 0xCC });		// BSUntypedPointerHandle::SetAge
				text.Write(__CKPE_OFFSET(2), { 0xCC });		// BSUntypedPointerHandle::SetActive
				text.Write(__CKPE_OFFSET(3), { 0xCC });		// BSUntypedPointerHandle::GetAge_0
				text.Write(__CKPE_OFFSET(4), { 0xCC });		// BSUntypedPointerHandle::Set
				text.Write(__CKPE_OFFSET(5), { 0xCC });		// BSUntypedPointerHandle::GetIndex_0
				text.Write(__CKPE_OFFSET(6), { 0xCC });		// BSUntypedPointerHandle::GetIndex
				text.Write(__CKPE_OFFSET(7), { 0xCC });		// BSUntypedPointerHandle::GetAge
				text.Write(__CKPE_OFFSET(8), { 0xCC });		// BSUntypedPointerHandle::ClearActive
				text.Write(__CKPE_OFFSET(9), { 0xCC });		// BSUntypedPointerHandle::SetIndex
				text.Write(__CKPE_OFFSET(10), { 0xCC });	// BSUntypedPointerHandle::IsBitwiseNull
				text.Write(__CKPE_OFFSET(11), { 0xCC });	// BSPointerHandle::AgeMatches
				text.Write(__CKPE_OFFSET(12), { 0xCC });	// BSPointerHandle::GetPtr
				text.Write(__CKPE_OFFSET(13), { 0xCC });	// BSPointerHandle::AssignPtr
				text.Write(__CKPE_OFFSET(14), { 0xCC });	// BSPointerHandle::IsActive
				text.Write(__CKPE_OFFSET(15), { 0xCC });	// BSHandleRefObject::AssignHandleIndex
				text.Write(__CKPE_OFFSET(16), { 0xCC });	// BSHandleRefObject::GetIndex

				return true;
			}
		}
	}
}