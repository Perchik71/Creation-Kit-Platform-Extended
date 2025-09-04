// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Singleton.h>
#include <CKPE.Common.PatchBaseWindow.h>
#include <CKPE.Common.UIBaseWindow.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
			class MainWindow : public Common::PatchBaseWindow
			{
				MainWindow(const MainWindow&) = delete;
				MainWindow& operator=(const MainWindow&) = delete;

				static bool CALLBACK HKInitialize(void* MainWindow, const char* ExeFileName, 
					std::uint32_t Unk01) noexcept(true);
				static void CALLBACK HKInitializeActions(void* MainWindow) noexcept(true);
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				constexpr static auto UI_EXTMENU_ID = 51001;
				constexpr static auto UI_EXTMENU_SHOWLOG = 51002;
				constexpr static auto UI_EXTMENU_CLEARLOG = 51003;
				constexpr static auto UI_EXTMENU_AUTOSCROLL = 51004;
				constexpr static auto UI_EXTMENU_DUMPRTTI = 51005;
				constexpr static auto UI_EXTMENU_LOADEDESPINFO = 51006;
				constexpr static auto UI_EXTMENU_HARDCODEDFORMS = 51007;
				constexpr static auto UI_EXTMENU_SDM = 51008;

				constexpr static auto UI_EXTMENU_IMPORT = 51009;
				constexpr static auto UI_EXTMENU_IMPORT_DIALOGUE = 40468;
				constexpr static auto UI_EXTMENU_IMPORT_QUESTSTAGES = 40469;
				constexpr static auto UI_EXTMENU_IMPORT_QUESTOBJECTIVES = 40772;
				constexpr static auto UI_EXTMENU_IMPORT_NAMES = 40473;
				constexpr static auto UI_EXTMENU_IMPORT_TOPICS = 40738;
				constexpr static auto UI_EXTMENU_IMPORT_SCRIPTSMSGBOX = 40475;
				constexpr static auto UI_EXTMENU_IMPORT_GAMESETTINGS = 40492;
				constexpr static auto UI_EXTMENU_IMPORT_DESCRIPTIONS = 40493;
				constexpr static auto UI_EXTMENU_IMPORT_FACTIONRANKNAMES = 40538;
				constexpr static auto UI_EXTMENU_IMPORT_AMMO = 40762;
				constexpr static auto UI_EXTMENU_IMPORT_BODYPARTDATA = 40781;

				constexpr static auto UI_EXTMENU_FORMINFOOUTPUT = 51010;
				constexpr static auto UI_EXTMENU_TRACER = 51011;
				constexpr static auto UI_EXTMENU_TRACER_CLEAR = 51012;
				constexpr static auto UI_EXTMENU_TRACER_DUMP = 51013;
				constexpr static auto UI_EXTMENU_TRACER_RECORD = 51014;
				constexpr static auto UI_EXTMENU_TOGGLE_ANTIALIASING = 51015;

				MainWindow();

				inline static ISingleton<MainWindow> Singleton;

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				static void CALLBACK ShowForm(std::uint32_t FormID) noexcept(true);
				static HWND CALLBACK GetWindowHandle();
			};
		}
	}
}