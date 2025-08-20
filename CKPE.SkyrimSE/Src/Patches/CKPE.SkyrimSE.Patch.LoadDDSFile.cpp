// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.Asserts.h>
#include <CKPE.ErrorHandler.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.RTTI.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/BSResources.h>
#include <Patches/CKPE.SkyrimSE.Patch.LoadDDSFile.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			std::uintptr_t pointer_LoadDDSFile_sub = 0;

			LoadDDSFile::LoadDDSFile() : Common::Patch()
			{
				SetName("Load .DDS File");
			}

			bool LoadDDSFile::HasOption() const noexcept(true)
			{
				return true;
			}

			const char* LoadDDSFile::GetOptionName() const noexcept(true)
			{
				return "CreationKit:bWarningCreateTexture2D";
			}

			bool LoadDDSFile::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> LoadDDSFile::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool LoadDDSFile::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool LoadDDSFile::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto _interface = CKPE::Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				// PS: for 1.6.1130 added check, but no assert message.

				//
				// Replace direct crash with an assertion when an incompatible texture format is used in the renderer
				//
				Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&sub);
				pointer_LoadDDSFile_sub = __CKPE_OFFSET(1);

				return true;
			}

			std::uint64_t LoadDDSFile::sub(std::int64_t a1, std::int64_t a2, std::int64_t a3, std::int64_t a4,
				std::uint32_t a5, std::int32_t a6) noexcept(true)
			{
				// Modified DirectX::LoadFromDDSFile from DDSTextureLoader (DirectXTex)
				HRESULT hr = ((HRESULT(__fastcall*)(std::int64_t, std::int64_t, std::int64_t, std::int64_t,
					std::uint32_t, std::int32_t))pointer_LoadDDSFile_sub)
					(a1, a2, a3, a4, a5, a6);

				if (FAILED(hr))
				{
					EditorAPI::BSFixedString FileName;
					auto Resource = ((EditorAPI::BSResourceNiBinaryStream*)a2);
					if (!Resource || !Resource->GetStream() || !Resource->GetStream()->GetFileName(FileName))
						CKPE_ASSERT_MSG(false, "Fatal error while trying to load unknown texture.");

					CKPE_ASSERT_MSG_FMT(false,
						"Fatal error while trying to load texture \"%s\" due to an incompatible file format. This "
						"indicates a problem with your mod or game files. Note that B5G6R5 and B5G5R5A1 texture "
						"formats are not supported on Windows 7. HR = (0x%08X) %s.",
						FileName.c_str(), hr, ErrorHandler::GetSystemMessage(hr).c_str());
				}

				// This return value is ignored. If it fails it returns a null pointer (a3) and crashes later on.
				return hr;
			}
		}
	}
}