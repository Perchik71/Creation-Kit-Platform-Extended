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
#pragma pack(push, 4)

#include "TESFormF4.h"
#include "TESObjectREFR.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			class TESObjectCELL;
			class BGSLayer : public TESForm 
			{
			private:
				enum { eTypeID = FormType::ftLayer };
			private:
				BGSLayer* _parent;
				BSTArray<TESObjectREFR_Original*> _items;
				BSTArray<BGSLayer*> _childs;
			public:
				inline uint32_t GetChildsCount() const
				{
					return (uint32_t)_childs.QSize();
				}
				inline const BGSLayer** GetChildsArrayConst() const
				{
					return (const BGSLayer**)_childs.QBuffer();
				}
				inline BGSLayer** GetChildsArray() 
				{
					return (BGSLayer**)_childs.QBuffer();
				}
				inline BGSLayer* GetChildById(uint32_t dwId)
				{
					return _childs[dwId];
				}
				inline uint32_t GetItemsCount() const
				{
					return (uint32_t)_items.QSize();
				}
				inline const TESObjectREFR_Original** GetItemsArrayConst() const
				{
					return (const TESObjectREFR_Original**)_items.QBuffer();
				}
				inline TESObjectREFR_Original** GetItemsArray()
				{
					return (TESObjectREFR_Original**)_items.QBuffer();
				}
				inline TESObjectREFR_Original* GetItemById(uint32_t dwId)
				{
					return _items[dwId];
				}
				inline const BGSLayer* GetParentConst() const 
				{
					return _parent;
				}
				inline BGSLayer* GetParent() 
				{
					return _parent;
				}
				inline bool IsParent() const 
				{
					return _parent != NULL;
				}
			public:
				uint32_t GetItemsCountInCell(const TESObjectCELL* cell) const;
			public:
				inline TESObjectREFR_Original* operator[](uint32_t dwId) {
					return _items[dwId];
				}
				inline const TESObjectREFR_Original* operator[](uint32_t dwId) const {
					return _items[dwId];
				}
			public:
				READ_PROPERTY(GetChildsCount) uint32_t ChildsCount;
				READ_PROPERTY(GetChildsArrayConst) const BGSLayer** ChildsArrayConst;
				READ_PROPERTY(GetChildsArray) BGSLayer** ChildsArray;
				READ_PROPERTY(GetItemsCount) uint32_t ItemsCount;
				READ_PROPERTY(GetItemsArrayConst) const TESObjectREFR_Original** ItemsArrayConst;
				READ_PROPERTY(GetItemsArray) TESObjectREFR_Original** ItemsArray;
				READ_PROPERTY(GetParentConst) const BGSLayer* ParentConst;
				READ_PROPERTY(GetParent) BGSLayer* Parent;
			};
		}
	}
}

#pragma pack(pop)