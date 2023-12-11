#include "TESForm.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			static_assert(sizeof(TESForm::Array) == 0x18);

			std::unordered_set<TESForm*, std::hash<TESForm*>,
				std::equal_to<TESForm*>, voltek::allocator<TESForm*>> AlteredFormListShadow;
			std::unordered_map<uint64_t, TESForm::Array*, std::hash<uint64_t>,
				std::equal_to<uint64_t>, voltek::allocator<std::pair<const uint64_t,
				TESForm::Array*>>> FormReferenceMap;

			bool TESForm::GetActive() const
			{
				return (FormFlags & FormFlags::fsModified) != 0;
			}

			bool TESForm::GetMarkedDelete() const {
				return (FormFlags & FormFlags::fsDeleted) != 0;
			}

			uint32_t TESForm::GetFormID() const
			{
				return FormID;
			}

			BSString TESForm::GetEditorID() const 
			{
				//return thisVirtualCall<char*>(0x1E8, this);
				return "";
			}

			void TESForm::SetNewFormID(uint32_t NewIndex, bool Unk) 
			{
				//thisVirtualCall<char*>(0x310, this, NewIndex, Unk);
			}

			TESForm* TESForm::GetFormByNumericID(uint32_t SearchID)
			{
				//return ((TESForm * (__fastcall*)(uint32_t))OFFSET(0x16B8780, 1530))(SearchID);
				return nullptr;
			}

			void* TESForm::AlteredFormList_Create(TESForm::Array* Array, uint32_t Unknown)
			{
				AlteredFormListShadow.clear();

				//return ((void* (__fastcall*)(TESForm::Array*, uint32_t))OFFSET(0x16C6990, 1530))(Array, Unknown);
				return nullptr;
			}

			void TESForm::AlteredFormList_RemoveAllEntries(TESForm::Array* Array, bool Unknown)
			{
				AlteredFormListShadow.clear();

				//((void(__fastcall*)(TESForm::Array*, bool))OFFSET(0x139B2B0, 1530))(Array, Unknown);
			}

			void TESForm::AlteredFormList_Insert(TESForm::Array* Array, TESForm*& Entry)
			{
				AlteredFormListShadow.insert(Entry);

				//((void(__fastcall*)(TESForm_CK::Array*, TESForm_CK*&))OFFSET(0x146A660, 1530))(Array, Entry);
			}

			void TESForm::AlteredFormList_RemoveEntry(TESForm::Array* Array, uint32_t Index, uint32_t Unknown)
			{
				AlteredFormListShadow.erase(Array->at(Index));

				//((void(__fastcall*)(TESForm_CK::Array*, uint32_t, uint32_t))OFFSET(0x165EA50, 1530))(Array, Index, Unknown);
			}

			bool TESForm::AlteredFormList_ElementExists(TESForm::Array* Array, TESForm*& Entry)
			{
				return AlteredFormListShadow.count(Entry) > 0;
			}

			void FormReferenceMap_RemoveAllEntries()
			{
				/*for (auto [k, v] : FormReferenceMap)
				{
					if (v)
						((void(__fastcall*)(TESForm::Array*, int))OFFSET(0x149F560, 1530))(v, 1);
				}*/

				FormReferenceMap.clear();
			}

			TESForm::Array* FormReferenceMap_FindOrCreate(uint64_t Key, bool Create)
			{
				/*auto itr = FormReferenceMap.find(Key);

				if (itr != FormReferenceMap.end() && itr->second)
					return itr->second;

				if (Create)
				{
					auto* ptr = ((TESForm::Array * (__fastcall*)(size_t))OFFSET(0x1219450, 1530))(24);

					if (ptr)
						ptr = ((TESForm::Array * (__fastcall*)(TESForm::Array*))OFFSET(0x1397CD0, 1530))(ptr);

					FormReferenceMap.insert_or_assign(Key, ptr);
					return ptr;
				}*/

				return nullptr;
			}

			void FormReferenceMap_RemoveEntry(uint64_t Key)
			{
				/*auto itr = FormReferenceMap.find(Key);

				if (itr != FormReferenceMap.end())
				{
					TESForm::Array* ptr = itr->second;
					FormReferenceMap.erase(itr);

					if (ptr)
						((void(__fastcall*)(TESForm::Array*, int))OFFSET(0x149F560, 1530))(ptr, 1);
				}*/
			}

			bool FormReferenceMap_Get(uint64_t Unused, uint64_t Key, TESForm::Array** Value)
			{
				// Function doesn't care if entry is nullptr, only if it exists
				auto itr = FormReferenceMap.find(Key);

				if (itr != FormReferenceMap.end())
				{
					*Value = itr->second;
					return true;
				}

				return false;
			}
		}
	}
}