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
				// 1.11.240.0: size 0x48, vtable RVA 0x3060DD0, 124 virtual functions
				class BGSReverbParameters :
					public TESForm,        // 000
					public BSIReverbType   // 028
				{
				public:
					virtual ~BGSReverbParameters() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 330 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftReverbParameters;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSReverbParameters[0];
				public:
					// DATA chunk (14 bytes); Load clamps decayTime to 100-20000 and hfReference to 20-20000
					struct ReverbParams
					{
						std::uint16_t decayTime; // 0
						std::uint16_t hfReference; // 2
						std::int8_t roomFilter; // 4
						std::int8_t roomHFFilter; // 5
						std::int8_t reflections; // 6
						std::int8_t reverb; // 7
						std::int8_t decayHFRatio; // 8
						std::int8_t reflectionDelay; // 9
						std::int8_t reverbDelay; // A
						std::int8_t diffusionPct; // B
						std::int8_t densityPct; // C
						std::int8_t pad; // D
					};
					static_assert(sizeof(ReverbParams) == 0xE);

					ReverbParams data; // 30
					std::uint8_t pad3E[0x2]; // 3E
					std::uint32_t reverbClass; // 40
					std::uint32_t unk44; // 44
				};
				static_assert(sizeof(BGSReverbParameters) == 0x48);
				static_assert(offsetof(BGSReverbParameters, data) == 0x30);
				static_assert(offsetof(BGSReverbParameters, reverbClass) == 0x40);
			}
		}
	}
}
