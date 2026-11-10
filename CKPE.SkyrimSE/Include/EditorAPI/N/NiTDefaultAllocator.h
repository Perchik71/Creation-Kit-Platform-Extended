#pragma once

#include <EditorAPI/N/NiTCollection.h>

#define NiTDefaultAllocator DFALL

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				template <class T>
				class NiTDefaultAllocator
				{
				public:
					void* Allocate()
					{
						return NiMalloc(sizeof(T));
					}

					void Deallocate(void* a_ptr)
					{
						return NiFree(a_ptr);
					}
				};
			}
		}
	}
}
