// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.RTTI.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixAcousticSpaceMarker.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			FixAcousticSpaceMarker::FixAcousticSpaceMarker() : Common::Patch()
			{
				SetName("Fix Acoustic Space Marker");
			}

			bool FixAcousticSpaceMarker::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixAcousticSpaceMarker::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixAcousticSpaceMarker::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixAcousticSpaceMarker::GetDependencies() const noexcept(true)
			{
				return {};
			}

			TEnum<Common::Patch::Method> FixAcousticSpaceMarker::GetMethods() const noexcept(true)
			{
				return Common::Patch::Method::kNoUseAnything;
			}

			bool FixAcousticSpaceMarker::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixAcousticSpaceMarker::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				//
				// TESForm::IsEditorMarkerForm (virtual, vtable slot 108 in every supported CK) decides which
				// references are hidden/shown with the render window "Markers" toggle. It knows the form types
				// SOUN, IDLM, ACTI/LIGH markers, ... but not ASPC, so acoustic space references are not hidden with the
				// other markers, and are not restored consistently after toggling them.
				//
				constexpr std::size_t slot = 108;

				auto rtti = Common::RTTI::GetSingleton()->Find("class BGSAcousticSpace");
				if (!rtti || (rtti->VFunctionCount <= slot))
					return false;

				ScopeSafeWrite vtable(rtti->VTableAddress, rtti->VFunctionCount * 8);

				auto Class = reinterpret_cast<std::uintptr_t*>(rtti->VTableAddress);
				Class[slot] = reinterpret_cast<std::uintptr_t>(&sub);

				return true;
			}

			bool FixAcousticSpaceMarker::sub([[maybe_unused]] void* form) noexcept(true)
			{
				return true;
			}
		}
	}
}
