// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <string>
#include <CKPE.Process.h>
#include <CKPE.GameManager.h>

namespace CKPE
{
	class CKPE_API Application : public Process
	{
		bool _init{ false };
		

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
	public:
		Application() noexcept(true);
		~Application() noexcept(true);

		[[nodiscard]] static const Application* GetSingleton() noexcept(true);

		virtual void MessageProcessing() const noexcept(true);
		virtual void Initialize() const;
		virtual void Shutdown() const;

		[[nodiscard]] virtual GameManager::Game GetGameType() const noexcept(true);
		[[nodiscard]] constexpr virtual bool IsSkyrim() const noexcept(true)
		{ return GetGameType() == GameManager::Game::CK_SKYRIMSE; }
		[[nodiscard]] constexpr virtual bool IsFallout4() const noexcept(true)
		{ return GetGameType() == GameManager::Game::CK_FALLOUT4; }
		[[nodiscard]] constexpr virtual bool IsStarfield() const noexcept(true)
		{ return GetGameType() == GameManager::Game::CK_STARFIELD; }
	};
}