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
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// vanilla --- class BSUntypedPointerHandle<21,5>
			template<typename _Ty, int IndexBits = 21, int AgeCountBits = 5>
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
				constexpr static _Ty UNUSED_BIT_START = INDEX_BIT + AGE_BIT;							// 26 in vanilla

				constexpr static _Ty MASK_INDEX_BIT = (((_Ty)1) << INDEX_BIT) - ((_Ty)1);				// 03FFFFFF
				constexpr static _Ty MASK_AGE_BIT = ((((_Ty)1) << AGE_BIT) - ((_Ty)1)) << INDEX_BIT;	// 7C000000
				constexpr static _Ty MASK_ACTIVE_BIT = ((_Ty)1) << UNUSED_BIT_START;					// 80000000
				constexpr static _Ty MAX_HANDLE_COUNT = ((_Ty)1) << INDEX_BIT;							// 04000000
			protected:
				_Ty m_Bits;
			public:
				inline void SetBitwiseNull() noexcept(true) { m_Bits = 0; }
				[[nodiscard]] inline bool IsBitwiseNull() const noexcept(true) { return !m_Bits; }
				[[nodiscard]] inline _Ty GetIndex() const noexcept(true) { return m_Bits & MASK_INDEX_BIT; }
				[[nodiscard]] inline _Ty GetAge() const noexcept(true) { return m_Bits & MASK_AGE_BIT; }
				inline void SetInUse() noexcept(true) { m_Bits |= MASK_ACTIVE_BIT; }
				inline void SetNotInUse() noexcept(true) { m_Bits &= ~MASK_ACTIVE_BIT; }
				[[nodiscard]] inline bool IsInUse() const noexcept(true) { return (m_Bits & MASK_ACTIVE_BIT) != 0; }
				inline void IncrementAge() noexcept(true)
				{
					m_Bits = ((m_Bits << INDEX_BIT) & MASK_AGE_BIT) | (m_Bits & ~MASK_AGE_BIT);
				}

				void Set(_Ty Index, _Ty Age) noexcept(true)
				{
					CKPE_ASSERT_MSG(Index < MAX_HANDLE_COUNT, "BSUntypedPointerHandle::Set - parameter Index is too large");

					m_Bits = Index | Age;
				}

				void SetIndex(_Ty Index) noexcept(true)
				{
					CKPE_ASSERT_MSG(Index < MAX_HANDLE_COUNT, "BSUntypedPointerHandle::Set - parameter Index is too large");

					m_Bits = (Index & MASK_INDEX_BIT) | (m_Bits & ~MASK_INDEX_BIT);
				}


				IBSUntypedPointerHandle& operator=(const IBSUntypedPointerHandle& Other) noexcept(true)
				{
					m_Bits = Other.m_Bits;
					return *this;
				}

				bool operator==(const IBSUntypedPointerHandle& Other) const noexcept(true)
				{
					return m_Bits == Other.m_Bits;
				}

				bool operator!=(const IBSUntypedPointerHandle& Other) const noexcept(true)
				{
					return m_Bits != Other.m_Bits;
				}

				IBSUntypedPointerHandle() : m_Bits(0) {}
			};
			static_assert(sizeof(IBSUntypedPointerHandle<std::uint32_t>) == 0x4);

			typedef IBSUntypedPointerHandle<std::uint32_t, 21, 5> BSUntypedPointerHandle_Original;
			typedef IBSUntypedPointerHandle<std::uint32_t, 23, 5> BSUntypedPointerHandle_Extended_NG;
			typedef IBSUntypedPointerHandle<std::uint32_t, 26, 5> BSUntypedPointerHandle_Extended;

			template<typename _Ty, typename HandleType, typename HandleRef>
			class IBSPointerHandleManagerEntry : public HandleType
			{
			private:
				NiAPI::NiPointer<HandleRef> m_Pointer;
			public:
				inline void SetNextFreeEntry(_Ty Index) noexcept(true)
				{
					HandleType::m_Bits = (Index & HandleType::MASK_INDEX_BIT) | (HandleType::m_Bits & ~HandleType::MASK_INDEX_BIT);
				}

				[[nodiscard]] inline _Ty GetNextFreeEntry() const noexcept(true)
				{
					return HandleType::m_Bits & HandleType::MASK_INDEX_BIT;
				}

				inline void SetPointer(HandleRef* Pointer) noexcept(true) { m_Pointer = Pointer; }
				[[nodiscard]] inline HandleRef* GetPointer() const noexcept(true) { return m_Pointer.operator HandleRef* (); }

				[[nodiscard]] inline bool IsValid(_Ty Age) const noexcept(true)
				{
					return HandleType::IsInUse() && HandleType::GetAge() == Age;
				}
			};

			typedef IBSPointerHandleManagerEntry<uint32_t, BSUntypedPointerHandle_Original,
				BSHandleRefObject_Original> BSPointerHandleManagerEntry_Original;
			typedef IBSPointerHandleManagerEntry<uint32_t, BSUntypedPointerHandle_Extended_NG,
				BSHandleRefObject_Extremly> BSPointerHandleManagerEntry_Extended_NG;
			typedef IBSPointerHandleManagerEntry<uint32_t, BSUntypedPointerHandle_Extended,
				BSHandleRefObject_64_Extremly> BSPointerHandleManagerEntry_Extended;

			static_assert(sizeof(BSPointerHandleManagerEntry_Original) == 0x10);
			static_assert(sizeof(BSPointerHandleManagerEntry_Extended_NG) == 0x10);
			static_assert(sizeof(BSPointerHandleManagerEntry_Extended) == 0x10);

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
				[[nodiscard]] inline static _Ty GetHead() { return FreeListHead; }
				[[nodiscard]] inline static _Ty GetTail() { return FreeListTail; }

				static IBSPointerHandleManagerEntry<_Ty, HandleType, HandleRef>* InitSDM()
				{
					HandleEntries.resize(HandleType::MAX_HANDLE_COUNT);
					// Reset
					CleanSDM();
					// Return address
					return HandleEntries.data();
				}

				static void CleanSDM()
				{
					FreeListHead = 0;

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
			typedef IBSPointerHandleManager<std::uint64_t, BSUntypedPointerHandle_Extended_NG,
				BSHandleRefObject_Extremly> BSPointerHandleManager_Extended_NG;
			typedef IBSPointerHandleManager<std::uint64_t, BSUntypedPointerHandle_Extended,
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
					CKPE_ASSERT_MSG(false, "Unimplemented");
				}
			};

			typedef IBSHandleManager<BSPointerHandleManager_Original> HandleManager_Original;
			typedef IBSHandleManager<BSPointerHandleManager_Extended_NG> HandleManager_Extended_NG;
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

				static bool GetSmartPointer1(const HandleType& Handle, NiAPI::NiPointer<ObjectType>& Out)
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

				static bool GetSmartPointer2(HandleType& Handle, NiAPI::NiPointer<ObjectType>& Out)
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
		
			typedef IBSPointerHandleManagerInterface<Forms::TESObjectREFR_Original,
				BSUntypedPointerHandle_Original, HandleManager_Original>
				BSPointerHandleManagerInterface_Original;
			typedef IBSPointerHandleManagerInterface<Forms::TESObjectREFR_Extremly_NG,
				BSUntypedPointerHandle_Extended_NG, HandleManager_Extended_NG>
				BSPointerHandleManagerInterface_Extended_NG;
			typedef IBSPointerHandleManagerInterface<Forms::TESObjectREFR_Extremly,
				BSUntypedPointerHandle_Extended, HandleManager_Extended>
				BSPointerHandleManagerInterface_Extended;

			struct BSPointerHandleManagerCurrent
			{
				inline static std::uint8_t PointerHandleManagerCurrentId = 0;
			};

			static Forms::TESForm* GetRefFormByUniqueId(std::uint32_t UniqueId) noexcept(true)
			{
				switch (BSPointerHandleManagerCurrent::PointerHandleManagerCurrentId)
				{
				case 1:
					return BSPointerHandleManagerInterface_Extended::GetPointer(UniqueId);
				case 2:
					return BSPointerHandleManagerInterface_Extended_NG::GetPointer(UniqueId);
				default:
					return BSPointerHandleManagerInterface_Original::GetPointer(UniqueId);
				}
			}
		}
	}
}