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

			const char* TESFullName::GetName() const
			{
				return thisVirtualCall<const char*>(0x78, this);
			}

			void TESForm::SetNewFormID(uint32_t NewIndex, bool Unk) 
			{
				thisVirtualCall<char*>(0x310, this, NewIndex, Unk);
			}

			const char* TESForm::GetEditorID() const
			{
				return thisVirtualCall<const char*>(0x1E8, this);
			}

			const char* TESForm::GetFullName() const
			{
				TESFullName* fullname = (TESFullName*)Core::_DYNAMIC_CAST(this, 0, "class TESForm", "class TESFullName");
				return fullname ? fullname->Name : "";
			}

			void TESForm::EditFormToWindow(HWND hParentWnd, __int64 a1, __int64 a2)
			{
				thisVirtualCall<const char*>(0x2D0, this, hParentWnd, a1, a2);
			}

			TESForm* TESForm::GetFormByNumericID(uint32_t SearchID)
			{
				return OldGetFormByNumericID(SearchID);
			}

			void* TESForm::AlteredFormList_Create(Array* Array, uint32_t Unknown)
			{
				AlteredFormListShadow.clear();
				return OldAlteredFormList_Create(Array, Unknown);
			}

			void TESForm::AlteredFormList_RemoveAllEntries(Array* Array, bool Unknown)
			{
				AlteredFormListShadow.clear();
				OldAlteredFormList_RemoveAllEntries(Array, Unknown);
			}

			void TESForm::AlteredFormList_Insert(Array* Array, TESForm*& Entry)
			{
				AlteredFormListShadow.insert(Entry);
				OldAlteredFormList_Insert(Array, Entry);
			}

			void TESForm::AlteredFormList_RemoveEntry(TESForm::Array* Array, uint32_t Index, uint32_t Unknown)
			{
				AlteredFormListShadow.erase(Array->at(Index));
				OldAlteredFormList_RemoveEntry(Array, Index, Unknown);
			}

			bool TESForm::AlteredFormList_ElementExists(TESForm::Array* Array, TESForm*& Entry)
			{
				return AlteredFormListShadow.count(Entry) > 0;
			}

			void TESForm::FormReferenceMap_RemoveAllEntries()
			{
				for (auto [k, v] : FormReferenceMap)
				{
					if (v)
						OldFormReferenceMap_RemoveEntry(v, 1);
				}

				FormReferenceMap.clear();
			}

			TESForm::Array* TESForm::FormReferenceMap_FindOrCreate(uint64_t Key, bool Create)
			{
				auto itr = FormReferenceMap.find(Key);

				if (itr != FormReferenceMap.end() && itr->second)
					return itr->second;

				if (Create)
				{
					auto* ptr = OldFormReferenceMap_Find(24);
					if (ptr)
						ptr = OldFormReferenceMap_Create(ptr);

					FormReferenceMap.insert_or_assign(Key, ptr);
					return ptr;
				}

				return nullptr;
			}

			void TESForm::FormReferenceMap_RemoveEntry(uint64_t Key)
			{
				auto itr = FormReferenceMap.find(Key);

				if (itr != FormReferenceMap.end())
				{
					TESForm::Array* ptr = itr->second;
					FormReferenceMap.erase(itr);

					if (ptr)
						OldFormReferenceMap_RemoveEntry(ptr, 1);
				}
			}

			bool TESForm::FormReferenceMap_Get(uint64_t Unused, uint64_t Key, Array** Value)
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