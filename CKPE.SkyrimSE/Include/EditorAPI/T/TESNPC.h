// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/T/TESActorBase.h>
#include <EditorAPI/B/TESRaceForm.h>
#include <EditorAPI/B/BGSOverridePackCollection.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x2E8, vtable RVA 0x30AF678 (NPC_)
				class TESNPC :
					public TESActorBase,				// 000
					public TESRaceForm,					// 178
					public BGSOverridePackCollection	// 188
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftNPC;

					virtual ~TESNPC() = default;

					/* 070 */ void InitializeData() noexcept(true) override;
					/* 078 */ void ClearData() noexcept(true) override;
					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 0A0 */ void OnPluginSaved(TESFile* File) noexcept(true) override;
					/* 0E0 */ void InitItemImpl() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 220 */ void GetObjectWindowTreePath(void* Path) const noexcept(true) override;
					/* 248 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 250 */ [[nodiscard]] bool CanCheckIn(const void* Forms) const noexcept(true) override;
					/* 258 */ void BuildCheckInDependencyMessage(const void* Forms, void* Message) const noexcept(true) override;
					/* 2A0 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) noexcept(true) override;
					/* 2A8 */ [[nodiscard]] bool IsSuitableDialogBox(HWND hDialogBox) const noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C8 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;
				public:
					std::uint8_t pad1B0[0x30];	// 1B0
					void* cnam;	// 1E0 - CNAM
					void* ftst;	// 1E8 - FTST
					void* gnam;	// 1F0 - GNAM
					std::uint8_t pad1F8[0x18];	// 1F8
					std::uint8_t shrt[0x10];	// 210 - SHRT
					std::uint8_t anam[0x18];	// 220 - ANAM
					void* doft;	// 238 - DOFT
					void* soft;	// 240 - SOFT
					void* dplt;	// 248 - DPLT
					void* crif;	// 250 - CRIF
					std::uint8_t pad258[0xD];	// 258
					std::uint8_t nam8;	// 265 - NAM8
					std::uint8_t pad266[0x1A];	// 266
					std::uint8_t nam9[0x4C];	// 280 - NAM9
					std::uint8_t nama[0x14];	// 2CC - NAMA
					void* unk2E0;	// 2E0 - TIAS/TINC/TINI/TINV
				};
				static_assert(offsetof(TESNPC, pad1B0) == 0x1B0);
				static_assert(offsetof(TESNPC, cnam) == 0x1E0);
				static_assert(offsetof(TESNPC, ftst) == 0x1E8);
				static_assert(offsetof(TESNPC, gnam) == 0x1F0);
				static_assert(offsetof(TESNPC, shrt) == 0x210);
				static_assert(offsetof(TESNPC, anam) == 0x220);
				static_assert(offsetof(TESNPC, doft) == 0x238);
				static_assert(offsetof(TESNPC, soft) == 0x240);
				static_assert(offsetof(TESNPC, dplt) == 0x248);
				static_assert(offsetof(TESNPC, crif) == 0x250);
				static_assert(offsetof(TESNPC, nam8) == 0x265);
				static_assert(offsetof(TESNPC, nam9) == 0x280);
				static_assert(offsetof(TESNPC, nama) == 0x2CC);
				static_assert(offsetof(TESNPC, unk2E0) == 0x2E0);
				static_assert(sizeof(TESNPC) == 0x2E8);
			}
		}
	}
}