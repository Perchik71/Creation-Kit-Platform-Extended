// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "UIHotkeys.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		uintptr_t pointer_UIHotkeys_sub = 0;

		UIHotkeysPatch::UIHotkeysPatch() : Module(GlobalEnginePtr)
		{}

		bool UIHotkeysPatch::HasOption() const
		{
			return true;
		}

		bool UIHotkeysPatch::HasCanRuntimeDisabled() const
		{
			return false;
		}

		const char* UIHotkeysPatch::GetOptionName() const
		{
			return "CreationKit:bUIHotkeys";
		}

		const char* UIHotkeysPatch::GetName() const
		{
			return "UI Hotkeys";
		}

		bool UIHotkeysPatch::HasDependencies() const
		{
			return false;
		}

		Array<String> UIHotkeysPatch::GetDependencies() const
		{
			return {};
		}

		bool UIHotkeysPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			return true;
		}

		bool UIHotkeysPatch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			auto verPatch = lpRelocationDatabaseItem->Version();
			if (verPatch == 1)
			{
				Detours::X64::DetourFunctionClass(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0)), &sub);
				pointer_UIHotkeys_sub = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));

				return true;
			}
			else if (verPatch == 2)
			{
				lpRelocator->PatchNop(lpRelocationDatabaseItem->At(0), 0x19);
				Detours::X64::DetourFunctionClass(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1)), &sub);
				pointer_UIHotkeys_sub = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(2));

				return true;
			}

			return false;
		}

		bool UIHotkeysPatch::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			return false;
		}

		void UIHotkeysPatch::sub(void* Thisptr, void(*Callback)(), const char** HotkeyFunction,
			const char** DisplayText, char VirtualKey, bool Alt, bool Ctrl, bool Shift)
		{
			// Read the setting, strip spaces/quotes, then split by each '+' modifier
			String newKeybind = _READ_OPTION_STR("Hotkeys", *HotkeyFunction, "");

			for (size_t i; (i = newKeybind.find("\"")) != String::npos;)
				newKeybind.replace(i, 1, "");

			for (size_t i; (i = newKeybind.find(" ")) != String::npos;)
				newKeybind.replace(i, 1, "");

			if (!newKeybind.empty())
			{
				std::transform(newKeybind.begin(), newKeybind.end(), newKeybind.begin(), toupper);

				VirtualKey = 0;
				Alt = false;
				Ctrl = false;
				Shift = false;

				char* context = nullptr;
				const char* t = strtok_s(newKeybind.data(), "+", &context);

				do
				{
					if (!strcmp(t, "CTRL"))
						Ctrl = true;
					else if (!strcmp(t, "SHIFT"))
						Shift = true;
					else if (!strcmp(t, "ALT"))
						Alt = true;
					else if (!strcmp(t, "ESC"))
						VirtualKey = VK_ESCAPE;
					else if (!strcmp(t, "DEL"))
						VirtualKey = VK_DELETE;
					else if (!strcmp(t, "TAB"))
						VirtualKey = VK_TAB;
					else if (strlen(t) > 1 && t[0] == 'F')
					{
						// Parse function keys F1 to F12
						int index = atoi(&t[1]);

						AssertMsgVa(index >= 1 && index <= 12, "Invalid function key index '%s' for hotkey function '%s'", t, 
							*HotkeyFunction);

						VirtualKey = VK_F1 + index - 1;
					}
					else
					{
						// Parse a regular character
						AssertMsgVa(strlen(t) == 1, "Invalid or unknown key binding '%s' for hotkey function '%s'", t, 
							*HotkeyFunction);

						// This should be translated with VkKeyScan but virtual keys make things difficult...
						VirtualKey = t[0];
					}
				} while (t = strtok_s(nullptr, "+", &context));
			}

			((decltype(&sub))pointer_UIHotkeys_sub)(Thisptr, Callback, HotkeyFunction, DisplayText,
				VirtualKey, Alt, Ctrl, Shift);
		}

		void UIHotkeysPatch::sub_ver2(LPVOID Thisptr, VOID(*Callback)(VOID), EditorAPI::Fallout4::BSEntryString** HotkeyFunction,
			EditorAPI::Fallout4::BSEntryString** DisplayText, CHAR VirtualKey, bool Alt, bool Ctrl, bool Shift)
		{
			// Read the setting, strip spaces/quotes, then split by each '+' modifier
			String newKeybind = _READ_OPTION_STR("Hotkeys", (*HotkeyFunction)->Get<CHAR>(TRUE), "");
			if (!newKeybind.length()) {
				_MESSAGE("Can't find this hotkeys ""%s"" in the mod settings. (%X), %d, %d, %d, %s)",
					(*HotkeyFunction)->Get<CHAR>(TRUE), VirtualKey, (int)Alt, (int)Ctrl, (int)Shift,
					(*DisplayText)->Get<CHAR>(TRUE));
			}

			for (size_t i; (i = newKeybind.find("\"")) != std::string::npos;)
				newKeybind.replace(i, 1, "");

			for (size_t i; (i = newKeybind.find(" ")) != std::string::npos;)
				newKeybind.replace(i, 1, "");

			if (!newKeybind.empty()) {
				std::transform(newKeybind.begin(), newKeybind.end(), newKeybind.begin(), toupper);

				VirtualKey = 0;
				Alt = false;
				Ctrl = false;
				Shift = false;

				LPSTR context = NULL;
				LPCSTR t = strtok_s(newKeybind.data(), "+", &context);

				do {
					if (!strcmp(t, "CTRL"))
						Ctrl = true;
					else if (!strcmp(t, "SHIFT"))
						Shift = true;
					else if (!strcmp(t, "ALT"))
						Alt = true;
					else if (!strcmp(t, "ESC"))
						VirtualKey = VK_ESCAPE;
					else if (!strcmp(t, "SPACE"))
						VirtualKey = VK_SPACE;
					else if (!strcmp(t, "ENTER"))
						VirtualKey = VK_RETURN;
					else if (!strcmp(t, "DEL"))
						VirtualKey = VK_DELETE;
					else if (!strcmp(t, "TAB"))
						VirtualKey = VK_TAB;
					else if (strlen(t) > 1 && t[0] == 'F') {
						// Parse function keys F1 to F12
						INT index = atoi(&t[1]);

						AssertMsgVa(index >= 1 && index <= 12, "Invalid function key index '%s' for hotkey function '%s'", t, 
							*HotkeyFunction);

						VirtualKey = VK_F1 + index - 1;
					}
					else {
						// Parse a regular character
						AssertMsgVa(strlen(t) == 1, "Invalid or unknown key binding '%s' for hotkey function '%s'", t,
							*HotkeyFunction);

						// This should be translated with VkKeyScan but virtual keys make things difficult...
						VirtualKey = t[0];
					}
				} while (t = strtok_s(NULL, "+", &context));
			}

			((decltype(&sub_ver2))pointer_UIHotkeys_sub)(Thisptr, Callback, HotkeyFunction, DisplayText, 
				VirtualKey, Alt, Ctrl, Shift);
		}
	}
}