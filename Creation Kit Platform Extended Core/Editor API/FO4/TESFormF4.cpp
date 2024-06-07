//////////////////////////////////////////
/*
* Copyright (c) 2020-2022 Perchik71 <email:perchik71@outlook.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this
* software and associated documentation files (the "Software"), to deal in the Software
* without restriction, including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////

#include "TESFormF4.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			uintptr_t pointer_TESForm_sub = 0;
			uintptr_t pointer_TESForm_data = 0;
			uintptr_t pointer_TESForm_sub1 = 0;

			using namespace CreationKitPlatformExtended::Core;

			void TESForm::LoadForm(TESFile* file)
			{
				thisVirtualCall<void>(0xD0, this, file);
			}

			void TESForm::SaveForm(TESFile* file) 
			{
				thisVirtualCall<void>(0xD8, this, file);
			}

			TESForm::FormType TESForm::GetFormType() const
			{
				return thisVirtualCall<TESForm::FormType>(0x120, this);
			}

			bool TESForm::CanPreview() const
			{
				return thisVirtualCall<bool>(0x1D0, this);
			}

			void TESForm::DebugInfo(char* buffer, uint32_t dwSize) const 
			{
				thisVirtualCall<void>(0x128, this, buffer, dwSize);
			}

			void TESForm::MarkAsDeleted(bool state) 
			{
				thisVirtualCall<void>(0x198, this, state);
			}

			void TESForm::MarkAsChanged(bool state) 
			{
				thisVirtualCall<void>(0x1A0, this, state);
			}

			uint32_t TESForm::GetEditIDLength() const 
			{
				return thisVirtualCall<uint32_t>(0x230, this);
			}

			const char* TESForm::GetEditorID() const 
			{
				return thisVirtualCall<const char*>(0x238, this);
			}

			TESFile* TESForm::GetBelongsToPlugin() const
			{
				return thisCall<TESFile*>(pointer_TESForm_sub1, this);
			}

			const char* TESForm::GetFullName() const
			{
				TESFullName* fullname = (TESFullName*)_DYNAMIC_CAST(this, 0, "class TESForm", "class TESFullName");
				return fullname ? fullname->Name : "";
			}

			const char* TESForm::GetFormTypeShortStr() const
			{
				struct BGSTypeConvertShortItem 
				{
					DWORD64 type_id;
					LPCSTR Name;
					CHAR ShortName[4];
					CHAR pad[4];
				};
				static_assert(sizeof(BGSTypeConvertShortItem) == 0x18);
				BGSTypeConvertShortItem* arr = (BGSTypeConvertShortItem*)pointer_TESForm_data;
				return arr[(DWORD)GetFormType()].Name;
			}

			BSString TESForm::GetID() const 
			{
				if (!GetEditIDLength())
					return "";
				else
					return GetEditorID();
			}

			TESForm* TESForm::GetFormByNumericID(const uint32_t SearchID)
			{
				return ((TESForm*(__fastcall*)(uint32_t))pointer_TESForm_sub)(SearchID);
			}
		}
	}
}