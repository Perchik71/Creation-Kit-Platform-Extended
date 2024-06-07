// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiAlphaProperty.h

#pragma once

#include "NiObjectNET.h"

namespace CreationKitPlatformExtended
{
	namespace NiAPI
	{
		namespace Templates
		{
			template<typename T, uint32_t REL_GETTYPE>
			class NiPropertyT : public T
			{
			public:
				enum
				{
					kTypeAlpha = 0,
					kTypeShade = 1
				};

				inline void GetType() const { thisVirtualCall<void>(REL_GETTYPE, this); }
			};

			template<typename T>
			class NiAlphaPropertyT : public T
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
				uint8_t alphaThreshold;

			public:
				bool GetAlphaBlending() const
				{
					return AlphaFlags.DoBlending;
				}

				bool GetAlphaTesting() const
				{
					return AlphaFlags.DoTesting;
				}

				uint8_t GetThreshold() const
				{
					return alphaThreshold;
				}

				void GetViewerStrings(void(*Callback)(const char*, ...), bool Recursive) const
				{
					if (Recursive)
						__super::GetViewerStrings(Callback, Recursive);

					Callback("-- NiAlphaProperty --\n");
					Callback("Alpha Blend: %s\n", GetAlphaBlending() ? "true" : "false");
					Callback("Alpha Test: %s\n", GetAlphaTesting() ? "true" : "false");
					Callback("Threshold: %u\n", (uint32_t)GetThreshold());
				}
			};
		}

		class NiProperty : public Templates::NiPropertyT<NiObjectNET, 0x138>
		{};
		static_assert(sizeof(NiProperty) == 0x30);

		class NiShadeProperty : public NiProperty
		{};
		static_assert(sizeof(NiShadeProperty) == 0x30);

		class NiAlphaProperty : public Templates::NiAlphaPropertyT<NiProperty>
		{};
		static_assert(sizeof(NiAlphaProperty) == 0x38);

		namespace Fallout4
		{
			class NiProperty : public Templates::NiPropertyT<NiObjectNET, 0x150>
			{};
			static_assert(sizeof(NiProperty) == 0x28);

			class NiShadeProperty : public NiProperty
			{};
			static_assert(sizeof(NiProperty) == 0x28);

			class NiAlphaProperty : public Templates::NiAlphaPropertyT<NiProperty>
			{};
			static_assert(sizeof(NiAlphaProperty) == 0x30);
		}
	}
}