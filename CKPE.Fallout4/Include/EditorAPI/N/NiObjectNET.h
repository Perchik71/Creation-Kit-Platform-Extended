// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiObjectNET.h

#pragma once

#include <EditorAPI/N/NiObject.h>
#include <EditorAPI/N/NiTimeController.h>
#include <EditorAPI/B/BSFixedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiTimeController;
				class NiExtraDataContainer;
				// 18
				class NiExtraData : public NiObject
				{
				public:
					virtual ~NiExtraData() = 0;

					/* 2A */ [[nodiscard]] virtual bool KeepForExport() const noexcept(true);
					/* 2B */ [[nodiscard]] virtual bool KeepForExporterOnly() const noexcept(true);
					/* 2C */ [[nodiscard]] virtual bool IsStreamable() const noexcept(true);
					/* 2D */ [[nodiscard]] virtual bool IsCloneable() const noexcept(true);

					[[nodiscard]] inline const char* GetName() const noexcept(true) { return name; }
				private:
					BSFixedString name;  // 10
				public:
					[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
					{
						return offsetof(NiExtraData, name) == 0x10;
					}
				};
				static_assert(sizeof(NiExtraData) == 0x18);
				static_assert(NiExtraData::VerifyLayout());

				// 28
				class NiObjectNET : public NiObject
				{
				public:
					virtual ~NiObjectNET() = default;

					[[nodiscard]] inline const char* GetName() const noexcept(true) { return _ObjectName; }
					[[nodiscard]] inline NiTimeController* GetControllers() const noexcept(true) { return _Controllers; }
					[[nodiscard]] inline NiExtraDataContainer* GetExtraDataContainer() const noexcept(true) { return _ExtraData; }

					inline void GetViewerStrings(void(*Callback)(const char*, ...)) const noexcept(true)
					{
						Callback("-- NiObjectNET --\n");
						Callback("Name = %s\n", GetName());
					}
				private:
					BSFixedString _ObjectName;
					NiTimeController* _Controllers;
					NiExtraDataContainer* _ExtraData;
				public:
					[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
					{
						return offsetof(NiObjectNET, _ObjectName) == 0x10 &&
							offsetof(NiObjectNET, _Controllers) == 0x18 &&
							offsetof(NiObjectNET, _ExtraData) == 0x20;
					}
				};
				static_assert(sizeof(NiObjectNET) == 0x28);
				static_assert(NiObjectNET::VerifyLayout());
			}
		}
	}
}