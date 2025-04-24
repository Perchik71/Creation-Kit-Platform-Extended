// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "RegistratorWindow.h"
#include "UITheme/VarCommon.h"
#include "Patches/UIThemePatch.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		RegistratorWindow* GlobalRegistratorWindowPtr = nullptr;

		RegistratorWindow::~RegistratorWindow()
		{
			Clear();
		}

		bool RegistratorWindow::Has(HWND hWnd) const
		{ 
			std::lock_guard lock(const_cast<RegistratorWindow*>(this)->_lock);
			return std::find(_aWnds.begin(), _aWnds.end(), hWnd) != _aWnds.end();
		}

		bool RegistratorWindow::HasMajor(HWND hWnd) const
		{
			std::lock_guard lock(const_cast<RegistratorWindow*>(this)->_lock);
			return _aMajorWnds.find(hWnd) != _aMajorWnds.end();
		}

		HWND RegistratorWindow::Register(HWND hWnd)
		{
			std::lock_guard lock(_lock);

			if (Has(hWnd) || !IsWindow(hWnd) || IsChildWindow(hWnd))
				return hWnd;

			_aWnds.push_back(hWnd);
			// Регистрируем индетификатор потока или инкреминируем счётчик
			AddWndThread(hWnd);
			SendMessageForUITheme(hWnd);

			return hWnd;
		}

		HWND RegistratorWindow::RegisterMajor(HWND hWnd, const char* sName)
		{
			std::lock_guard lock(_lock);

			if (HasMajor(hWnd) || !IsWindow(hWnd) || IsChildWindow(hWnd))
				return hWnd;

			_aMajorWnds.insert(std::pair<HWND, String>(hWnd, sName));
			// Регистрируем индетификатор потока или инкреминируем счётчик
			AddWndThread(hWnd);
			SendMessageForUITheme(hWnd);

			return hWnd;
		}

		bool RegistratorWindow::Unregister(HWND hWnd, bool Major)
		{
			std::lock_guard lock(_lock);

			if (Major)
			{
				auto it = _aMajorWnds.find(hWnd);
				if (it != _aMajorWnds.end())
				{
					RemoveWndThread(hWnd);
					_aMajorWnds.erase(it);
					return true;
				}
			}
			else
			{
				auto it = std::find(_aWnds.begin(), _aWnds.end(), hWnd);
				if (it != _aWnds.end())
				{
					RemoveWndThread(hWnd);
					_aWnds.erase(it);
					return true;
				}
			}

			return false;
		}

		bool RegistratorWindow::IsChildWindow(HWND hWnd)
		{
			return (GetWindowLongPtr(hWnd, GWL_STYLE) & WS_CHILD) == WS_CHILD;
		}

		HWND RegistratorWindow::Get(uint32_t Id) const
		{
			std::lock_guard lock(const_cast<RegistratorWindow*>(this)->_lock);
			return (Count() > Id) ? _aWnds[Id] : NULL;
		}

		HWND RegistratorWindow::GetMajor(const char* sName) const
		{
			std::lock_guard lock(const_cast<RegistratorWindow*>(this)->_lock);

			for (auto it = _aMajorWnds.begin(); it != _aMajorWnds.end(); it++)
			{
				if (!_strcmpi(it->second.c_str(), sName))
					return it->first;
			}

			return NULL;
		}

		void RegistratorWindow::CloseWindowExceptForMajor(HWND hCallWnd) const
		{
			std::lock_guard lock(const_cast<RegistratorWindow*>(this)->_lock);

			String sClassName;
			sClassName.resize(MAX_PATH);

			for (auto itWnd = _aWnds.begin(); itWnd != _aWnds.end(); itWnd++)
			{
				// Если окно есть в списке главных пропускаем
				// Пропускаем дочерние окна (скорее всего это элементы управления)
				if ((*itWnd == hCallWnd) || HasMajor(*itWnd) || IsChildWindow(*itWnd))
					continue;

				// Типо нажимаем "крестик"
				SendMessage(*itWnd, WM_CLOSE, 0, 0);
			}
		}

		void RegistratorWindow::AddWndThread(HWND hWnd)
		{
			// Извлекаем индетификатор потока, который создал окно из HWND
			auto ThreadId = (uint32_t)GetWindowThreadProcessId(hWnd, NULL);

			auto It = _aThreadWnds.find(ThreadId);
			if (It == _aThreadWnds.end())
			{
				_aThreadWnds.insert(std::make_pair(ThreadId, (uint32_t)1));
				// Вызов для всех слушателей
				for(auto Invoke : _aRegInvoke)
					Invoke(ThreadId);
			}
			else
				It->second++;
		}

		void RegistratorWindow::RemoveWndThread(HWND hWnd)
		{
			// Извлекаем индетификатор потока, который создал окно из HWND
			auto ThreadId = (uint32_t)GetWindowThreadProcessId(hWnd, NULL);

			auto It = _aThreadWnds.find(ThreadId);
			if (It == _aThreadWnds.end())
				_ERROR("There is no registered window with such a thread index [%u]", ThreadId);
			else
			{
				if (It->second == 1)
				{
					_aThreadWnds.erase(It);
					// Вызов для всех слушателей
					for (auto Invoke : _aUnregInvoke)
						Invoke(ThreadId);
				}
				else
					It->second--;
			}
		}

		void RegistratorWindow::SendMessageForUITheme(HWND hWnd)
		{
			if (UITheme::IsDarkThemeSystemSupported() && UITheme::IsDarkTheme())
				Patches::UIThemePatch::ApplyThemeForWindow(hWnd);
		}
	}
}