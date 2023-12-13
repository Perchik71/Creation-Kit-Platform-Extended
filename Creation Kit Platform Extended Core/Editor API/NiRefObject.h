// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiRefObject.h

#pragma once

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
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
	}
}