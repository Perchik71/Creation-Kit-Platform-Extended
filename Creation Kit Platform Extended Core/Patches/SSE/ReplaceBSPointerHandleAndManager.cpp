// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/SSE/BSPointerHandleManager.h"
#include "ReplaceBSPointerHandleAndManager.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			using namespace EditorAPI::SkyrimSpectialEdition;

			ReplaceBSPointerHandleAndManagerPatch::ReplaceBSPointerHandleAndManagerPatch() : Module(GlobalEnginePtr)
			{}

			bool ReplaceBSPointerHandleAndManagerPatch::HasOption() const
			{
				return false;
			}

			bool ReplaceBSPointerHandleAndManagerPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* ReplaceBSPointerHandleAndManagerPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* ReplaceBSPointerHandleAndManagerPatch::GetName() const
			{
				return "Replace BSPointerHandle And Manager";
			}

			bool ReplaceBSPointerHandleAndManagerPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> ReplaceBSPointerHandleAndManagerPatch::GetDependencies() const
			{
				return {};
			}

			bool ReplaceBSPointerHandleAndManagerPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool ReplaceBSPointerHandleAndManagerPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					bool ExtremlyMode = _READ_OPTION_BOOL("CreationKit", "bBSPointerHandleExtremly", false);
					if (ExtremlyMode)
					{
						_CONSOLE("[WARNING] An extended set of refs has been included. You use it at your own risk.");

						lpRelocator->DetourJump(lpRelocationDatabaseItem->At(17),
							(uintptr_t)&BSPointerHandleManager_Extended_Extremly::InitSDM);
						lpRelocator->DetourJump(lpRelocationDatabaseItem->At(18),
							(uintptr_t)&HandleManager_Extended_Extremly::KillSDM);
						lpRelocator->DetourJump(lpRelocationDatabaseItem->At(19),
							(uintptr_t)&HandleManager_Extended_Extremly::WarnForUndestroyedHandles);
						lpRelocator->DetourJump(lpRelocationDatabaseItem->At(20),
							(uintptr_t)&BSPointerHandleManagerInterface_Extended_Extremly::GetCurrentHandle);
						lpRelocator->DetourJump(lpRelocationDatabaseItem->At(21),
							(uintptr_t)&BSPointerHandleManagerInterface_Extended_Extremly::CreateHandle);
						lpRelocator->DetourJump(lpRelocationDatabaseItem->At(22),
							(uintptr_t)&BSPointerHandleManagerInterface_Extended_Extremly::Destroy1);
						lpRelocator->DetourJump(lpRelocationDatabaseItem->At(23),
							(uintptr_t)&BSPointerHandleManagerInterface_Extended_Extremly::Destroy2);
						lpRelocator->DetourJump(lpRelocationDatabaseItem->At(24),
							(uintptr_t)&BSPointerHandleManagerInterface_Extended_Extremly::GetSmartPointer1);
						lpRelocator->DetourJump(lpRelocationDatabaseItem->At(25),
							(uintptr_t)&BSPointerHandleManagerInterface_Extended_Extremly::GetSmartPointer2);
						lpRelocator->DetourJump(lpRelocationDatabaseItem->At(26),
							(uintptr_t)&BSPointerHandleManagerInterface_Extended_Extremly::IsValid);

						BSPointerHandleManagerCurrent::PointerHandleManagerCurrentId = 2;
					}
					else
					{
						_CONSOLE("An slightly expanded set of refs has been included.");

						lpRelocator->DetourJump(lpRelocationDatabaseItem->At(17),
							(uintptr_t)&BSPointerHandleManager_Extended::InitSDM);
						lpRelocator->DetourJump(lpRelocationDatabaseItem->At(18),
							(uintptr_t)&HandleManager_Extended::KillSDM);
						lpRelocator->DetourJump(lpRelocationDatabaseItem->At(19),
							(uintptr_t)&HandleManager_Extended::WarnForUndestroyedHandles);
						lpRelocator->DetourJump(lpRelocationDatabaseItem->At(20),
							(uintptr_t)&BSPointerHandleManagerInterface_Extended::GetCurrentHandle);
						lpRelocator->DetourJump(lpRelocationDatabaseItem->At(21),
							(uintptr_t)&BSPointerHandleManagerInterface_Extended::CreateHandle);
						lpRelocator->DetourJump(lpRelocationDatabaseItem->At(22),
							(uintptr_t)&BSPointerHandleManagerInterface_Extended::Destroy1);
						lpRelocator->DetourJump(lpRelocationDatabaseItem->At(23),
							(uintptr_t)&BSPointerHandleManagerInterface_Extended::Destroy2);
						lpRelocator->DetourJump(lpRelocationDatabaseItem->At(24),
							(uintptr_t)&BSPointerHandleManagerInterface_Extended::GetSmartPointer1);
						lpRelocator->DetourJump(lpRelocationDatabaseItem->At(25),
							(uintptr_t)&BSPointerHandleManagerInterface_Extended::GetSmartPointer2);
						lpRelocator->DetourJump(lpRelocationDatabaseItem->At(26),
							(uintptr_t)&BSPointerHandleManagerInterface_Extended::IsValid);

						BSPointerHandleManagerCurrent::PointerHandleManagerCurrentId = 1;
					}

					ScopeRelocator textSection;

					// Stub out the rest of the functions which shouldn't ever be called now
					lpRelocator->Patch(lpRelocationDatabaseItem->At(0),		{ 0xCC });	// BSUntypedPointerHandle::Clear
					lpRelocator->Patch(lpRelocationDatabaseItem->At(1),		{ 0xCC });	// BSUntypedPointerHandle::SetAge
					lpRelocator->Patch(lpRelocationDatabaseItem->At(2),		{ 0xCC });	// BSUntypedPointerHandle::SetActive
					lpRelocator->Patch(lpRelocationDatabaseItem->At(3),		{ 0xCC });	// BSUntypedPointerHandle::GetAge_0
					lpRelocator->Patch(lpRelocationDatabaseItem->At(4),		{ 0xCC });	// BSUntypedPointerHandle::Set
					lpRelocator->Patch(lpRelocationDatabaseItem->At(5),		{ 0xCC });	// BSUntypedPointerHandle::GetIndex_0
					lpRelocator->Patch(lpRelocationDatabaseItem->At(6),		{ 0xCC });	// BSUntypedPointerHandle::GetIndex
					lpRelocator->Patch(lpRelocationDatabaseItem->At(7),		{ 0xCC });	// BSUntypedPointerHandle::GetAge
					lpRelocator->Patch(lpRelocationDatabaseItem->At(8),		{ 0xCC });	// BSUntypedPointerHandle::ClearActive
					lpRelocator->Patch(lpRelocationDatabaseItem->At(9),		{ 0xCC });	// BSUntypedPointerHandle::SetIndex
					lpRelocator->Patch(lpRelocationDatabaseItem->At(10),	{ 0xCC });	// BSUntypedPointerHandle::IsBitwiseNull
					lpRelocator->Patch(lpRelocationDatabaseItem->At(11),	{ 0xCC });	// BSPointerHandle::AgeMatches
					lpRelocator->Patch(lpRelocationDatabaseItem->At(12),	{ 0xCC });	// BSPointerHandle::GetPtr
					lpRelocator->Patch(lpRelocationDatabaseItem->At(13),	{ 0xCC });	// BSPointerHandle::AssignPtr
					lpRelocator->Patch(lpRelocationDatabaseItem->At(14),	{ 0xCC });	// BSPointerHandle::IsActive
					lpRelocator->Patch(lpRelocationDatabaseItem->At(15),	{ 0xCC });	// BSHandleRefObject::AssignHandleIndex
					lpRelocator->Patch(lpRelocationDatabaseItem->At(16),	{ 0xCC });	// BSHandleRefObject::GetIndex

					return true;
				}

				return false;
			}

			bool ReplaceBSPointerHandleAndManagerPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}