// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/E/EmbeddedChildDialog.h>

namespace CKPE::Fallout4::EditorAPI
{
	namespace Forms
	{
		class TESWaterForm;
		struct WaterShaderData;
	}

	namespace TESWaterFormEditor
	{
		class WaterPropertiesTab : public Dialog::EmbeddedChildDialog
		{
		public:
			/* 0x000 */ ~WaterPropertiesTab() override;
			/* 0x010 */ std::int32_t DlgProc(UINT Message, WPARAM wParam,
				LPARAM lParam) noexcept(true) override;

		protected:
			/* 0x018 */ virtual void Unk018();
			/* 0x020 */ virtual void Unk020();
			/* 0x028 */ virtual void Unk028();

		public:
			/* 0x030 */ Forms::TESWaterForm* WaterForm;
			/* 0x038 */ Forms::WaterShaderData* WaterData;
			/* 0x040 */ std::uint8_t pad040[0xB0];
		};
		static_assert(sizeof(WaterPropertiesTab) == 0xF0);
		static_assert(offsetof(WaterPropertiesTab, WaterForm) == 0x30);
		static_assert(offsetof(WaterPropertiesTab, WaterData) == 0x38);

		class FogProperties : public WaterPropertiesTab
		{
		public:
			enum ControlID : std::uint32_t
			{
				kMaxDepth = 2430,
				kShallowFogColorRange = 6249,
				kDeepFogColorRange = 6252,
				kShallowAlpha = 6253,
				kDeepAlpha = 2746,
				kShallowFogAlphaRange = 2429,
				kDeepFogAlphaRange = 5697,
				kUnderwaterFogAmount = 2745,
				kUnderwaterFogDistNear = 2426,
				kUnderwaterFogDistFar = 2427
			};

			/* 0x000 */ ~FogProperties() override;
		};
		static_assert(sizeof(FogProperties) == 0xF0);

		class NoiseProperties : public WaterPropertiesTab
		{
		public:
			/* 0x000 */ ~NoiseProperties() override;
			/* 0x010 */ std::int32_t DlgProc(UINT Message, WPARAM wParam,
				LPARAM lParam) noexcept(true) override;

		protected:
			/* 0x018 */ void Unk018() override;
			/* 0x020 */ void Unk020() override;
			/* 0x028 */ void Unk028() override;
		};
		static_assert(sizeof(NoiseProperties) == 0xF0);

		class PhysicalProperties : public WaterPropertiesTab
		{
		public:
			/* 0x000 */ ~PhysicalProperties() override;
			/* 0x010 */ std::int32_t DlgProc(UINT Message, WPARAM wParam,
				LPARAM lParam) noexcept(true) override;

		protected:
			/* 0x018 */ void Unk018() override;
		};
		static_assert(sizeof(PhysicalProperties) == 0xF0);

		class SiltProperties : public WaterPropertiesTab
		{
		public:
			/* 0x000 */ ~SiltProperties() override;
		};
		static_assert(sizeof(SiltProperties) == 0xF0);

		class SpecularProperties : public WaterPropertiesTab
		{
		public:
			/* 0x000 */ ~SpecularProperties() override;
		};
		static_assert(sizeof(SpecularProperties) == 0xF0);
	}
}
