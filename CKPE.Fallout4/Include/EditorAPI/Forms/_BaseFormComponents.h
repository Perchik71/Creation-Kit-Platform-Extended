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

#pragma once
#pragma pack(push, 1)

#include "..\BSString.h"
#include "BSEntryString.h"
#include "BSFixedString.h"
#include "..\BGSLocalizedString.h"
#include "..\BSTList.h"
#include "..\BSTArray.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			class TESForm;
			class TESObjectCELL;

			struct BSModInfo {
				LPVOID entries;
				UINT64 size;
			};

			class TESCondition
			{
			public:
				virtual ~TESCondition(VOID) = default;
			private:
				void* _Conditions;
			};

			class TESPersistent {
			public:
				TESPersistent(VOID) = default;
			public:
				virtual ~TESPersistent(VOID);	// 000
				virtual VOID VT_Func01(VOID);	// 008
				virtual VOID VT_Func02(VOID);	// 010
				virtual VOID VT_Func03(VOID);	// 018
				virtual VOID VT_Func04(VOID);	// 020 not implemented
				virtual VOID VT_Func05(VOID);	// 028 not implemented
				virtual VOID VT_Func06(VOID);	// 030 ---
			};

			// 0x18
			class TESFullName
			{
			public:
				virtual ~TESFullName() = default;

				inline const char* GetName() const
				{
					return thisVirtualCall<const char*>(0xB0, this);
				}

				READ_PROPERTY(GetName) const char* Name;
			private:
				BGSLocalizedString<BSEntryString> _Str;
			};

			// 0x10
			class TESCellUseList
			{
			public:
				__declspec(align(8)) struct CellUseInfo {
					TESObjectCELL* cell;
					CHAR		   pad0[8];
					DWORD          count;		// or DWORD64 ???
				};
				typedef BSSimpleList<CellUseInfo> CellUseInfoListT;

				// members
				/*00*/ CellUseInfoListT	cellUses;
			};
			static_assert(sizeof(TESCellUseList) == 0x10);
			static_assert(sizeof(TESCellUseList::CellUseInfo) == 0x18);

			class BGSMod {
			public:
				class Template {
				public:
					// 0x20 -> RVA vtable 0x38181A8
					class Items {
					private:
						// members
						CHAR pad0[0x18];
					public:
						virtual ~Items(VOID);
					};
				};
			};
			static_assert(sizeof(BGSMod::Template::Items) == 0x20);

			// 10
			class TESForcedLocRef : public TESPersistent {
			public:
				// members
				/*08*/ TESForm* LocRefForm;
			};
			static_assert(sizeof(TESForcedLocRef) == 0x10);

			// 28
			// The class does not have a direct receipt of the text, only in the IDC_DESCRIPTION_NAME window, 
			// so let's figure out our own
			class TESDescription : public TESPersistent {
			private:
				// members
				/*08*/ DWORD Unk08;
				/*0C*/ DWORD Unk0C;
				/*10*/ BGSLocalizedStringDL<BSEntryString> LocalizedString;
			public:
				inline const char* Get() const { return LocalizedString.c_str(); }
			public:
				inline LPCSTR operator*() const { return Get(); }
			};
			static_assert(sizeof(TESDescription) == 0x28);
		}
	}
}

#pragma pack(pop)