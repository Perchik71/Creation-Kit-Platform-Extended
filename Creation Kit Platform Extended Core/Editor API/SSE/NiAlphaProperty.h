// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiAlphaProperty.h

#pragma once

#include "NiObjectNET.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			class NiProperty : public NiObjectNET
			{
			public:
				void GetViewerStrings(void(*Callback)(const char*, ...), bool Recursive) const
				{
					if (Recursive)
						__super::GetViewerStrings(Callback, Recursive);

					Callback("-- NiProperty --\n");
				}
			};
			static_assert(sizeof(NiProperty) == 0x30);

			class NiShadeProperty : public NiProperty
			{
			public:
				void GetViewerStrings(void(*Callback)(const char*, ...), bool Recursive) const
				{
					if (Recursive)
						__super::GetViewerStrings(Callback, Recursive);

					Callback("-- NiShadeProperty --\n");
				}
			};
			static_assert(sizeof(NiProperty) == 0x30);

			class NiAlphaProperty : public NiProperty
			{
			private:
				union
				{
					uint16_t Value;
					struct
					{
						uint16_t DoBlending : 1;
						uint16_t SrcBlend : 4;	// BlendFunction
						uint16_t DstBlend : 4;	// BlendFunction
						uint16_t DoTesting : 1;
						uint16_t TestFunc : 3;	// TestFunction
						uint16_t NoSorter : 1;
					};
				} AlphaFlags;
				uint8_t m_ucAlphaTestRef;

			public:
				bool GetAlphaBlending() const
				{
					return AlphaFlags.DoBlending;
				}

				bool GetAlphaTesting() const
				{
					return AlphaFlags.DoTesting;
				}

				uint8_t GetTestRef() const
				{
					return m_ucAlphaTestRef;
				}

				void GetViewerStrings(void(*Callback)(const char*, ...), bool Recursive) const
				{
					if (Recursive)
						__super::GetViewerStrings(Callback, Recursive);

					Callback("-- NiAlphaProperty --\n");
					Callback("Alpha Blend: %s\n", GetAlphaBlending() ? "true" : "false");
					Callback("Alpha Test: %s\n", GetAlphaTesting() ? "true" : "false");
					Callback("Test Ref: %u\n", (uint32_t)GetTestRef());
				}
			};
			static_assert(sizeof(NiAlphaProperty) == 0x38);
		}
	}
}