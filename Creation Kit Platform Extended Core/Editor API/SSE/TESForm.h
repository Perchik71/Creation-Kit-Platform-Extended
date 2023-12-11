#pragma once

#include "../BSTArray.h"
#include "../BSString.h"
#include "../BSStringCache.h"

#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
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
			private:
				char _pad0[0x8];
				uint32_t FormFlags;
				uint32_t FormID;
				char* EditorID;
				BSTrackingData _trackingData;
				char FormType;
				char _pad1[0x1];
			};
		}
	}
}