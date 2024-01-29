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
				BSString GetEditorID() const;

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
			private:
				char _pad0[0x8];
				uint32_t FormFlags;
				uint32_t FormID;
				char* EditorID;
				BSTrackingData _trackingData;
				char FormType;
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