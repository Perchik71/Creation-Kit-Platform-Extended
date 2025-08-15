// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/Forms/TESObjectREFR.h>
#include <CKPE.Singleton.h>
#include <CKPE.Common.PatchBaseWindow.h>
#include <CKPE.Common.UICheckboxControl.h>
#include <CKPE.ImageList.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			class BNetUploadWindow : public Common::PatchBaseWindow
			{
				Common::UI::CUICheckbox _Platform_PC;
				Common::UI::CUICheckbox _Platform_XBOX;
				Common::UI::CUICheckbox _Platform_PS4;
				Common::UI::CUIBaseControl _ModName_Platform_PC;
				Common::UI::CUIBaseControl _Archives_Platform_PC;
				Common::UI::CUICheckbox _IncludeArchives_Platform_PC;
				Common::UI::CUICheckbox _CreateESL_Platform_PC;
				Common::UI::CUIBaseControl _Browse_Platform_PC;
				Common::UI::CUIBaseControl _Generate_Platform_PC;
				Common::UI::CUIBaseControl _ModName_Platform_XBOX;
				Common::UI::CUIBaseControl _Archives_Platform_XBOX;
				Common::UI::CUICheckbox _IncludeArchives_Platform_XBOX;
				Common::UI::CUICheckbox _CreateESL_Platform_XBOX;
				Common::UI::CUIBaseControl _Browse_Platform_XBOX;
				Common::UI::CUIBaseControl _Generate_Platform_XBOX;
				Common::UI::CUIBaseControl _ModName_Platform_PS4;
				Common::UI::CUICheckbox _ExcludeAssetsFiles;
				Common::UI::CUICheckbox _CreateESL_Platform_PS4;
				Common::UI::CUIBaseControl _Browse_Platform_PS4;

				BNetUploadWindow(const BNetUploadWindow&) = delete;
				BNetUploadWindow& operator=(const BNetUploadWindow&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				constexpr static std::uint16_t UI_PLATFORM_PC_CHECK = 6333;
				constexpr static std::uint16_t UI_PLATFORM_XBOX_CHECK = 6336;
				constexpr static std::uint16_t UI_PLATFORM_PS4_CHECK = 6339;
				constexpr static std::uint16_t UI_PLATFORM_PC_MODNAME = 6334;
				constexpr static std::uint16_t UI_PLATFORM_PC_BROWSE = 3681;
				constexpr static std::uint16_t UI_PLATFORM_PC_ESL_CHECK = 1007;
				constexpr static std::uint16_t UI_PLATFORM_PC_ARCHIVE = 6335;
				constexpr static std::uint16_t UI_INCLUDE_UPLOAD_ARCHIVE_PC_CHECK = 1010;
				constexpr static std::uint16_t UI_PLATFORM_PC_GENERATE = 3682;
				constexpr static std::uint16_t UI_PLATFORM_XBOX_MODNAME = 6337;
				constexpr static std::uint16_t UI_PLATFORM_XBOX_BROWSE = 3683;
				constexpr static std::uint16_t UI_PLATFORM_XBOX_ESL_CHECK = 1008;
				constexpr static std::uint16_t UI_PLATFORM_XBOX_ARCHIVE = 6338;
				constexpr static std::uint16_t UI_INCLUDE_UPLOAD_ARCHIVE_XBOX_CHECK = 1011;
				constexpr static std::uint16_t UI_PLATFORM_XBOX_GENERATE = 3684;
				constexpr static std::uint16_t UI_PLATFORM_PS4_MODNAME = 6340;
				constexpr static std::uint16_t UI_PLATFORM_PS4_BROWSE = 3685;
				constexpr static std::uint16_t UI_PLATFORM_PS4_ESL_CHECK = 1009;
				constexpr static std::uint16_t UI_EXCLUDE_UPLOAD_ASSETS_FILES_CHECK = 1012;

				BNetUploadWindow();

				inline static ISingleton<BNetUploadWindow> Singleton;
				static INT_PTR CALLBACK HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
				static void sub(std::int64_t a1, std::int64_t a2);

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);
			};
		}
	}
}