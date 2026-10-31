// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/F/FormInterfaces.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// CK 1.10.982.3+ layout; 1.10.162 has 394 default objects.
				class BGSDefaultObjectManager :
					public TESForm,                                        // 000
					public BSTSingletonImplicit<BGSDefaultObjectManager>   // 028
				{
				public:
					struct DefaultObjectData
					{
						std::uint32_t uniqueID;
						std::uint32_t formID;
					};
					static_assert(sizeof(DefaultObjectData) == 0x8);

					constexpr static std::size_t DEFAULT_OBJECT_COUNT = 396;
					constexpr static std::uint8_t TYPE_ID = ftDefaultObjectManager;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSDialogueView[0];

					virtual ~BGSDefaultObjectManager() = default;

					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2A8 */ [[nodiscard]] bool CanCheckIn(const CheckInFormList* Forms) const noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;

				public:
					// Load stores form IDs here until InitItemImpl resolves them.
					/* 0x028 */ TESForm* objects[DEFAULT_OBJECT_COUNT];
					/* 0xC88 */ bool objectInitialized[DEFAULT_OBJECT_COUNT];
					/* 0xE14 */ DefaultObjectData originalObjects[DEFAULT_OBJECT_COUNT];
					/* 0x1A74 */ bool hasOriginalObjects;
					/* 0x1A75 */ std::uint8_t pad1A75[3];
				};
				static_assert(sizeof(BGSDefaultObjectManager) == 0x1A78);
				static_assert(offsetof(BGSDefaultObjectManager, objects) == 0x28);
				static_assert(offsetof(BGSDefaultObjectManager, objectInitialized) == 0xC88);
				static_assert(offsetof(BGSDefaultObjectManager, originalObjects) == 0xE14);
				static_assert(offsetof(BGSDefaultObjectManager, hasOriginalObjects) == 0x1A74);

				// CK 1.10.162 only; the modern view has two additional entries.
				class BGSDefaultObjectManager_Original : public TESForm
				{
				public:
					using DefaultObjectData = BGSDefaultObjectManager::DefaultObjectData;
					constexpr static std::size_t DEFAULT_OBJECT_COUNT = 394;

					virtual ~BGSDefaultObjectManager_Original() = default;

					/* 0x028 */ TESForm* objects[DEFAULT_OBJECT_COUNT];
					/* 0xC78 */ bool objectInitialized[DEFAULT_OBJECT_COUNT];
					/* 0xE04 */ DefaultObjectData originalObjects[DEFAULT_OBJECT_COUNT];
					/* 0x1A54 */ bool hasOriginalObjects;
					/* 0x1A55 */ std::uint8_t pad1A55[3];
				};
				static_assert(sizeof(BGSDefaultObjectManager_Original) == 0x1A58);
				static_assert(offsetof(BGSDefaultObjectManager_Original, objects) == 0x28);
				static_assert(offsetof(BGSDefaultObjectManager_Original, objectInitialized) == 0xC78);
				static_assert(offsetof(BGSDefaultObjectManager_Original, originalObjects) == 0xE04);
				static_assert(offsetof(BGSDefaultObjectManager_Original, hasOriginalObjects) == 0x1A54);
			}
		}
	}
}
