// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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

			bool BSPointerHandleManager::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool BSPointerHandleManager::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool BSPointerHandleManager::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				bool ExtremlyMode = _READ_OPTION_BOOL("CreationKit", "bBSPointerHandleExtremly", false);
				if (ExtremlyMode)
				{
					_CONSOLE("[WARNING] An extended set of refs has been included. You use it at your own risk.");

					Relocation(ID(433096)).WriteJump(&EditorAPI::BSPointerHandleManager_Extended_Extremly::InitSDM);
					Relocation(ID(214413)).WriteJump(&EditorAPI::HandleManager_Extended_Extremly::KillSDM);
					Relocation(ID(237124)).WriteJump(&EditorAPI::HandleManager_Extended_Extremly::WarnForUndestroyedHandles);
					Relocation(ID(237233)).WriteJump(&EditorAPI::BSPointerHandleManagerInterface_Extended_Extremly::GetCurrentHandle);
					Relocation(ID(90425)).WriteJump(&EditorAPI::BSPointerHandleManagerInterface_Extended_Extremly::CreateHandle);
					Relocation(ID(118309)).WriteJump(&EditorAPI::BSPointerHandleManagerInterface_Extended_Extremly::Destroy1);
					Relocation(ID(19091)).WriteJump(&EditorAPI::BSPointerHandleManagerInterface_Extended_Extremly::Destroy2);
					Relocation(ID(277012)).WriteJump(&EditorAPI::BSPointerHandleManagerInterface_Extended_Extremly::GetSmartPointer1);
					Relocation(ID(12732)).WriteJump(&EditorAPI::BSPointerHandleManagerInterface_Extended_Extremly::GetSmartPointer2);
					Relocation(ID(155378)).WriteJump(&EditorAPI::BSPointerHandleManagerInterface_Extended_Extremly::IsValid);

					EditorAPI::BSPointerHandleManagerCurrent::PointerHandleManagerCurrentId = 2;
				}
				else
				{
					_CONSOLE("An slightly expanded set of refs has been included.");

					Relocation(ID(433096)).WriteJump(&EditorAPI::BSPointerHandleManager_Extended::InitSDM);
					Relocation(ID(214413)).WriteJump(&EditorAPI::HandleManager_Extended::KillSDM);
					Relocation(ID(237124)).WriteJump(&EditorAPI::HandleManager_Extended::WarnForUndestroyedHandles);
					Relocation(ID(237233)).WriteJump(&EditorAPI::BSPointerHandleManagerInterface_Extended::GetCurrentHandle);
					Relocation(ID(90425)).WriteJump(&EditorAPI::BSPointerHandleManagerInterface_Extended::CreateHandle);
					Relocation(ID(118309)).WriteJump(&EditorAPI::BSPointerHandleManagerInterface_Extended::Destroy1);
					Relocation(ID(19091)).WriteJump(&EditorAPI::BSPointerHandleManagerInterface_Extended::Destroy2);
					Relocation(ID(277012)).WriteJump(&EditorAPI::BSPointerHandleManagerInterface_Extended::GetSmartPointer1);
					Relocation(ID(12732)).WriteJump(&EditorAPI::BSPointerHandleManagerInterface_Extended::GetSmartPointer2);
					Relocation(ID(155378)).WriteJump(&EditorAPI::BSPointerHandleManagerInterface_Extended::IsValid);

					EditorAPI::BSPointerHandleManagerCurrent::PointerHandleManagerCurrentId = 1;
				}

				// Cutting a lot is faster this way
				auto stext = Application::GetSingleton()->GetSegment(Segment::text);
				ScopeSafeWrite text(stext.GetAddress(), stext.GetSize());
				
				// Stub out the rest of the functions which shouldn't ever be called now
				text.Write(ID(393023).Address(), { 0xCC });	// BSUntypedPointerHandle::Clear
				text.Write(ID(94244).Address(),  { 0xCC });	// BSUntypedPointerHandle::SetAge
				text.Write(ID(354023).Address(), { 0xCC });	// BSUntypedPointerHandle::SetActive
				text.Write(ID(392950).Address(), { 0xCC });	// BSUntypedPointerHandle::GetAge_0
				text.Write(ID(236407).Address(), { 0xCC });	// BSUntypedPointerHandle::Set
				text.Write(ID(279861).Address(), { 0xCC });	// BSUntypedPointerHandle::GetIndex_0
				text.Write(ID(491034).Address(), { 0xCC });	// BSUntypedPointerHandle::GetIndex
				text.Write(ID(392949).Address(), { 0xCC });	// BSUntypedPointerHandle::GetAge
				text.Write(ID(49295).Address(),  { 0xCC });	// BSUntypedPointerHandle::ClearActive
				text.Write(ID(117969).Address(), { 0xCC });	// BSUntypedPointerHandle::SetIndex
				text.Write(ID(278892).Address(), { 0xCC });	// BSUntypedPointerHandle::IsBitwiseNull
				text.Write(ID(392888).Address(), { 0xCC });	// BSPointerHandle::AgeMatches
				text.Write(ID(571752).Address(), { 0xCC });	// BSPointerHandle::GetPtr
				text.Write(ID(571728).Address(), { 0xCC });	// BSPointerHandle::AssignPtr
				text.Write(ID(254056).Address(), { 0xCC });	// BSPointerHandle::IsActive
				text.Write(ID(393028).Address(), { 0xCC });	// BSHandleRefObject::AssignHandleIndex
				text.Write(ID(491031).Address(), { 0xCC });	// BSHandleRefObject::GetIndex

				return true;
			}
		}
	}
}