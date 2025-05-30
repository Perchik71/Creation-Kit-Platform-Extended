// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <string>
#include <CKPE.Process.h>

namespace CKPE
{
	class CKPE_API Application : public Process
	{
		bool _init{ false };
		std::wstring* _fname{ nullptr };
		std::wstring* _fpath{ nullptr };

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
	public:
		Application() noexcept(true);
		~Application() noexcept(true);

		[[nodiscard]] inline constexpr virtual const wchar_t* GetFileName() const noexcept(true) 
		{ return _fname ? _fname->c_str() : L""; }
		[[nodiscard]] inline constexpr virtual const wchar_t* GetPath() const noexcept(true) 
		{ return _fpath ? _fpath->c_str() : L""; }
		[[nodiscard]] static const Application* GetSingleton() noexcept(true);

		virtual void MessageProcessing() const noexcept(true);
		virtual void Initialize() const;
		virtual void Shutdown() const;
	};
}