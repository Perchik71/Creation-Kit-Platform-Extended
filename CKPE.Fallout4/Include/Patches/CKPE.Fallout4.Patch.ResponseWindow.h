// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Singleton.h>
#include <CKPE.Common.PatchBaseWindow.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			class ResponseWindow : public Common::PatchBaseWindow
			{
				Common::UI::CUIBaseControl m_List;

				static void sub(HWND hWndButtonGenerate) noexcept(true);
				static bool sub2(const char* AudioPath, const char* ResponseText, const char* LipPath) noexcept(true);
				static bool ExecuteApplication(const char* CmdLine, bool Hide = true,
					std::uint32_t Timeout = INFINITE) noexcept(true);

				ResponseWindow(const ResponseWindow&) = delete;
				ResponseWindow& operator=(const ResponseWindow&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				ResponseWindow();

				inline static ISingleton<ResponseWindow> Singleton;
				static INT_PTR CALLBACK HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

				static bool GenerationLip(const char* AudioPath, const char* LipPath, 
					const char* ResponseText) noexcept(true);
				static bool GenerationFuz(const char* FuzPath, const char* AudioPath,
					const char* LipPath) noexcept(true);

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);
			};
		}
	}
}