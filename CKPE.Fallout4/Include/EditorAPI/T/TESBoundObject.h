// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/TBO_InstanceData.h>
#include <EditorAPI/N/NiPointer.h>
#include <EditorAPI/B/BSTList.h>
#include <EditorAPI/T/TESObject.h>
#include <EditorAPI/T/TESCellUseList.h>
#include <EditorAPI/B/BGSMod.h>
#include <EditorAPI/B/BGSPreviewTransform.h>
#include <EditorAPI/B/BGSSoundTagComponent.h>
#include <EditorAPI/T/TESObjectREFRFwd.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class TESContainer;
			struct ContainerItemExtra;

			namespace NiAPI
			{
				class NiAVObject;
			}

			namespace Forms
			{
				class BGSVoiceType;
				class BGSSoundDescriptorForm;
				// 0x90
				class TESBoundObject :
					public TESObject,        // 000
					public TESCellUseList    // 030
				{
				public:
					struct TESBound { std::int16_t x, y, z; };

					constexpr static std::uint8_t TYPE_ID = ftReserved000;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESBoundObject[0];

					// CK 1.11.246.0: 154 virtuals, vtable RVA 0x30920B8.
					virtual ~TESBoundObject() = default;

					/* 1B0 */ void SaveObjectBound() override;
					/* 1B8 */ void LoadObjectBound(TESFile* pFile) override;
					/* 1C0 */ void SavePreviewTransform() override;
					/* 1C8 */ void LoadPreviewTransform(TESFile* pFile) override;
					/* 1D0 */ [[nodiscard]] bool IsBoundObject() const noexcept(true) override;
					/* 200 */ std::uint32_t GetRefCount() const noexcept(true) override;
					/* 3B0 */ [[nodiscard]] BGSMod::Template::Items* GetObjectTemplate() const noexcept(true) override;
					/* 3B8 */ [[nodiscard]] BGSPreviewTransform* GetPreviewTransform() const noexcept(true) override;
					/* 3C0 */ [[nodiscard]] BGSSoundTagComponent* GetSoundTagComponent() const noexcept(true) override;
					/* 400 */ void Clone3D(TESObjectREFR* pRequester, NiAPI::NiPointer<NiAPI::NiAVObject>* pObj3D, bool bBackground) noexcept(true) override;
					/* 420 */ bool ReplaceModel() noexcept(true) override;
					/* 428 */ std::uint32_t IncRef() noexcept(true) override;
					/* 430 */ std::uint32_t DecRef() noexcept(true) override;
					virtual TBO_InstanceData* CreateInstanceData(const TBO_InstanceData* pData) const noexcept(true) { return nullptr; } // 136 - RVA 0x436870
					virtual const TBO_InstanceData* GetBaseInstanceData() const noexcept(true) { return nullptr; } // 137 - RVA 0x4368B0
					virtual bool GetAttachedMarkersVisible(void* pObj3D) const noexcept(true) { return false; } // 138 - RVA 0xC5A210
					virtual void SetAttachedMarkersVisible(void* pObj3D, bool bVisible) const noexcept(true) { return; } // 139 - RVA 0xC5A220
					virtual bool QSupportsCountPerInstance() const noexcept(true) { return false; } // 140 - RVA 0x4369C0
					virtual void SetObjectVoiceType(BGSVoiceType* pVoiceType) noexcept(true) { return; } // 141 - RVA 0x4369E0
					virtual BGSVoiceType* GetObjectVoiceType() const noexcept(true) { return nullptr; } // 142 - RVA 0x4368D0
					[[nodiscard]] virtual BGSSoundDescriptorForm* GetSoundForTag(const BSFixedString& Tag) const noexcept(true) { return nullptr; } // 143
					virtual void Clone3D(TESObjectREFR* pRequester, NiAPI::NiPointer<NiAPI::NiAVObject>* pObj3D) noexcept(true) { return; } // 144 - RVA 0xC5A5D0
					virtual bool ReplaceModel(const char* lpFileName) noexcept(true) { return false; } // 145 - RVA 0xC5A720
					virtual void UpdatePreview(HWND hDialogBox, bool ForceUpdate) noexcept(true) { return; } // 146
					virtual void CollectInventoryItems(TESContainer* Inventory, std::uint16_t Level, const ContainerItemExtra* ExtraData) noexcept(true) { return; } // 147
					virtual void CollectVoiceTypes(BSSimpleList<BGSVoiceType*>* VoiceTypes) const noexcept(true) { return; } // 148
					virtual void HandleRemoveItemFromContainer(TESObjectREFR* Reference) noexcept(true) { return; } // 149
					virtual void OnRemove3D(NiAPI::NiAVObject* Object3D) noexcept(true) { return; } // 150
					virtual void OnCheckModels() noexcept(true) { return; } // 151
					virtual void OnCopyReference() noexcept(true) { return; } // 152
					virtual void OnFinishScale() noexcept(true) { return; } // 153

				private:
					TESBound _BoundMin; // 40
					TESBound _BoundMax; // 46
					char pad4C[4]; // 4C
					BGSMod::Template::Items _templateItems; // 50 - ???
					BGSPreviewTransform _PreviewTransform; // 70
					BGSSoundTagComponent _SoundTag; // 80 - 8 bytes in the CK
					std::uint32_t _unk88; // 88
					char pad8C[4]; // 8C
				public:
					[[nodiscard]] inline TESBound GetBoundMin() const noexcept(true) { return _BoundMin; }
					[[nodiscard]] inline TESBound GetBoundMax() const noexcept(true) { return _BoundMax; }

					/*
					Declare TESForm 
					[[nodiscard]] inline const BGSPreviewTransform* GetPreviewTransform() const noexcept(true) { return &_PreviewTransform; }
					[[nodiscard]] inline const BGSSoundTagComponent* GetSoundTagComponent() const noexcept(true) { return &_SoundTag; }*/
				public:
					CKPE_READ_PROPERTY(GetBoundMin) TESBound BoundMin;
					CKPE_READ_PROPERTY(GetBoundMax) TESBound BoundMax;
				};
				static_assert(sizeof(TESBoundObject) == 0x90);

				// 0x90
				class TESBoundAnimObject :
					public TESBoundObject   // 000
				{};
				static_assert(sizeof(TESBoundAnimObject) == 0x90);

				// 0x90
				class TESBoundTreeObject : public TESBoundObject 
				{};
				static_assert(sizeof(TESBoundTreeObject) == 0x90);
			}
		}
	}
}
