// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Utils.h>
#include <EditorAPI/TESTexture.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			void TESTexture::SetTextureFromDialogBox(HWND hDialogBox, std::uint32_t CtrlID) noexcept(true)
			{
				this_call<void>(0xA8, this, hDialogBox, CtrlID);
			}

			std::uint32_t TESTexture::GetSlotIndex() const noexcept(true)
			{
				return 0; /* always */ 
			}

			const char* TESTexture::GetAsNormalFile(BSString& sOut) const noexcept(true)
			{
				return this_call<const char*>(0xB8, this, &sOut);
			}

			const char* TESTexture::GetDefaultPath() const noexcept(true) 
			{
				return "Data\\Textures\\"; 
			}

			const char* TESTexture::GetActionName() const noexcept(true) 
			{ 
				return "Add Image File"; 
			}

			void TESTexture::SetTextureFromFileName(const char* lpstrFileName) noexcept(true)
			{
				this_call<void>(0xD0, this, lpstrFileName);
			}
		}
	}
}