#include <CKPE.Common.ID.h>

#pragma once

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace IDs
			{
				namespace StringCache
				{
					inline constexpr Common::ID StringPool_AnsiRelease{ 216797, 1016197 };
					inline constexpr Common::ID StringPool_WideRelease{ 758662, 1016177 };

					inline constexpr Common::ID BucketTable_Singleton{ 591027 };

					inline constexpr Common::ID FixedString_AnsiCreate{ 604959 };
					inline constexpr Common::ID FixedString_WideCreate{ 519648 };
				}

				namespace BSResource
				{
					namespace ID
					{
						inline constexpr Common::ID GenerateFromPath{ 758809, 1014466 };
					}
				}

				namespace TESDataHandler
				{
					inline constexpr Common::ID Instance{ 476691 };
					inline constexpr Common::ID UserModded{ 223341 };
				}

				namespace TESForm
				{
					inline constexpr Common::ID GetFile{ 569205 };
					inline constexpr Common::ID GetFormByNumericID{ 76804 };
				}
			}
		}
	}
}


