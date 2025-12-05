// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <algorithm>
#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.Asserts.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.UIHotkeys.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			decltype(&UIHotkeys::sub) UIHotkeysSub;

			static bool ReplaceHotkeys(const char* HotkeyFunction, const char* DisplayText, char* VirtualKey,
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

				char* context = NULL;
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
				} while (t = strtok_s(NULL, "+", &context));

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

			bool UIHotkeys::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool UIHotkeys::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				Detours::DetourClassJump(__CKPE_OFFSET(0), &sub);
				UIHotkeysSub = (decltype(&sub))__CKPE_OFFSET(1);

				return true;
			}

			void UIHotkeys::sub(void* Thisptr, void(*Callback)(), const char** HotkeyFunction,
				const char** DisplayText, char VirtualKey, bool Alt, bool Ctrl, bool Shift) noexcept(true)
			{
				if (!ReplaceHotkeys(*HotkeyFunction, *DisplayText, &VirtualKey, &Alt, &Ctrl, &Shift))
					_MESSAGE("Can't find this hotkeys ""%s"" in the mod settings. (%X), %d, %d, %d, %s)",
						*HotkeyFunction, VirtualKey, (int)Alt, (int)Ctrl, (int)Shift, *DisplayText);

				//_CONSOLE("%s %p", *HotkeyFunction, Callback);

				UIHotkeysSub(Thisptr, Callback, HotkeyFunction, DisplayText, VirtualKey, Alt, Ctrl, Shift);
			}
		}
	}
}