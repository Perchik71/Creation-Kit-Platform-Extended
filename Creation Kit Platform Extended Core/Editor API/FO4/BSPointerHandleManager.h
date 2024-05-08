//////////////////////////////////////////
/*
* Copyright (c) 2022-2024 Perchik71 <email:perchik71@outlook.com>
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

#include "NiAPI\NiTypes.h"
#include "NiAPI\NiPointer.h"
#include "..\\BSHandleRefObject.h"
#include "TESObjectREFR.h"
#include "NiClassesF4.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			// vanilla --- class BSUntypedPointerHandle<21,5>

			template <typename _Ty, int IndexBits = 21, int AgeCountBits = 5>
			class IBSUntypedPointerHandle
			{
			public:
				// NOTE: Handle index bits increased from 21 (vanilla)
				//
				// 27       26       21                   0
				// |--------|--------|--------------------|
				// | Active |    Age |    Handle Index    |
				// |--------|--------|--------------------|
				//
				constexpr static _Ty INDEX_BIT = IndexBits;
				constexpr static _Ty AGE_BIT = AgeCountBits;
				constexpr static _Ty UNUSED_BIT_START = INDEX_BIT + AGE_BIT;							// 21 in vanilla
								 
				constexpr static _Ty MASK_INDEX_BIT = (((_Ty)1) << INDEX_BIT) - ((_Ty)1);				// 03FFFFFF
				constexpr static _Ty MASK_AGE_BIT = ((((_Ty)1) << AGE_BIT) - ((_Ty)1)) << INDEX_BIT;	// 7C000000
				constexpr static _Ty MASK_ACTIVE_BIT = ((_Ty)1) << UNUSED_BIT_START;					// 80000000
				constexpr static _Ty MAX_HANDLE_COUNT = ((_Ty)1) << INDEX_BIT;							// 04000000
			protected:
				_Ty m_Bits;
			public:
				void SetBitwiseNull() { m_Bits = 0; }
				bool IsBitwiseNull() const { return !m_Bits; }
				_Ty GetIndex() const { return m_Bits & MASK_INDEX_BIT; }
				_Ty GetAge() const { return m_Bits & MASK_AGE_BIT; }
				void SetInUse() { m_Bits |= MASK_ACTIVE_BIT; }
				void SetNotInUse() { m_Bits &= ~MASK_ACTIVE_BIT; }
				inline bool IsInUse() const { return (m_Bits & MASK_ACTIVE_BIT) != 0; }
				inline void IncrementAge() 
				{
					m_Bits = ((m_Bits << INDEX_BIT) & MASK_AGE_BIT) | (m_Bits & ~MASK_AGE_BIT);
				}

				void Set(_Ty Index, _Ty Age)
				{
					AssertMsg(Index < MAX_HANDLE_COUNT, "IBSUntypedPointerHandle::Set - parameter Index is too large");

					m_Bits = Index | Age;
				}

				void SetIndex(_Ty Index)
				{
					AssertMsg(Index < MAX_HANDLE_COUNT, "IBSUntypedPointerHandle::Set - parameter Index is too large");

					m_Bits = (Index & MASK_INDEX_BIT) | (m_Bits & ~MASK_INDEX_BIT);
				}

				IBSUntypedPointerHandle& operator=(const IBSUntypedPointerHandle& Other)
				{
					m_Bits = Other.m_Bits;
					return *this;
				}

				bool operator==(const IBSUntypedPointerHandle& Other) const
				{
					return m_Bits == Other.m_Bits;
				}

				bool operator!=(const IBSUntypedPointerHandle& Other) const
				{
					return m_Bits != Other.m_Bits;
				}
			public:
				READ_PROPERTY(GetAge) _Ty Age;
				PROPERTY(GetIndex, SetIndex) _Ty Index;
			public:
				IBSUntypedPointerHandle() : m_Bits(0) {}
			};
			static_assert(sizeof(IBSUntypedPointerHandle<uint32_t>) == 0x4);

			typedef IBSUntypedPointerHandle<uint32_t, 21, 5> BSUntypedPointerHandle_Original;
			typedef IBSUntypedPointerHandle<uint32_t, 26, 5> BSUntypedPointerHandle_Extended;

			template<typename _Ty, typename HandleType, typename HandleRef>
			class IBSPointerHandleManagerEntry : public HandleType
			{
			private:
				NiPointer<HandleRef> m_Pointer;
			public:
				void SetNextFreeEntry(_Ty Index)
				{
					HandleType::m_Bits = (Index & HandleType::MASK_INDEX_BIT) | (HandleType::m_Bits & ~HandleType::MASK_INDEX_BIT);
				}

				_Ty GetNextFreeEntry() const
				{
					return HandleType::m_Bits & HandleType::MASK_INDEX_BIT;
				}

				void SetPointer(HandleRef* Pointer)
				{
					m_Pointer = Pointer;
				}

				HandleRef* GetPointer() const
				{
					return m_Pointer.operator HandleRef * ();
				}

				bool IsValid(_Ty Age) const
				{
					return HandleType::IsInUse() && HandleType::GetAge() == Age;
				}
			};

			typedef IBSPointerHandleManagerEntry<uint32_t, BSUntypedPointerHandle_Original,
				BSHandleRefObject_Original> BSPointerHandleManagerEntry_Original;
			typedef IBSPointerHandleManagerEntry<uint32_t, BSUntypedPointerHandle_Extended,
				BSHandleRefObject_64_Extremly> BSPointerHandleManagerEntry_Extended;

			static_assert(sizeof(BSPointerHandleManagerEntry_Original) == 0x10);
			static_assert(sizeof(BSPointerHandleManagerEntry_Extended) == 0x10);

			template<typename _Ty, typename HandleType, typename HandleRef>
			class IBSPointerHandleManager
			{
			protected:
				inline static _Ty FreeListHead;
				inline static _Ty FreeListTail;
				inline static BSReadWriteLock HandleManagerLock;
				inline static Array<IBSPointerHandleManagerEntry<_Ty, HandleType, HandleRef>> HandleEntries;
				inline const static HandleType NullHandle;
			public:
				constexpr static _Ty INVALID_INDEX = (_Ty)-1;
			public:
				inline static _Ty GetHead() { return FreeListHead; }
				inline static _Ty GetTail() { return FreeListTail; }

				static IBSPointerHandleManagerEntry<_Ty, HandleType, HandleRef>* InitSDM()
				{
					FreeListHead = 0;
					HandleEntries.resize(HandleType::MAX_HANDLE_COUNT);

					for (_Ty i = 0; i < HandleType::MAX_HANDLE_COUNT; i++)
					{
						if ((i + 1) >= HandleType::MAX_HANDLE_COUNT)
							HandleEntries[i].SetNextFreeEntry(i);
						else
							HandleEntries[i].SetNextFreeEntry(i + 1);
					}

					FreeListTail = HandleType::MAX_HANDLE_COUNT - 1;
					return HandleEntries.data();
				}

				static void KillSDM()
				{
					HandleManagerLock.TryLockForWrite();

					for (_Ty i = 0; i < HandleType::MAX_HANDLE_COUNT; i++)
					{
						auto& arrayHandle = HandleEntries[i];

						if (!arrayHandle.IsInUse())
							continue;

						if (arrayHandle.GetPointer())
							arrayHandle.GetPointer()->ClearHandleEntryIndex();

						arrayHandle.SetPointer(nullptr);
						arrayHandle.SetNotInUse();

						if (FreeListTail == INVALID_INDEX)
							FreeListHead = i;
						else
							HandleEntries[FreeListTail].SetNextFreeEntry(i);

						arrayHandle.SetNextFreeEntry(i);
						FreeListTail = i;
					}

					HandleManagerLock.Unlock();
				}
			};

			typedef IBSPointerHandleManager<uint32_t, BSUntypedPointerHandle_Original,
				BSHandleRefObject_Original> BSPointerHandleManager_Original;
			typedef IBSPointerHandleManager<uint64_t, BSUntypedPointerHandle_Extended,
				BSHandleRefObject_64_Extremly> BSPointerHandleManager_Extended;

			template<typename Manager>
			class IBSHandleManager : public Manager
			{
			public:
				static void KillSDM()
				{
					Manager::KillSDM();
				}

				static void WarnForUndestroyedHandles()
				{
					AssertMsg(false, "Unimplemented");
				}
			};

			typedef IBSHandleManager<BSPointerHandleManager_Original> HandleManager_Original;
			typedef IBSHandleManager<BSPointerHandleManager_Extended> HandleManager_Extended;

			template<typename ObjectType, typename HandleType, typename Manager>
			class IBSPointerHandleManagerInterface : public Manager
			{
			public:
				static HandleType GetCurrentHandle(ObjectType* Refr)
				{
					HandleType untypedHandle;

					if (Refr && Refr->IsHandleValid())
					{
						auto& handle = Manager::HandleEntries[Refr->GetHandleEntryIndex()];
						untypedHandle.Set(Refr->GetHandleEntryIndex(), handle.GetAge());

						AssertMsg(untypedHandle.GetAge() == handle.GetAge(),
							"BSPointerHandleManagerInterface::GetCurrentHandle - Handle already exists but has wrong age!");
					}

					return untypedHandle;
				}

				static HandleType CreateHandle(ObjectType* Refr)
				{
					HandleType untypedHandle;

					if (!Refr)
						return untypedHandle;

					// Shortcut: Check if the handle is already valid
					untypedHandle = GetCurrentHandle(Refr);

					if (untypedHandle != Manager::NullHandle)
						return untypedHandle;

					// Wasn't present. Acquire lock and add it (unless someone else inserted it in the meantime)
					Manager::HandleManagerLock.TryLockForWrite();
					{
						untypedHandle = GetCurrentHandle(Refr);

						if (untypedHandle == Manager::NullHandle)
						{
							if (Manager::FreeListHead == Manager::INVALID_INDEX)
							{
								untypedHandle.SetBitwiseNull();
								AssertMsgVa(false, "OUT OF HANDLE ARRAY ENTRIES. Null handle created for pointer 0x%p.", Refr);
							}
							else
							{
								auto& newHandle = Manager::HandleEntries[Manager::FreeListHead];
								newHandle.IncrementAge();
								newHandle.SetInUse();

								untypedHandle.Set(newHandle.GetNextFreeEntry(), newHandle.GetAge());
								newHandle.SetPointer(Refr);

								Refr->SetHandleEntryIndex(Manager::FreeListHead);
								Assert(Refr->GetHandleEntryIndex() == Manager::FreeListHead);

								if (newHandle.GetNextFreeEntry() == Manager::FreeListHead)
								{
									// Table reached the maximum count
									Assert(Manager::FreeListHead == Manager::FreeListTail);

									Manager::FreeListHead = Manager::INVALID_INDEX;
									Manager::FreeListTail = Manager::INVALID_INDEX;
								}
								else
								{
									Assert(Manager::FreeListHead != Manager::FreeListTail);
									Manager::FreeListHead = newHandle.GetNextFreeEntry();
								}
							}
						}
					}
					Manager::HandleManagerLock.Unlock();

					return untypedHandle;
				}

				static void Destroy1(const HandleType& Handle)
				{
					if (Handle.IsBitwiseNull())
						return;

					Manager::HandleManagerLock.TryLockForWrite();
					{
						const auto handleIndex = Handle.GetIndex();
						auto& arrayHandle = Manager::HandleEntries[handleIndex];

						if (arrayHandle.IsValid(Handle.GetAge()))
						{
							arrayHandle.GetPointer()->ClearHandleEntryIndex();
							arrayHandle.SetPointer(nullptr);
							arrayHandle.SetNotInUse();

							if (Manager::FreeListTail == Manager::INVALID_INDEX)
								Manager::FreeListHead = handleIndex;
							else
								Manager::HandleEntries[Manager::FreeListTail].SetNextFreeEntry(handleIndex);

							arrayHandle.SetNextFreeEntry(handleIndex);
							Manager::FreeListTail = handleIndex;
						}
					}
					Manager::HandleManagerLock.Unlock();
				}

				static void Destroy2(HandleType& Handle)
				{
					if (Handle.IsBitwiseNull())
						return;

					Manager::HandleManagerLock.TryLockForWrite();
					{
						const auto handleIndex = Handle.GetIndex();
						auto& arrayHandle = Manager::HandleEntries[handleIndex];

						if (arrayHandle.IsValid(Handle.GetAge()))
						{
							arrayHandle.GetPointer()->ClearHandleEntryIndex();
							arrayHandle.SetPointer(nullptr);
							arrayHandle.SetNotInUse();

							if (Manager::FreeListTail == Manager::INVALID_INDEX)
								Manager::FreeListHead = handleIndex;
							else
								Manager::HandleEntries[Manager::FreeListTail].SetNextFreeEntry(handleIndex);

							arrayHandle.SetNextFreeEntry(handleIndex);
							Manager::FreeListTail = handleIndex;
						}

						// Identical to Destroy1 except for this Handle.SetBitwiseNull();
						Handle.SetBitwiseNull();
					}
					Manager::HandleManagerLock.Unlock();
				}

				static bool GetSmartPointer1(const HandleType& Handle, NiPointer<ObjectType>& Out)
				{
					if (Handle.IsBitwiseNull())
					{
						Out = nullptr;
						return false;
					}

					const auto handleIndex = Handle.GetIndex();
					auto& arrayHandle = Manager::HandleEntries[handleIndex];

					Out = static_cast<ObjectType*>(arrayHandle.GetPointer());

					if (!arrayHandle.IsValid(Handle.GetAge()) || Out->GetHandleEntryIndex() != handleIndex)
						Out = nullptr;

					return Out != nullptr;
				}

				static bool GetSmartPointer2(HandleType& Handle, NiPointer<ObjectType>& Out)
				{
					if (Handle.IsBitwiseNull())
					{
						Out = nullptr;
						return false;
					}

					const auto handleIndex = Handle.GetIndex();
					auto& arrayHandle = Manager::HandleEntries[handleIndex];

					Out = static_cast<ObjectType*>(arrayHandle.GetPointer());

					if (!arrayHandle.IsValid(Handle.GetAge()) || Out->GetHandleEntryIndex() != handleIndex)
					{
						// Identical to GetSmartPointer1 except for this Handle.SetBitwiseNull();
						Handle.SetBitwiseNull();
						Out = nullptr;
					}

					return Out != nullptr;
				}

				static ObjectType* GetPointer(uint32_t UniqueId)
				{
					const auto handleIndex = UniqueId & HandleType::MASK_INDEX_BIT;
					auto& arrayHandle = Manager::HandleEntries[handleIndex];
					auto Out = static_cast<ObjectType*>(arrayHandle.GetPointer());

					if (Out->GetHandleEntryIndex() != handleIndex)
						Out = nullptr;

					return Out;
				}

				static bool IsValid(const HandleType& Handle)
				{
					const auto handleIndex = Handle.GetIndex();
					auto& arrayHandle = Manager::HandleEntries[handleIndex];

					// Handle.IsBitwiseNull(); -- This if() is optimized away because the result is irrelevant

					if (!arrayHandle.IsValid(Handle.GetAge()))
						return false;

					return arrayHandle.GetPointer()->GetHandleEntryIndex() == handleIndex;
				}
			};

			typedef IBSPointerHandleManagerInterface<TESObjectREFR_Original,
				BSUntypedPointerHandle_Original, HandleManager_Original>
				BSPointerHandleManagerInterface_Original;

			typedef IBSPointerHandleManagerInterface<TESObjectREFR_Extremly,
				BSUntypedPointerHandle_Extended, HandleManager_Extended>
				BSPointerHandleManagerInterface_Extended;

			struct BSPointerHandleManagerCurrent
			{
				inline static uint8_t PointerHandleManagerCurrentId = 0;
			};

			static TESForm* GetRefFormByUniqueId(uint32_t UniqueId)
			{
				switch (BSPointerHandleManagerCurrent::PointerHandleManagerCurrentId)
				{
				case 1:
					return BSPointerHandleManagerInterface_Extended::GetPointer(UniqueId);
				default:
					return BSPointerHandleManagerInterface_Original::GetPointer(UniqueId);
				}
			}
		}
	}
}
