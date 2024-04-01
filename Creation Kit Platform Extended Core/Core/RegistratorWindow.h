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
			void CloseWindowExceptForMajor(HWND hCallWnd) const;
		private:
			std::recursive_mutex _lock;
			Map<HWND, String> _aMajorWnds;
			Array<HWND> _aWnds;
		};

		extern RegistratorWindow* GlobalRegistratorWindowPtr;
	}
}