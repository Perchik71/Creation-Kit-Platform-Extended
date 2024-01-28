// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiRefObject.h

#pragma once

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		// 10
		class __declspec(align(8)) NiRefObject
		{
		protected:
			volatile ULONG m_uiRefCount;

		public:
			NiRefObject() : m_uiRefCount(0)
			{
				InterlockedIncrement(&m_uiRefCount);
			}

			virtual ~NiRefObject()
			{
				InterlockedDecrement(&m_uiRefCount);
			}

			virtual void DeleteThis()
			{
				if (this)
					this->~NiRefObject();
			}

			uint32_t IncRefCount()
			{
				return (uint32_t)InterlockedIncrement(&m_uiRefCount);
			}

			uint32_t DecRefCount()
			{
				uint32_t count = (uint32_t)InterlockedDecrement(&m_uiRefCount);

				if (count <= 0)
					DeleteThis();

				return count;
			}

			void GetViewerStrings(void(*Callback)(const char*, ...), bool Recursive) const
			{
				Callback("-- NiRefObject --\n");
				Callback("This = 0x%p\n", this);
				Callback("Ref Count = %u\n", m_uiRefCount);
			}
		};

		// 10
		class __declspec(align(8)) NiRefObject_64
		{
		protected:
			volatile LONGLONG m_uiRefCount;	// 08

		public:
			NiRefObject_64() : m_uiRefCount(0)
			{
				InterlockedIncrement64(&m_uiRefCount);
			}

			virtual ~NiRefObject_64()
			{
				InterlockedDecrement64(&m_uiRefCount);
			}

			virtual void DeleteThis()
			{
				if (this)
					this->~NiRefObject_64();
			}

			uint64_t IncRefCount()
			{
				return (uint64_t)InterlockedIncrement64(&m_uiRefCount);
			}

			uint64_t DecRefCount()
			{
				uint64_t count = (uint64_t)InterlockedDecrement64(&m_uiRefCount);

				if (count <= 0)
					DeleteThis();

				return count;
			}
		};
	}
}