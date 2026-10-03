#pragma once

#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/N/NiAVObject.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESWaterForm;
				class TESObjectREFR;
				class TESObject : public TESForm
				{
				public:
					virtual ~TESObject() = default;

					// override (TESForm)
					bool IsObject() const noexcept(true) override { return true; }
					std::uint32_t GetRefCount() const noexcept(true) override { return 0; }

					// add
					virtual void Unk_66(void) noexcept(true);												// 66 - { return 0; }
					virtual bool IsBoundAnimObject() noexcept(true);										// 67 - { return false; }
					[[nodiscard]] virtual TESWaterForm* GetWaterType() const noexcept(true);				// 68 - { return 0; }
					[[nodiscard]] virtual bool IsAutoCalc() const noexcept(true);							// 69 - { return false; }
					virtual void SetAutoCalc(bool a_autoCalc) noexcept(true);								// 6A - { return; }
					virtual NiAPI::NiAVObject* Clone3D(TESObjectREFR* a_ref, bool a_arg3) noexcept(true);	// 6B - { return 0; }
					virtual void UnClone3D(TESObjectREFR* a_ref) noexcept(true);							// 6C
					virtual bool IsMarker() noexcept(true);													// 6D
					virtual bool IsOcclusionMarker() noexcept(true);										// 6E - { return formType == FormType::Static && this == Plane/Room/PortalMarker; }
					virtual bool ReplaceModel() noexcept(true);												// 6F
					virtual std::uint32_t IncRef() noexcept(true);											// 70 - { return 0; }
					virtual std::uint32_t DecRef() noexcept(true);											// 71 - { return 0; }
					virtual NiAPI::NiAVObject* LoadGraphics(TESObjectREFR* a_ref) noexcept(true);			// 72
				};
				static_assert(sizeof(TESObject) == 0x28);
			}
		}
	}
}