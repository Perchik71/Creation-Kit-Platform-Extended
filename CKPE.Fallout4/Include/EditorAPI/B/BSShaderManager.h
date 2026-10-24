// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/IDs.h>
#include <cstddef>
#include <cstdint>
#include <CKPE.Common.Relocation.h>
#include <EditorAPI/N/NiColor.h>
#include <EditorAPI/N/NiTransform.h>

namespace CKPE::Fallout4::EditorAPI
{
	class BSShader;
	class ShadowSceneNode;
	namespace NiAPI { class NiCamera; }

	class BSShaderManager
	{
	public:
		enum class TimerMode : std::uint32_t
		{
			Default = 0,
			Delta = 1,
			System = 2,
			RealDelta = 3,
			FrameCount = 4,
			AccumulatedDelta = 5,
			Total = 6
		};

		struct alignas(0x10) State
		{
			[[nodiscard]] static State* GetSingleton() noexcept(true)
			{
				static Common::Relocation<State*> instance{ EditorAPI::IDs::BSShaderManager::State::Singleton };
				return instance.Get();
			}

			/* 0x000 */ ShadowSceneNode* shadowSceneNodes[5];
			/* 0x028 */ float timerValues[6];
			/* 0x040 */ std::uint32_t frameCount;
			/* 0x044 */ NiAPI::NiColorA loadedRange;
			/* 0x054 */ std::byte _pad054[0x7C];
			/* 0x0D0 */ NiAPI::NiTransform directionalAmbientTransform;
			/* 0x110 */ std::byte _pad110[0x2F0];
		};
		static_assert(sizeof(State) == 0x400);
		static_assert(offsetof(State, timerValues) == 0x28);
		static_assert(offsetof(State, frameCount) == 0x40);
		static_assert(offsetof(State, directionalAmbientTransform) == 0xD0);

		[[nodiscard]] static const NiAPI::NiCamera* GetCurrentCamera() noexcept(true)
		{
			static Common::Relocation<const NiAPI::NiCamera**> camera{
				EditorAPI::IDs::BSShaderManager::CurrentCamera };
			return *camera;
		}

		[[nodiscard]] static BSShader* GetShader(std::uint32_t Index) noexcept(true)
		{
			using Function = BSShader* (*)(std::uint32_t);
			static Common::Relocation<Function> function{
				EditorAPI::IDs::BSShaderManager::GetShader };
			return function(Index);
		}
	};
}
