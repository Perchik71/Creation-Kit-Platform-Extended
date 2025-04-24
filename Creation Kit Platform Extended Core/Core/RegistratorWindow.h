// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		class RegistratorWindow
		{
		public:
			typedef void(*SignalType)(uint32_t);

			RegistratorWindow() = default;
			~RegistratorWindow();
		
			inline void Clear() { _aWnds.clear(); _aWnds.clear(); }
			virtual bool Has(HWND hWnd) const;
			virtual bool HasMajor(HWND hWnd) const;
			HWND Register(HWND hWnd);
			HWND RegisterMajor(HWND hWnd, const char* sName);
			bool Unregister(HWND hWnd, bool Major = false);
			static bool IsChildWindow(HWND hWnd);
			virtual HWND Get(uint32_t Id) const;
			virtual HWND GetMajor(const char* sName) const;
			inline uint32_t Count() const { return (uint32_t)_aWnds.size(); }
			inline uint32_t CountMajor() const { return (uint32_t)_aMajorWnds.size(); }
			inline Array<HWND>::iterator Begin() { return _aWnds.begin(); }
			inline Array<HWND>::iterator End() { return _aWnds.end(); }
			inline Map<HWND, String>::iterator BeginMajor() { return _aMajorWnds.begin(); }
			inline Map<HWND, String>::iterator EndMajor() { return _aMajorWnds.end(); }
			inline void SetListenerNewThread(SignalType pSignal) { _aRegInvoke.push_back(pSignal); }
			inline void SetListenerReleaseThread(SignalType pSignal) { _aUnregInvoke.push_back(pSignal); }
			inline void UnsetListenerNewThread(SignalType pSignal) { _aRegInvoke.erase(std::find(_aRegInvoke.begin(), _aRegInvoke.end(), pSignal)); }
			inline void UnsetListenerReleaseThread(SignalType pSignal) { _aUnregInvoke.erase(std::find(_aUnregInvoke.begin(), _aUnregInvoke.end(), pSignal)); }
			void CloseWindowExceptForMajor(HWND hCallWnd) const;
		private:
			void AddWndThread(HWND hWnd);
			void RemoveWndThread(HWND hWnd);
			void SendMessageForUITheme(HWND hWnd);
		private:
			std::recursive_mutex _lock;
			Map<HWND, String> _aMajorWnds;
			Array<HWND> _aWnds;
			Map<uint32_t, uint32_t> _aThreadWnds;
			Array<SignalType> _aRegInvoke;
			Array<SignalType> _aUnregInvoke;
		};

		extern RegistratorWindow* GlobalRegistratorWindowPtr;
	}
}