// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/CKSSE/TESForm_CK.h

#pragma once

#include "../BSTArray.h"
#include "../BSString.h"
#include "BSStringCache.h"
#include "../BSHandleRefObject.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			class TESFullName
			{
			private:
				virtual void FuncUnk00();
				virtual void FuncUnk08();
				virtual void FuncUnk10();
				virtual void FuncUnk18();
				virtual void FuncUnk20();
				virtual void FuncUnk28();
				virtual void FuncUnk30();
				virtual void FuncUnk38();
				virtual void FuncUnk40();
				virtual void FuncUnk48();
				virtual void FuncUnk50();
				virtual void FuncUnk58();
				virtual void FuncUnk60();
				virtual void FuncUnk68();
				virtual void FuncUnk70();
			public:
				virtual const char* GetName() const;

				READ_PROPERTY(GetName) const char* Name;
			};

#pragma pack(push, 1)
			class TESForm
			{
			public:
				// Form State
				enum FormFlags : DWORD {
					fsMaster	= /*00*/ 0x1,		// form is from an esm file
					fsModified	= /*01*/ 0x2,		// form is overriden by active mod or save file
					fsLinked	= /*03*/ 0x8,		// set after formids have been resolved into TESForm*
					fsDeleted	= /*05*/ 0x20,		// set on deletion, not saved in CK
					fsTemporary = /*0E*/ 0x4000,	// not saved in CK
				};
			protected:
				// 06
				struct BSTrackingData {
					WORD date;         // 00 low byte is day of month, high byte is number of months with 1 = Jan. 
									   // 2003 (Decembers are a little weird)
					WORD pad02;
					BYTE lastUser;     // 04 userID that last had this form checked out
					BYTE currentUser;  // 05 userID that has this form checked out
				};
			public:
				virtual ~TESForm();

				using Array = BSTArray<TESForm*>;

				bool GetActive() const;
				bool GetMarkedDelete() const;
				uint32_t GetFormID() const;
				char GetFormType() const;
				inline const char* GetEditorID_NoVTable() const { return _EditorID; }

				void SetNewFormID(uint32_t NewIndex, bool Unk = true);

				static TESForm* GetFormByNumericID(uint32_t SearchID);
				static void* AlteredFormList_Create(Array* Array, uint32_t Unknown);
				static void AlteredFormList_RemoveAllEntries(Array* Array, bool Unknown);
				static void AlteredFormList_Insert(Array* Array, TESForm*& Entry);
				static void AlteredFormList_RemoveEntry(Array* Array, uint32_t Index, uint32_t Unknown);
				static bool AlteredFormList_ElementExists(Array* Array, TESForm*& Entry);

				inline static TESForm* (*OldGetFormByNumericID)(uint32_t);
				inline static void* (*OldAlteredFormList_Create)(Array*, uint32_t);
				inline static void (*OldAlteredFormList_RemoveAllEntries)(Array*, bool);
				inline static void (*OldAlteredFormList_Insert)(Array*, TESForm*&);
				inline static void (*OldAlteredFormList_RemoveEntry)(Array*, uint32_t, uint32_t);

				static void FormReferenceMap_RemoveAllEntries();
				static Array* FormReferenceMap_FindOrCreate(uint64_t Key, bool Create);
				static void FormReferenceMap_RemoveEntry(uint64_t Key);
				static bool FormReferenceMap_Get(uint64_t Unused, uint64_t Key, Array** Value);

				inline static Array* (*OldFormReferenceMap_Find)(uint64_t Key);
				inline static Array* (*OldFormReferenceMap_Create)(Array*);
				inline static void (*OldFormReferenceMap_RemoveEntry)(Array*, int);
			public:
				READ_PROPERTY(GetActive) bool Active;
				READ_PROPERTY(GetFormID) uint32_t FormID;
				READ_PROPERTY(GetEditorID) const char* EditorID;
				READ_PROPERTY(GetFormType) char FormType;
			private:
				virtual void FuncUnk008();
				virtual void FuncUnk010();
				virtual void FuncUnk018();
				virtual void FuncUnk020();
				virtual void FuncUnk028();
				virtual void FuncUnk030();
				virtual void FuncUnk038();
				virtual void FuncUnk040();
				virtual void FuncUnk048();
				virtual void FuncUnk050();
				virtual void FuncUnk058();
				virtual void FuncUnk060();
				virtual void FuncUnk068();
				virtual void FuncUnk070();
				virtual void FuncUnk078();
				virtual void FuncUnk080();
				virtual void FuncUnk088();
				virtual void FuncUnk090();
				virtual void FuncUnk098();
				virtual void FuncUnk0A0();
				virtual void FuncUnk0A8();
				virtual void FuncUnk0B0();
				virtual void FuncUnk0B8();
				virtual void FuncUnk0C0();
				virtual void FuncUnk0C8();
				virtual void FuncUnk0D0();
				virtual void FuncUnk0D8();
				virtual void FuncUnk0E0();
				virtual void FuncUnk0E8();
				virtual void FuncUnk0F0();
				virtual void FuncUnk0F8();
				virtual void FuncUnk100();
				virtual void FuncUnk108();
				virtual void FuncUnk110();
				virtual void FuncUnk118();
				virtual void FuncUnk120();
				virtual void FuncUnk128();
				virtual void FuncUnk130();
				virtual void FuncUnk138();
				virtual void FuncUnk140();
				virtual void FuncUnk148();
				virtual void FuncUnk150();
				virtual void FuncUnk158();
				virtual void FuncUnk160();
				virtual void FuncUnk168();
				virtual void FuncUnk170();
				virtual void FuncUnk178();
				virtual void FuncUnk180();
				virtual void FuncUnk188();
				virtual void FuncUnk190();
				virtual void FuncUnk198();
				virtual void FuncUnk1A0();
				virtual void FuncUnk1A8();
				virtual void FuncUnk1B0();
				virtual void FuncUnk1B8();
				virtual void FuncUnk1C0();
				virtual void FuncUnk1C8();
				virtual void FuncUnk1D0();
				virtual void FuncUnk1D8();
				virtual void FuncUnk1E0();
			public:
				virtual const char* GetEditorID() const;
			private:
				virtual void FuncUnk1F0();
				virtual void FuncUnk1F8();
				virtual void FuncUnk200();
				virtual void FuncUnk208();
				virtual void FuncUnk210();
				virtual void FuncUnk218();
				virtual void FuncUnk220();
				virtual void FuncUnk228();
				virtual void FuncUnk230();
				virtual void FuncUnk238();
				virtual void FuncUnk240();
				virtual void FuncUnk248();
				virtual void FuncUnk250();
				virtual void FuncUnk258();
				virtual void FuncUnk260();
				virtual void FuncUnk268();
				virtual void FuncUnk270();
				virtual void FuncUnk278();
				virtual void FuncUnk280();
				virtual void FuncUnk288();
				virtual void FuncUnk290();
				virtual void FuncUnk298();
				virtual void FuncUnk2A0();
				virtual void FuncUnk2A8();
				virtual void FuncUnk2B0();
				virtual void FuncUnk2B8();
				virtual void FuncUnk2C0();
				virtual void FuncUnk2C8();
			public:
				virtual void EditFormToWindow(HWND, __int64, __int64);
			private:
				virtual void FuncUnk2D8();
				virtual void FuncUnk2E0();
				virtual void FuncUnk2E8();
				virtual void FuncUnk2F0();
				virtual void FuncUnk2F8();
				virtual void FuncUnk300();
				virtual void FuncUnk308();
				virtual void FuncUnk310();
				virtual void FuncUnk318();
				virtual void FuncUnk320();
			private:
				char _pad0[0x8];
				uint32_t _FormFlags;
				uint32_t _FormID;
				char* _EditorID;
				BSTrackingData _trackingData;
				char _FormType;
				char _pad1[0x1];
			};
			static_assert(sizeof(TESForm) == 0x28);
#pragma pack(pop)

			class TESChildCell
			{
			public:
				virtual ~TESChildCell();
				virtual void* GetSaveParentCell();
			};
			static_assert(sizeof(TESChildCell) == 0x8);

			class TESObjectREFR_Original : public TESForm, public TESChildCell, public BSHandleRefObject_Original
			{
			public:
				virtual ~TESObjectREFR_Original();
				virtual void OtherTestFunction2();
				char _pad0[0x5C];
			};
			static_assert(sizeof(TESObjectREFR_Original) == 0xA0);

			class TESObjectREFR_Extremly : public TESForm, public TESChildCell, public BSHandleRefObject_Extremly
			{
			public:
				virtual ~TESObjectREFR_Extremly();
				virtual void OtherTestFunction2();
				char _pad0[0x5C];
			};
			static_assert(sizeof(TESObjectREFR_Extremly) == 0xA0);

			typedef BSTArray<TESForm*>	TESFormArray;
			typedef BSTArray<uint32_t>	TESFormIDArray;
		}
	}
}