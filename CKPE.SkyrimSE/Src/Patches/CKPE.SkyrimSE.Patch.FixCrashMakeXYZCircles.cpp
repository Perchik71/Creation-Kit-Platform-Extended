// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixCrashMakeXYZCircles.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			FixCrashMakeXYZCircles::FixCrashMakeXYZCircles() : Common::Patch()
			{
				SetName("Fix Crash MakeXYZCircles");
			}

			bool FixCrashMakeXYZCircles::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixCrashMakeXYZCircles::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixCrashMakeXYZCircles::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixCrashMakeXYZCircles::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixCrashMakeXYZCircles::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixCrashMakeXYZCircles::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixCrashMakeXYZCircles::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fix for crash when MakeXYZCircles passes an invalid line count to BSShaderResourceManager::CreateLineShape. 
				// BGSPrimitiveSphere only creates these debug marker circles in the editor.
				//
				Relocation(ID{ 760688, 1019708 }, Offset{ 0x3C0, 0x357 }).WriteCall(&sub);

				return true;
			}

			void* FixCrashMakeXYZCircles::sub(std::int64_t ResourceManager, std::uint32_t IndexCount, std::int64_t a3, 
				std::int64_t a4, std::int64_t a5, std::int64_t a6, std::int64_t a7) noexcept(true)
			{
				// Convert # of indices to # of lines (divide by two)
				return (*(void*(__fastcall**)(std::int64_t, std::int64_t, std::int64_t, std::int64_t, 
					std::int64_t, std::int64_t, std::int64_t))
					(*(std::int64_t*)ResourceManager + 152))(ResourceManager, IndexCount / 2, a3, a4, a5, a6, a7);
			}
		}
	}
}