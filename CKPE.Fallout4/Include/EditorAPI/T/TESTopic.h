// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.h>
#include <EditorAPI/N/NiTSimpleArray.h>
#include <EditorAPI/T/TESTopicInfo.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESQuest;
				// size = 0x98
				// func = 124
				class TESTopic : public TESForm
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftTopic;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESTopic[0];

					enum SubtypeT : std::uint8_t
					{
						stObserveCombat = 0x54,
						stNoticeCorpose = 0x55,
						stTimeToGo = 0x56,
						stGoodbye = 0x57,
						stSwingMeleeWeapon = 0x59,
						stShootBow = 0x5A,
						stZKeyObject = 0x5B,
						stJump = 0x5C,
						stKnockOverObject = 0x5D,
						stDestroyObject = 0x5E,
						stStandonFurniture = 0x5F,
						stLockedObject = 0x60,
						stPickpocketTopic = 0x61,
						stPursueIdleTopic = 0x62,
						stSceneChoice = 0x64,
						stPlayerCastProjectileSpell = 0x65,
						stPlayerCastSelfSpell = 0x66,
						stPlayerShout = 0x67,
						stIdle = 0x68,
						stEnterSprintBreath = 0x69,
						stEnterBowZoomBreath = 0x6A,
						stExitBowZoomBreath = 0x6B,
						stActorCollidewithActor = 0x6C,
						stPlayerinIronSights = 0x6D,
						stOutofBreath = 0x6E,
						stCombatGrunt = 0x6F,
						stLeaveWaterBreath = 0x70,
						stImpatientPositive = 0x71,
						stImpatientNegative = 0x72,
						stImpatientNatural = 0x73,
						stImpatientQuestion = 0x74,
						stGreeting = 0x76,
						stPlayerActiveDoor = 0x77,
						stPlayerActiveTerminals = 0x78,
						stPlayerActiveFurniture = 0x79,
						stPlayerActiveActivators = 0x7A,
						stPlayerActiveContainer = 0x7B,
						stPlayerAquireFeaturedItem = 0x7C,
					};

					virtual ~TESTopic() = default;

					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 0D8 */ bool SavePlugin(TESFile* File) override;
					/* 0E8 */ bool SaveForm(TESFile* File) override;
					/* 0F0 */ [[nodiscard]] bool IsBeforeGroup(const TESGroupHeader* GroupRecord) const noexcept(true) override;
					/* 0F8 */ [[nodiscard]] bool IsBeforeForm(TESForm* Rhs) const noexcept(true) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 1A0 */ void MarkAsChanged(bool bValue) noexcept(true) override;
					/* 220 */ bool BelongsInGroup(TESGroupHeader* Group, bool bAllowParentGroups, bool bCurrentOnly) override;
					/* 228 */ void CreateGroupData(TESGroupHeader* OutGroup, TESGroupHeader* ParentGroup) override;
					/* 240 */ [[nodiscard]] bool IsParentForm() const noexcept(true) override;
					/* 250 */ [[nodiscard]] bool IsFormTypeChild(FormType Type) const noexcept(true) override;
					/* 270 */ [[nodiscard]] bool HasAlteredChildren() const noexcept(true) override;
					/* 280 */ bool RemoveChildFormsFromList(CheckInFormList* Forms) noexcept(true) override;
					/* 288 */ bool CanCheckOutChildren() noexcept(true) override;
					/* 290 */ bool CheckOutChildren(TESBitArrayFile* UserFile, TESBitArrayFile* VersionFile) noexcept(true) override;
					/* 298 */ bool MarkChildFormsAsDeleted(bool bValue, TESBitArrayFile* UserFile, TESBitArrayFile* VersionFile) noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2A8 */ [[nodiscard]] bool CanCheckIn(const CheckInFormList* Forms) const noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 340 */ void EditFormToWindow(HWND hParentWnd, bool bModalWindow, [[maybe_unused]] bool bReserved, std::int32_t nObjectTemplateRequest) noexcept(true) override;
					/* 360 */ [[nodiscard]] bool HasRequiredUniqueEditorID() const noexcept(true) override;
				public:
					inline const char* GetTopicText() const { return _TopicText.c_str(); }
					inline SubtypeT GetSubtype() const { return _Subtype; }
					inline std::uint8_t GetPriority() const { return _Priority; }
					inline TESQuest* GetQuest() const { return _Quest[0]; }
					inline NiAPI::NiTSimpleArray<TESTopicInfo*>& GetInfos() { return _Infos; }

					CKPE_READ_PROPERTY(GetTopicText) const char* TopicText;
					CKPE_READ_PROPERTY(GetSubtype) SubtypeT Subtype;
					CKPE_READ_PROPERTY(GetPriority) std::uint8_t Priority;
					CKPE_READ_PROPERTY(GetQuest) TESQuest* Quest;
					CKPE_READ_PROPERTY(GetInfos) NiAPI::NiTSimpleArray<TESTopicInfo*>& Infos;
				private:
					char pad28[0x8];
					BGSLocalizedStringIL _TopicText;
					char pad40[0x2];
					SubtypeT _Subtype;
					char pad43[0x4];
					std::uint8_t _Priority;
					char pad48[0x8];
					TESQuest* _Quest[2];
					char pad60[0x8];
					NiAPI::NiTSimpleArray<TESTopicInfo*> _Infos;
					char pad78[0x20];
				};
				static_assert(sizeof(TESTopic) == 0x98);
			}
		}
	}
}