// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "LoadDDSFile.h"
#include "Editor API/SSE/BSResources.h"

#include <comdef.h>

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_LoadDDSFile_sub = 0;

			LoadDDSFilePatch::LoadDDSFilePatch() : Module(GlobalEnginePtr)
			{}

			bool LoadDDSFilePatch::HasOption() const
			{
				return false;
			}

			bool LoadDDSFilePatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* LoadDDSFilePatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* LoadDDSFilePatch::GetName() const
			{
				return "Load .DDS File";
			}

			bool LoadDDSFilePatch::HasDependencies() const
			{
				return false;
			}

			Array<String> LoadDDSFilePatch::GetDependencies() const
			{
				return {};
			}

			bool LoadDDSFilePatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool LoadDDSFilePatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// PS: for 1.6.1130 added check, but no assert message.

					//
					// Replace direct crash with an assertion when an incompatible texture format is used in the renderer
					//
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);

					pointer_LoadDDSFile_sub = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));

					return true;
				}

				return false;
			}

			bool LoadDDSFilePatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			HRESULT LoadDDSFilePatch::TryCheckArgs(__int64 a2)
			{
				auto Resource = ((EditorAPI::SkyrimSpectialEdition::BSResourceNiBinaryStream*)a2);

				__try
				{
					if (!Resource->GetStream())
					{
						_CONSOLE("ERROR: DirectX::LoadFromDDSFile empty stream.");
						return E_FAIL;
					}

					auto FileName = Resource->GetStream()->GetFileName();
					if (!FileName.data)
					{
						_CONSOLE("ERROR: DirectX::LoadFromDDSFile empty filename.");
						return E_FAIL;
					}

					FileName.data[0] = FileName.data[0];
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					_CONSOLE("ERROR: DirectX::LoadFromDDSFile fatal error has occurred (invalid pointer Filename or NiStream).");
					return E_FAIL;
				}

				return S_OK;
			}

			HRESULT LoadDDSFilePatch::sub(__int64 a1, __int64 a2, __int64 a3, __int64 a4, unsigned int a5, int a6)
			{
				if (!a1 || !a2)
					return E_FAIL;

				auto Resource = ((EditorAPI::SkyrimSpectialEdition::BSResourceNiBinaryStream*)a2);
				AssertMsg(SUCCEEDED(TryCheckArgs(a2)), "Fatal load unknown texture");

				if (!Resource->GetStream()->GetFileName().data[0])
					// Skips empty filename
					return E_FAIL;

				// Modified DirectX::LoadFromDDSFile from DDSTextureLoader (DirectXTex)
				HRESULT hr = ((HRESULT(__fastcall*)(__int64, __int64, __int64, __int64, unsigned int, int))pointer_LoadDDSFile_sub)
					(a1, a2, a3, a4, a5, a6);
				if (FAILED(hr))
				{
					_CONSOLE("ERROR: DirectX::LoadFromDDSFile returned failed (0x%08X) \"%s\" \"%s\".", 
						hr, _com_error(hr).ErrorMessage(), Resource->GetStream()->GetFileName().data);

					AssertMsgVa((hr == E_FAIL),
						"Fatal error while trying to load texture \"%s\" due to an incompatible file format. This "
						"indicates a problem with your mod or game files. Note that B5G6R5 and B5G5R5A1 texture "
						"formats are not supported on Windows 7. HR = 0x%08X.",
						Resource->GetStream()->GetFileName().data, hr);

					return hr;
				}

				//_CONSOLE("DirectX::LoadFromDDSFile opened file \"%s\".", Resource->GetStream()->GetFileName().data);

				// This return value is ignored. If it fails it returns a null pointer (a3) and crashes later on.
				return S_OK;
			}
		}
	}
}