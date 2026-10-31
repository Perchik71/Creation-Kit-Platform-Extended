// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>
#include <EditorAPI/IDs_VTABLE.h>
#include <EditorAPI/N/NiPoint.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BGSRenderWindow;
			class BGSRenderWindowEditModule
			{
			public:
				virtual ~BGSRenderWindowEditModule() = default;
				constexpr static Common::ID VTABLE = IDs::VTABLE::BGSRenderWindowEditModule[0];

				[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
				{
					return offsetof(BGSRenderWindowEditModule, _Renderer) == 0x8 &&
						offsetof(BGSRenderWindowEditModule, _TempPosition) == 0x70;
				}
			private:
				/* 0x008 */ virtual void sub_008();
				/* 0x010 */ virtual void sub_010();
				/* 0x018 */ virtual void sub_018();
public:
				/* 0x020 */ virtual void HandleKey(std::uint8_t key); // shift (16) clears a flag, escape (27) cancels the current mode
				/* 0x028 */ virtual bool MovePlusY(); // passes a 2048 unit offset along +Y to the helper on the renderer member at 0xC0, returns true
				/* 0x030 */ virtual bool MoveMinusY(); // passes a 2048 unit offset along -Y to the helper on the renderer member at 0xC0, returns true
				/* 0x038 */ virtual bool MoveMinusX(); // passes a 2048 unit offset along -X to the helper on the renderer member at 0xC0, returns true
				/* 0x040 */ virtual bool MovePlusX(); // passes a 2048 unit offset along +X to the helper on the renderer member at 0xC0, returns true
				/* 0x048 */ virtual bool MovePlusZ(); // passes a 2048 unit offset along +Z to the helper on the renderer member at 0xC0, returns true
				/* 0x050 */ virtual bool MoveMinusZ(); // passes a 2048 unit offset along -Z to the helper on the renderer member at 0xC0, returns true
private:
				/* 0x058 */ virtual void sub_058();
				/* 0x060 */ virtual void sub_060();
				/* 0x068 */ virtual void sub_068();
				/* 0x070 */ virtual void sub_070();
				/* 0x078 */ virtual void sub_078();
				/* 0x080 */ virtual void sub_080();
				/* 0x088 */ virtual void sub_088();
				/* 0x090 */ virtual void sub_090();
				/* 0x098 */ virtual void sub_098();
				/* 0x0A0 */ virtual void sub_0A0();
				/* 0x0A8 */ virtual void sub_0A8();
				/* 0x0B0 */ virtual void sub_0B0();
				/* 0x0B8 */ virtual void sub_0B8();
				/* 0x0C0 */ virtual void sub_0C0();
				/* 0x0C8 */ virtual void sub_0C8();
				/* 0x0D0 */ virtual void sub_0D0();
				/* 0x0D8 */ virtual void sub_0D8();
				/* 0x0E0 */ virtual void sub_0E0();
				/* 0x0E8 */ virtual void sub_0E8();
				/* 0x0F0 */ virtual void sub_0F0();
				/* 0x0F8 */ virtual void sub_0F8();
				/* 0x100 */ virtual void sub_100();
				/* 0x108 */ virtual void sub_108();
				/* 0x110 */ virtual void sub_110();
				/* 0x118 */ virtual void sub_118();
				/* 0x120 */ virtual void sub_120();
				/* 0x128 */ virtual void sub_128();
				/* 0x130 */ virtual void sub_130();
				/* 0x138 */ virtual void sub_138();
				/* 0x140 */ virtual void sub_140();
				/* 0x148 */ virtual void sub_148();
				/* 0x150 */ virtual void sub_150();
				/* 0x158 */ virtual void sub_158();
				/* 0x160 */ virtual void sub_160();
				/* 0x168 */ virtual void sub_168();
				/* 0x170 */ virtual void sub_170();
				/* 0x178 */ virtual void sub_178();
				/* 0x180 */ virtual void sub_180();
				/* 0x188 */ virtual void sub_188();
				/* 0x190 */ virtual void sub_190();
				/* 0x198 */ virtual void sub_198();
				/* 0x1A0 */ virtual void sub_1A0();
				/* 0x1A8 */ virtual void sub_1A8();
				/* 0x1B0 */ virtual void sub_1B0();
				/* 0x1B8 */ virtual void sub_1B8();
				/* 0x1C0 */ virtual void sub_1C0();
				/* 0x1C8 */ virtual void sub_1C8();
				/* 0x1D0 */ virtual void sub_1D0();
				/* 0x1D8 */ virtual void sub_1D8();
				/* 0x1E0 */ virtual void sub_1E0();
				/* 0x1E8 */ virtual void sub_1E8();
				/* 0x1F0 */ virtual void sub_1F0();
				/* 0x1F8 */ virtual void sub_1F8();
				/* 0x200 */ virtual void sub_200();
				/* 0x208 */ virtual void sub_208();
				/* 0x210 */ virtual void sub_210();
				/* 0x218 */ virtual void sub_218();
public:
				/* 0x220 */ [[nodiscard]] virtual bool IsModeActive() const; // the mode value at 0x1C is greater than 2
private:
				/* 0x228 */ virtual void sub_228();
				/* 0x230 */ virtual void sub_230();
				/* 0x238 */ virtual void sub_238();
				//									00 - vtable
				BGSRenderWindow* _Renderer;		//	08 - render	
				char pad10[0x60];				//	10
				NiAPI::NiPoint3 _TempPosition;	//	70 - position	
				char pad7C[0x4];				//	7C
				char pad80[0x30];				//	80
			};
			static_assert(sizeof(BGSRenderWindowEditModule) == 0xB0);
			static_assert(BGSRenderWindowEditModule::VerifyLayout());

			class BGSRenderWindowReferenceEditModule : public BGSRenderWindowEditModule
			{
			public:
				virtual ~BGSRenderWindowReferenceEditModule() = default;
				[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
				{
					return BGSRenderWindowEditModule::VerifyLayout() &&
						offsetof(BGSRenderWindowReferenceEditModule, pad0B0) == 0xB0;
				}
			private:
				/* 0x240 */ virtual void sub_240();

				/* 0x0B0 */ char pad0B0[0x78];
			};
			static_assert(sizeof(BGSRenderWindowReferenceEditModule) == 0x128);
			static_assert(BGSRenderWindowReferenceEditModule::VerifyLayout());
		}
	}
}