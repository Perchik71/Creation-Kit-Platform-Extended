// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <windows.h>
#include <CKPE.Application.h>
#include <CKPE.PathUtils.h>
#include <CKPE.Logger.h>

namespace CKPE
{
	static Application* _sapp;

	Application::Application() noexcept(true) :
		Process()
	{
		_fname = new std::wstring;
		_fpath = new std::wstring;

		if (_fname)
			*_fname = PathUtils::GetApplicationFileName();

		if (_fpath)
			*_fpath = PathUtils::GetApplicationPath();
	}

	Application::~Application() noexcept(true)
	{
		if (_fname)
		{
			delete _fname;
			_fname = nullptr;
		}

		if (_fpath)
		{
			delete _fpath;
			_fpath = nullptr;
		}

		auto slog = const_cast<Logger*>(Logger::GetSingleton());
		slog->Close();
	}

	const Application* Application::GetSingleton() noexcept(true)
	{
		if (!_sapp) _sapp = new Application;
		return _sapp;
	}

	void Application::MessageProcessing() const noexcept(true)
	{
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void Application::Initialize() const
	{
		if (_init) return;
		auto _This = const_cast<Application*>(this);
		_This->_init = true;

		auto slog = const_cast<Logger*>(Logger::GetSingleton());
		if (!slog->Open(std::wstring(GetPath()) + L"CreationKitPlatformExtended.log"))
			throw std::runtime_error("Failed to create a file: CreationKitPlatformExtended.log");

		_This->LoadInfo();
	}

	void Application::Shutdown() const
	{
		if (_sapp)
		{
			delete _sapp;
			_sapp = nullptr;
		}
	}
}