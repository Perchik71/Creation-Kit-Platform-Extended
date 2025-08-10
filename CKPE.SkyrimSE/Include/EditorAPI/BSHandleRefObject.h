// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/CKSSE/BSHandleRefObject_CK.h

#include <windows.h>
#include <CKPE.Asserts.h>
#include <EditorAPI/NiAPI/NiPointer.h>

#pragma once

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			template<std::uint32_t RefCountBits>
			class BSHandleRefObject32T : public NiAPI::NiRefObject
			{
			public:
				//
				// 31             +1       RefCountBits    0
				// |--------------|--------|---------------|
				// | Handle Index | Active |   Ref Count   |
				// |--------------|--------|---------------|
				//
				constexpr static std::uint32_t ACTIVE_BIT_INDEX = (std::uint32_t)RefCountBits;
				constexpr static std::uint32_t HANDLE_BIT_INDEX = (std::uint32_t)ACTIVE_BIT_INDEX + 1;
				constexpr static std::uint32_t REF_COUNT_MASK = (1ull << ACTIVE_BIT_INDEX) - 1ull;

			public:
				BSHandleRefObject32T() noexcept(true)
				{ ClearHandleEntryIndex(); }
				virtual ~BSHandleRefObject32T() noexcept(true)
				{ ClearHandleEntryIndex(); }

				inline std::uint32_t IncRefCount() noexcept(true)
				{
					CKPE_ASSERT_MSG(GetRefCount() < REF_COUNT_MASK,
						"BSHandleRefObject - IncRefCount is about to cause refcount wraparound to 0.");

					return ((NiRefObject*)this)->IncRefCount();
				}

				inline std::uint32_t DecRefCount() noexcept(true)
				{
					CKPE_ASSERT_MSG(GetRefCount() != 0,
						"BSHandleRefObject - DecRefCount called with refcount already 0.");

					return ((NiRefObject*)this)->DecRefCount();
				}

				[[nodiscard]] inline std::uint32_t GetRefCount() const noexcept(true)
				{ return ((std::uint32_t)m_uiRefCount) & REF_COUNT_MASK; }

				inline void SetHandleEntryIndex(std::uint32_t HandleIndex) noexcept(true)
				{
					m_uiRefCount = (std::uint32_t)(((std::uint64_t)HandleIndex << HANDLE_BIT_INDEX) |
						(1ull << ACTIVE_BIT_INDEX) | GetRefCount());
				}

				[[nodiscard]] inline std::uint32_t GetHandleEntryIndex() const noexcept(true)
				{ return ((std::uint32_t)m_uiRefCount) >> HANDLE_BIT_INDEX; }
				inline void ClearHandleEntryIndex() noexcept(true)
				{ m_uiRefCount &= REF_COUNT_MASK; }
				[[nodiscard]] inline bool IsHandleValid() const noexcept(true)
				{ return ((std::uint32_t)m_uiRefCount & (1ull << ACTIVE_BIT_INDEX)) != 0; }
			};
			static_assert(sizeof(BSHandleRefObject32T<10>) == 0x10);

			typedef BSHandleRefObject32T<10> BSHandleRefObject_Original;
			typedef BSHandleRefObject32T<8> BSHandleRefObject_Extremly;

			template<std::uint64_t RefCountBits>
			class BSHandleRefObject64T : public NiAPI::NiRefObject64
			{
			public:
				//
				// 63             +1       RefCountBits    0
				// |--------------|--------|---------------|
				// | Handle Index | Active |   Ref Count   |
				// |--------------|--------|---------------|
				//
				constexpr static std::uint64_t ACTIVE_BIT_INDEX = (std::uint64_t)RefCountBits;
				constexpr static std::uint64_t HANDLE_BIT_INDEX = (std::uint64_t)ACTIVE_BIT_INDEX + 1;
				constexpr static std::uint64_t REF_COUNT_MASK = (1ull << ACTIVE_BIT_INDEX) - 1ull;

			public:
				BSHandleRefObject64T() noexcept(true)
				{ ClearHandleEntryIndex(); }
				virtual ~BSHandleRefObject64T() noexcept(true)
				{ ClearHandleEntryIndex(); }

				inline std::uint64_t IncRefCount() noexcept(true)
				{
					CKPE_ASSERT_MSG(GetRefCount() < REF_COUNT_MASK,
						"BSHandleRefObject - IncRefCount is about to cause refcount wraparound to 0.");

					return ((NiRefObject64*)this)->IncRefCount();
				}

				inline std::uint64_t DecRefCount() noexcept(true)
				{
					CKPE_ASSERT_MSG(GetRefCount() != 0,
						"BSHandleRefObject - DecRefCount called with refcount already 0.");

					return ((NiRefObject64*)this)->DecRefCount();
				}

				[[nodiscard]] inline std::uint64_t GetRefCount() const noexcept(true)
				{ return ((std::uint64_t)m_uiRefCount) & REF_COUNT_MASK; }

				inline void SetHandleEntryIndex(std::uint64_t HandleIndex) noexcept(true)
				{
					m_uiRefCount = (LONGLONG)((HandleIndex << HANDLE_BIT_INDEX) |
						(1ull << ACTIVE_BIT_INDEX) | GetRefCount());
				}

				[[nodiscard]] inline std::uint64_t GetHandleEntryIndex() const noexcept(true)
				{ return ((std::uint64_t)m_uiRefCount) >> HANDLE_BIT_INDEX; }
				inline void ClearHandleEntryIndex() noexcept(true)
				{ m_uiRefCount &= REF_COUNT_MASK; }
				[[nodiscard]] inline bool IsHandleValid() const noexcept(true)
				{ return ((std::uint64_t)m_uiRefCount & (1ull << ACTIVE_BIT_INDEX)) != 0; }
			};
			static_assert(sizeof(BSHandleRefObject64T<10>) == 0x10);

			typedef BSHandleRefObject64T<10> BSHandleRefObject_64_Extremly;
		}
	}
}