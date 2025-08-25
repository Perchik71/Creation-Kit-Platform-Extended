// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/BSSpinLock.h

#pragma once

#include <cstdint>

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace BSReflection
			{
				template<typename T>
				class ReflectionBase
				{

				};

				class IType
				{

				};

				class ClassType : public ReflectionBase<ClassType>, public IType
				{

				};

				class IObject
				{
				public:
					virtual ClassType* GetType();
					virtual ~IObject();
				};
			}
		}
	}
}