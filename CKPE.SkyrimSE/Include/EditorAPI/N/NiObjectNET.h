// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiObjectNET.h

#pragma once

#include <EditorAPI/N/NiTArray.h>
#include <EditorAPI/N/NiTimeController.h>
#include <EditorAPI/B/BSFixedString.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				// 18
				class NiExtraData : public NiObject
				{
				public:
					virtual ~NiExtraData() = 0;

					// add
					[[nodiscard]] virtual bool IsStreamable() const;
					[[nodiscard]] virtual bool IsCloneable() const;

					static NiExtraData* Create(std::size_t a_size, std::uintptr_t a_vtbl);

					[[nodiscard]] inline const char* GetName() const noexcept(true) { return name; }
					inline void SetName(const BSFixedString& a_name) noexcept(true) { name = a_name; }
				private:
					BSFixedString name;
				};
				static_assert(sizeof(NiExtraData) == 0x18);

				// 30
				class NiObjectNET : public NiObject
				{
				public:
					virtual ~NiObjectNET() = default;

					[[nodiscard]] inline const char* GetName() const noexcept(true) { return _ObjectName; }

					inline void GetViewerStrings(void(*Callback)(const char*, ...)) const noexcept(true)
					{
						Callback("-- NiObjectNET --\n");
						Callback("Name = %s\n", _ObjectName);
					}

					[[nodiscard]] NiExtraData* GetExtraData(const BSFixedString& a_key) const noexcept(true);

					template <class T>
					[[nodiscard]] T* GetExtraData(const BSFixedString& a_key) const noexcept(true);

					[[nodiscard]] NiExtraData* GetExtraDataAt(std::uint16_t a_extraDataIndex) const noexcept(true);
					[[nodiscard]] std::uint16_t GetExtraDataSize() const noexcept(true);
					[[nodiscard]] bool HasExtraData(const BSFixedString& a_key) const noexcept(true);
				private:
					BSFixedString _ObjectName;
					NiPointer<NiTimeController> _Controllers;
					NiTArray<NiExtraData*> _ExtraData;
				};
				static_assert(sizeof(NiObjectNET) == 0x30);
			}
		}
	}
}