// Special thanks to Nukem: 
// https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/CKSSE/BSPointerHandleManager.h
// https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/CKSSE/BSPointerHandleManager.cpp

#pragma once

#include <vector>
#include <CKPE.Asserts.h>
#include <EditorAPI/NiAPI/NiPointer.h>
#include <EditorAPI/Forms/TESObjectREFR.h>
#include "BSHandleRefObject.h"
#include "BSReadWriteLock.h"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			using namespace NiAPI;

			template<typename _Ty, int IndexBits = 20, int AgeCountBits = 6>
			class IBSUntypedPointerHandle
			{
			public:
				//
				// NOTE: Handle index bits increased from 20 (vanilla) to 21 (limit doubled) or to 23 (limit doubled)
				//
				// 31       27       26    20             0
				// |--------|--------|-----|--------------|
				// | Unused | Active | Age | Handle Index |
				// |--------|--------|-----|--------------|
				//
				constexpr static _Ty INDEX_BITS = IndexBits;
				constexpr static _Ty AGE_BITS = AgeCountBits;
				constexpr static _Ty UNUSED_BIT_START = INDEX_BITS + AGE_BITS;							// 26 in vanilla

				constexpr static _Ty INDEX_MASK = (((_Ty)1) << INDEX_BITS) - ((_Ty)1);					// 0x00FFFFF
				constexpr static _Ty AGE_MASK = ((((_Ty)1) << AGE_BITS) - ((_Ty)1)) << INDEX_BITS;		// 0x3F00000
				constexpr static _Ty ACTIVE_BIT_MASK = ((_Ty)1) << UNUSED_BIT_START;					// 0x4000000

				constexpr static _Ty MAX_HANDLE_COUNT = ((_Ty)1) << INDEX_BITS;

			protected:
				_Ty m_Bits;
			public:
				inline void SetBitwiseNull() { m_Bits = 0; }
				inline bool IsBitwiseNull() const { return !m_Bits; }
				inline _Ty GetIndex() const { return m_Bits & INDEX_MASK; }
				inline _Ty GetAge() const { return m_Bits & AGE_MASK; }
				inline void SetInUse() { m_Bits |= ACTIVE_BIT_MASK; }
				inline void SetNotInUse() { m_Bits &= ~ACTIVE_BIT_MASK; }
				inline bool IsInUse() const { return (m_Bits & ACTIVE_BIT_MASK) != 0; }
				inline void IncrementAge() {
					m_Bits = ((m_Bits << INDEX_BITS) & AGE_MASK) | (m_Bits & ~AGE_MASK);
				}

				void Set(_Ty Index, _Ty Age)
				{
					CKPE_ASSERT_MSG(Index < MAX_HANDLE_COUNT, "BSUntypedPointerHandle::Set - parameter Index is too large");

					m_Bits = Index | Age;
				}

				void SetIndex(_Ty Index)
				{
					CKPE_ASSERT_MSG(Index < MAX_HANDLE_COUNT, "BSUntypedPointerHandle::Set - parameter Index is too large");

					m_Bits = (Index & INDEX_MASK) | (m_Bits & ~INDEX_MASK);
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

				IBSUntypedPointerHandle() : m_Bits(0) {}
			};
			static_assert(sizeof(IBSUntypedPointerHandle<std::uint32_t>) == 0x4);

			typedef IBSUntypedPointerHandle<std::uint32_t, 20, 6> BSUntypedPointerHandle_Original;
			typedef IBSUntypedPointerHandle<std::uint32_t, 21, 6> BSUntypedPointerHandle_Extended;
			typedef IBSUntypedPointerHandle<std::uint32_t, 23, 6> BSUntypedPointerHandle_Extended_Extremly;

			/*template<typename ObjectType, typename HandleType>
			class BSPointerHandle : public HandleType
			{};

			typedef BSPointerHandle<TESObjectREFR_Original, 
				BSUntypedPointerHandle_Original> BSPointerHandle_Original;
			typedef BSPointerHandle<TESObjectREFR_Original, 
				BSUntypedPointerHandle_Extended> BSPointerHandle_Extended;
			typedef BSPointerHandle<TESObjectREFR_Extremly, 
				BSUntypedPointerHandle_Extended_Extremly> BSPointerHandle_Extended_Extremly;

			static_assert(sizeof(BSPointerHandle_Original) == 0x4);
			static_assert(sizeof(BSPointerHandle_Extended) == 0x4);
			static_assert(sizeof(BSPointerHandle_Extended_Extremly) == 0x4);*/

			template<typename _Ty, typename HandleType, typename HandleRef>
			class IBSPointerHandleManagerEntry : public HandleType
			{
			private:
				NiPointer<HandleRef> m_Pointer;
			public:
				void SetNextFreeEntry(_Ty Index)
				{
					HandleType::m_Bits = (Index & HandleType::INDEX_MASK) | (HandleType::m_Bits & ~HandleType::INDEX_MASK);
				}

				_Ty GetNextFreeEntry() const
				{
					return HandleType::m_Bits & HandleType::INDEX_MASK;
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

			typedef IBSPointerHandleManagerEntry<std::uint32_t, BSUntypedPointerHandle_Original,
				BSHandleRefObject_Original> BSPointerHandleManagerEntry_Original;
			typedef IBSPointerHandleManagerEntry<std::uint32_t, BSUntypedPointerHandle_Extended,
				BSHandleRefObject_Original> BSPointerHandleManagerEntry_Extended;
			typedef IBSPointerHandleManagerEntry<std::uint32_t, BSUntypedPointerHandle_Extended_Extremly,
				BSHandleRefObject_Extremly> BSPointerHandleManagerEntry_Extended_Extremly;

			static_assert(sizeof(BSPointerHandleManagerEntry_Original) == 0x10);
			static_assert(sizeof(BSPointerHandleManagerEntry_Extended) == 0x10);
			static_assert(sizeof(BSPointerHandleManagerEntry_Extended_Extremly) == 0x10);

			template<typename _Ty, typename HandleType, typename HandleRef>
			class IBSPointerHandleManager
			{
			protected:
				inline static _Ty FreeListHead;
				inline static _Ty FreeListTail;
				inline static BSReadWriteLock HandleManagerLock;
				inline static std::vector<IBSPointerHandleManagerEntry<_Ty, HandleType, HandleRef>> HandleEntries;
				inline const static HandleType NullHandle;
			public:
				constexpr static _Ty INVALID_INDEX = (_Ty)-1;
			public:
				inline static _Ty GetHead() { return FreeListHead; }
				inline static _Ty GetTail() { return FreeListTail; }

				static void InitSDM()
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
				}
				static void KillSDM()
				{
					HandleManagerLock.LockForWrite();

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

					HandleManagerLock.UnlockWrite();
				}
			};

			typedef IBSPointerHandleManager<std::uint32_t, BSUntypedPointerHandle_Original,
				BSHandleRefObject_Original> BSPointerHandleManager_Original;
			typedef IBSPointerHandleManager<std::uint32_t, BSUntypedPointerHandle_Extended,
				BSHandleRefObject_Original> BSPointerHandleManager_Extended;
			typedef IBSPointerHandleManager<std::uint32_t, BSUntypedPointerHandle_Extended_Extremly,
				BSHandleRefObject_Extremly> BSPointerHandleManager_Extended_Extremly;

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
					CKPE_ASSERT_MSG(false, "Unimplemented");
				}
			};

			typedef IBSHandleManager<BSPointerHandleManager_Original> HandleManager_Original;
			typedef IBSHandleManager<BSPointerHandleManager_Extended> HandleManager_Extended;
			typedef IBSHandleManager<BSPointerHandleManager_Extended_Extremly> HandleManager_Extended_Extremly;

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

						CKPE_ASSERT_MSG(untypedHandle.GetAge() == handle.GetAge(),
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
					Manager::HandleManagerLock.LockForWrite();
					{
						untypedHandle = GetCurrentHandle(Refr);

						if (untypedHandle == Manager::NullHandle)
						{
							if (Manager::FreeListHead == Manager::INVALID_INDEX)
							{
								untypedHandle.SetBitwiseNull();
								CKPE_ASSERT_MSG_FMT(false, "OUT OF HANDLE ARRAY ENTRIES. Null handle created for pointer 0x%p.", Refr);
							}
							else
							{
								auto& newHandle = Manager::HandleEntries[Manager::FreeListHead];
								newHandle.IncrementAge();
								newHandle.SetInUse();

								untypedHandle.Set(newHandle.GetNextFreeEntry(), newHandle.GetAge());
								newHandle.SetPointer(Refr);

								Refr->SetHandleEntryIndex(Manager::FreeListHead);
								CKPE_ASSERT(Refr->GetHandleEntryIndex() == Manager::FreeListHead);

								if (newHandle.GetNextFreeEntry() == Manager::FreeListHead)
								{
									// Table reached the maximum count
									CKPE_ASSERT(Manager::FreeListHead == Manager::FreeListTail);

									Manager::FreeListHead = Manager::INVALID_INDEX;
									Manager::FreeListTail = Manager::INVALID_INDEX;
								}
								else
								{
									CKPE_ASSERT(Manager::FreeListHead != Manager::FreeListTail);
									Manager::FreeListHead = newHandle.GetNextFreeEntry();
								}
							}
						}
					}
					Manager::HandleManagerLock.UnlockWrite();

					return untypedHandle;
				}

				static void Destroy1(const HandleType& Handle)
				{
					if (Handle.IsBitwiseNull())
						return;

					Manager::HandleManagerLock.LockForWrite();
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
					Manager::HandleManagerLock.UnlockWrite();
				}

				static void Destroy2(HandleType& Handle)
				{
					if (Handle.IsBitwiseNull())
						return;

					Manager::HandleManagerLock.LockForWrite();
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
					Manager::HandleManagerLock.UnlockWrite();
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

				static ObjectType* GetPointer(std::uint32_t UniqueId)
				{
					const auto handleIndex = UniqueId & HandleType::INDEX_MASK;
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
		
			typedef IBSPointerHandleManagerInterface<Forms::TESObjectREFR_Original, 
				BSUntypedPointerHandle_Original, HandleManager_Original>
				BSPointerHandleManagerInterface_Original;

			typedef IBSPointerHandleManagerInterface<Forms::TESObjectREFR_Original,
				BSUntypedPointerHandle_Extended, HandleManager_Extended>
				BSPointerHandleManagerInterface_Extended;

			typedef IBSPointerHandleManagerInterface<Forms::TESObjectREFR_Extremly,
				BSUntypedPointerHandle_Extended_Extremly, HandleManager_Extended_Extremly>
				BSPointerHandleManagerInterface_Extended_Extremly;

			struct BSPointerHandleManagerCurrent
			{
				inline static std::uint8_t PointerHandleManagerCurrentId = 0;
			};

			static Forms::TESForm* GetRefFormByUniqueId(std::uint32_t UniqueId)
			{
				switch (BSPointerHandleManagerCurrent::PointerHandleManagerCurrentId)
				{
				case 1:
					return BSPointerHandleManagerInterface_Extended::GetPointer(UniqueId);
				case 2:
					return BSPointerHandleManagerInterface_Extended_Extremly::GetPointer(UniqueId);
				default:
					return BSPointerHandleManagerInterface_Original::GetPointer(UniqueId);
				}
			}
		}
	}
}