// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <algorithm>
#include <windows.h>
#include <CKPE.Asserts.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.UIHotkeys.h>
#include <Patches/CKPE.SkyrimSE.Patch.RenderWindow.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			decltype(&UIHotkeys::CKPEHotkeys) UIHotkeysGeneral;
			decltype(&UIHotkeys::sub) UIHotkeysSub;
		
			using TUIHotkeysSub2 = std::uintptr_t(void*, const char*);
			using TUIHotkeysSub3 = std::uintptr_t(void*);
			static std::function<TUIHotkeysSub2> UIHotkeysSub2;
			static std::function<TUIHotkeysSub3> UIHotkeysSub3;
		
			byte* ToggleDebugShaderSetting = 0;
			uint32_t* DebugShaderTypeSetting = 0;
			// 1 = Number of Lights
			// 4 = Number of Passes
			// 6 = Texture Usage

			static bool ReplaceHotkeys(const char* HotkeyFunction, [[maybe_unused]] const char* DisplayText, char* VirtualKey,
				bool* Alt, bool* Ctrl, bool* Shift) noexcept(true)
			{
				// Read the setting, strip spaces/quotes, then split by each '+' modifier
				auto newKeybind = _READ_OPTION_STR("Hotkeys", HotkeyFunction, "");

				for (std::size_t i; (i = newKeybind.find("\"")) != std::string::npos;)
					newKeybind.replace(i, 1, "");

				for (std::size_t i; (i = newKeybind.find(" ")) != std::string::npos;)
					newKeybind.replace(i, 1, "");

				if (newKeybind.empty())
					return false;

				std::transform(newKeybind.begin(), newKeybind.end(), newKeybind.begin(), toupper);

				*VirtualKey = 0;
				*Alt = false;
				*Ctrl = false;
				*Shift = false;

				char* context = nullptr;
				const char* t = strtok_s(newKeybind.data(), "+", &context);

				do
				{
					if (!strcmp(t, "CTRL"))
						*Ctrl = true;
					else if (!strcmp(t, "SHIFT"))
						*Shift = true;
					else if (!strcmp(t, "ALT"))
						*Alt = true;
					else if (!strcmp(t, "ESC"))
						*VirtualKey = VK_ESCAPE;
					else if (!strcmp(t, "SPACE"))
						*VirtualKey = VK_SPACE;
					else if (!strcmp(t, "ENTER"))
						*VirtualKey = VK_RETURN;
					else if (!strcmp(t, "DEL"))
						*VirtualKey = VK_DELETE;
					else if (!strcmp(t, "TAB"))
						*VirtualKey = VK_TAB;
					else if (strlen(t) > 1 && t[0] == 'F')
					{
						// Parse function keys F1 to F12
						INT index = atoi(&t[1]);

						CKPE_ASSERT_MSG_FMT(index >= 1 && index <= 12, "Invalid function key index '%s' for hotkey function '%s'", t,
							HotkeyFunction);

						*VirtualKey = VK_F1 + index - 1;
					}
					else
					{
						// Parse a regular character
						CKPE_ASSERT_MSG_FMT(strlen(t) == 1, "Invalid or unknown key binding '%s' for hotkey function '%s'", t,
							HotkeyFunction);

						// This should be translated with VkKeyScan but virtual keys make things difficult...
						*VirtualKey = t[0];
					}
				} while (t = strtok_s(nullptr, "+", &context));

				return true;
			}

			UIHotkeys::UIHotkeys() : Common::Patch()
			{
				SetName("UI Hotkeys");
			}

			bool UIHotkeys::HasOption() const noexcept(true)
			{
				return true;
			}

			const char* UIHotkeys::GetOptionName() const noexcept(true)
			{
				return "CreationKit:bUIHotkeys";
			}

			bool UIHotkeys::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> UIHotkeys::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool UIHotkeys::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool UIHotkeys::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool UIHotkeys::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;
				
				UIHotkeysGeneral = (decltype(&CKPEHotkeys))Relocation(ID(191407)).WriteJump(&CKPEHotkeys);
				UIHotkeysSub = (decltype(&sub))Relocation(ID(618847)).WriteJump(&sub);
				UIHotkeysSub2 = Relocation<TUIHotkeysSub2>(ID(604959)).Get();
				UIHotkeysSub3 = Relocation<TUIHotkeysSub3>(ID{ 619686, 974159 }).Get();
				
				ToggleDebugShaderSetting = (byte*)ID(486220).Address();
				DebugShaderTypeSetting = (uint32_t*)ID(340720).Address();

				return true;
			}

			void UIHotkeys::sub(void* Thisptr, void(*Callback)(), const char** HotkeyFunction,
				const char** DisplayText, char VirtualKey, bool Alt, bool Ctrl, bool Shift) noexcept(true)
			{
				if (!ReplaceHotkeys(*HotkeyFunction, *DisplayText, &VirtualKey, &Alt, &Ctrl, &Shift))
					_MESSAGE("Can't find this hotkeys ""%s"" in the mod settings. (%X), %d, %d, %d, %s)"sv,
						*HotkeyFunction, VirtualKey, (int)Alt, (int)Ctrl, (int)Shift, *DisplayText);

				//_CONSOLE("%s %p", *HotkeyFunction, Callback);

				UIHotkeysSub(Thisptr, Callback, HotkeyFunction, DisplayText, VirtualKey, Alt, Ctrl, Shift);
			}
			
			void UIHotkeys::CKPEHotkeys(void* Thisptr)
			{
				const char** ToggleDebugShader_text;
				const char** ToggleDebugShader_func;
				void* unk_var1[8];
				void* unk_var2[8];
				const char** ToggleOverlay_text;
				const char** ToggleOverlay_func;
				void* unk_var3[8];
				void* unk_var4[8];
				
				ToggleDebugShader_text = (const char**)UIHotkeysSub2(unk_var1,"Toggle debug shader");
				ToggleDebugShader_func = (const char**)UIHotkeysSub2(unk_var2,"HKFunc_ToggleDebugShader");
				sub(Thisptr,ToggleDebugShader,ToggleDebugShader_func,ToggleDebugShader_text,VK_F3,false,false,false);
				UIHotkeysSub3(unk_var1);
				UIHotkeysSub3(unk_var2);
				
				ToggleOverlay_text = (const char**)UIHotkeysSub2(unk_var3,"Toggle overlay");
				ToggleOverlay_func = (const char**)UIHotkeysSub2(unk_var4,"HKFunc_ToggleOverlay");
				sub(Thisptr,ToggleOverlay,ToggleOverlay_func,ToggleOverlay_text,VK_F1,false,false,false);
				UIHotkeysSub3(unk_var3);
				UIHotkeysSub3(unk_var4);
				
				UIHotkeysGeneral(Thisptr);
				return;
			}
			
			void UIHotkeys::ToggleDebugShader()
			{
				if (*ToggleDebugShaderSetting == 0)
				{
					*ToggleDebugShaderSetting = 1;
					*DebugShaderTypeSetting = 1;
				}
				else
				{
					*ToggleDebugShaderSetting = 0;
					*DebugShaderTypeSetting = 0;
				}
			}
			
			void UIHotkeys::ToggleOverlay()
			{
				RenderWindow::Singleton->HideMainImguiWnd = !RenderWindow::Singleton->HideMainImguiWnd;
			}
		}
	}
}