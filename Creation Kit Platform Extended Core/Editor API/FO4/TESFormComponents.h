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

			// 0x8 5 functions RVA 0x38931A0
			class IBGSLocalizedString {
			public:
				virtual ~IBGSLocalizedString(VOID);
				// don't use in release the class IBGSLocalizedString, function call __purecall (abstract)
				virtual LPCSTR Get(VOID) const;
			};

			// 0x10 5 functions RVA 0x38931E0
			class BGSLocalizedString : public IBGSLocalizedString {
			private:
				// members
				/*08*/ BSEntryString* String;
			};

			// 0x18 5 functions RVA 0x3893220
			class BGSLocalizedStringDL : public IBGSLocalizedString {
			private:
				// members
				/*08*/ LPVOID Unk08;
				/*10*/ BSEntryString* String;
			public:
				VOID Set(LPCSTR str);
			};

			// 0x10 5 functions RVA 0x3893260
			class BGSLocalizedStringIL : public BGSLocalizedString {
			private:
			};

			// 0x18
			class TESFullName : public TESPersistent, public BGSLocalizedString {
			public:
				virtual ~TESFullName(VOID);														// 000
			public:
				virtual VOID VT_Func07(VOID);													// 038
				virtual VOID VT_Func08(VOID);													// 040
				virtual VOID VT_Func09(VOID);													// 048
				virtual VOID VT_Func10(VOID);													// 050
				virtual VOID VT_Func11(VOID);													// 058	not implemented
				virtual VOID VT_Func12(VOID);													// 060	not implemented
				virtual VOID VT_Func13(VOID);													// 068	not implemented
				virtual VOID VT_Func14(VOID);													// 070
				// Checking for the existence of a control
				// control UID 1060(0x424)
				virtual VOID Debug_CheckExistsControl_DisplayName_N1060(HWND hDialog) const;	// 078
				virtual VOID VT_Func16(VOID);													// 080
				// The function is passed a window descriptor where there is an edit IDC_FULL_NAME window. 
				// She takes the text from there and saves it.
				// control UID 1060(0x424)
				virtual VOID UpdateStringFromControl_DisplayName_N1060(HWND hDialog) const;		// 088
				virtual VOID VT_Func18(VOID);													// 090	not implemented
				virtual VOID VT_Func19(VOID);													// 098	not implemented
				virtual VOID VT_Func20(VOID);													// 0A0	not implemented
				virtual VOID VT_Func21(VOID);													// 0A8	
				// return full name
				virtual LPCSTR c_str(VOID) const;												// 0B0
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

			// 0x18 -> RVA vtable 0x3908058
			class TESModel : public BSFixedString {
			private:
				// members
				/*10*/ LPVOID textures_hash;
				/*18*/ LPVOID materials_hash;
				/*20*/ LPVOID unk20;
				/*28*/ DWORD dwUnk28;
				/*2C*/ DWORD dwUnk2C;
				/*30*/ DWORD dwUnk30;
				/*34*/ DWORD dwUnk34;
				/*38*/ DWORD dwUnk38;
				/*3C*/ DWORD dwUnk3C;
				/*40*/ TESForm* MaterialSwapForm;
				/*48*/ DWORD dwUnk48;
				/*4C*/ DWORD dwUnk4C;
			};
			static_assert(sizeof(TESModel) == 0x50);

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
				/*10*/ BGSLocalizedStringDL LocalizedString;
			public:
				inline LPCSTR Get() const { return LocalizedString.Get(); }
				inline void Set(LPCSTR str) { LocalizedString.Set(str); }
				inline void Set(const BSString& str) { LocalizedString.Set(*str); }
			public:
				inline TESDescription& operator=(LPCSTR str) { Set(str); return *this; }
				inline TESDescription& operator=(const BSString& str) { Set(*str); return *this; }
				inline LPCSTR operator*() const { return Get(); }
			};
			static_assert(sizeof(TESDescription) == 0x28);
		}
	}
}

#pragma pack(pop)