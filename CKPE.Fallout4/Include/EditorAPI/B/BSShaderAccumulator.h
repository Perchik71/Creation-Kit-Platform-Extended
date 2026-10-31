// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSBatchRenderer.h>
#include <EditorAPI/N/NiAlphaAccumulator.h>
#include <EditorAPI/N/NiColor.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/N/NiPointer.h>

namespace CKPE::Fallout4::EditorAPI
{
	class BSLight;
	class ShadowSceneNode;

	class BSShaderAccumulator : public NiAPI::NiAlphaAccumulator
	{
	public:
		/* 02 */ virtual NiAPI::NiRTTI* GetRTTI() const noexcept(true) override;
		constexpr static Common::ID VTABLE = IDs::VTABLE::BSShaderAccumulator[0];

		/* 30 */ virtual void FinishAccumulatingPreResolveDepth();
		/* 31 */ virtual void FinishAccumulatingPostResolveDepth();

		/* 0x058 */ std::byte _pad058[0x68];
		/* 0x0C0 */ std::uint32_t sunPixelCount;
		/* 0x0C4 */ bool waitingForSunQuery;
		/* 0x0C5 */ std::byte _pad0C5[3];
		/* 0x0C8 */ float percentSunOccludedStored;
		/* 0x0CC */ std::byte _pad0CC[4];
		/* 0x0D0 */ std::byte _sunTests[3][0x18];
		/* 0x118 */ bool firstPerson;
		/* 0x119 */ bool zPrePass;
		/* 0x11A */ std::byte _pad11A[2];
		/* 0x11C */ NiAPI::NiColorA silhouetteColor;
		/* 0x12C */ bool renderDecals;
		/* 0x12D */ std::byte _pad12D[3];
		/* 0x130 */ BSBatchRenderer batchRenderer;
		/* 0x5B8 */ std::uint32_t currentPass;
		/* 0x5BC */ std::uint32_t currentBucket;
		/* 0x5C0 */ bool currentActive;
		/* 0x5C1 */ std::byte _pad5C1[7];
		/* 0x5C8 */ ShadowSceneNode* activeShadowSceneNode;
		/* 0x5D0 */ std::uint32_t renderMode;
		/* 0x5D4 */ std::byte _pad5D4[4];
		/* 0x5D8 */ NiAPI::NiPointer<BSLight> shadowLight;
		/* 0x5E0 */ NiAPI::NiPoint3A eyePosition;
		/* 0x5F0 */ std::uint32_t depthPassIndex;
		/* 0x5F4 */ std::byte _pad5F4[0x4C];
	};
	static_assert(sizeof(BSShaderAccumulator) == 0x640);
	static_assert(offsetof(BSShaderAccumulator, firstPerson) == 0x118);
	static_assert(offsetof(BSShaderAccumulator, batchRenderer) == 0x130);
	static_assert(offsetof(BSShaderAccumulator, currentPass) == 0x5B8);
	static_assert(offsetof(BSShaderAccumulator, shadowLight) == 0x5D8);
	static_assert(offsetof(BSShaderAccumulator, eyePosition) == 0x5E0);
}
