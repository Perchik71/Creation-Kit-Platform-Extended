// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Asserts.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixNiSkinInstance.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			using TFixNiSkinInstanceSub = void(std::int64_t, std::int64_t);

			static std::function<TFixNiSkinInstanceSub> FixNiSkinInstanceSub;

			FixNiSkinInstance::FixNiSkinInstance() : Common::Patch()
			{
				SetName("NiSkinInstance Fix");
			}

			bool FixNiSkinInstance::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixNiSkinInstance::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixNiSkinInstance::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixNiSkinInstance::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixNiSkinInstance::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixNiSkinInstance::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixNiSkinInstance::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				Relocation(ID(599092)).WriteVFunc(25, &NiSkinInstance_LinkObject);
				Relocation(ID(40269)).WriteVFunc(25, &NiSkinInstance_LinkObject);
				FixNiSkinInstanceSub = Relocation<TFixNiSkinInstanceSub>(ID(657313)).Get();

				return true;
			}

			void FixNiSkinInstance::NiSkinInstance_LinkObject(std::int64_t SkinInstance, std::int64_t Stream) noexcept(true)
			{
				FixNiSkinInstanceSub(SkinInstance, Stream);

				// SkinInstance->RootParent can't be null
				std::int64_t rootParent = *(std::int64_t*)(SkinInstance + 0x20);
				auto nifPath = (const char*)(Stream + 0x108);

				CKPE_ASSERT_MSG_FMT(rootParent, "A mesh's NiSkinInstance is missing a skeleton root node. This is a fatal error. NIF path is \"%s\".", nifPath);
			}
		}
	}
}