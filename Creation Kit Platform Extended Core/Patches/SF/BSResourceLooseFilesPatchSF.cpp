// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "BSResourceLooseFilesPatchSF.h"
#include "Editor API/SF/BSResourceLooseFilesSF.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Starfield
		{
			using namespace EditorAPI::Starfield;

			BSResourceLooseFilesPatch::BSResourceLooseFilesPatch() : Module(GlobalEnginePtr)
			{}

			bool BSResourceLooseFilesPatch::HasOption() const
			{
				return false;
			}

			bool BSResourceLooseFilesPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* BSResourceLooseFilesPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* BSResourceLooseFilesPatch::GetName() const
			{
				return "BSResource Loose Files";
			}

			bool BSResourceLooseFilesPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> BSResourceLooseFilesPatch::GetDependencies() const
			{
				return {};
			}

			bool BSResourceLooseFilesPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return (eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_1_14_74_0) &&
					(eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_LAST);
			}

			bool BSResourceLooseFilesPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				auto verPatch = lpRelocationDatabaseItem->Version();
				if (verPatch == 1)
				{
					// I will add initialization to fill in the new fields with data
					/*class LooseFileStreamHook : public Xbyak::CodeGenerator
					{
					public:
						LooseFileStreamHook(VOID) : Xbyak::CodeGenerator()
						{
							push(rdi);
							mov(rcx, rdi);
							mov(rdx, r13);
							sub(rsp, 0x40);
							mov(rax, (uintptr_t)&BSResource::LooseFileStream::CreateInstance);
							call(rax);
							add(rsp, 0x40);
							pop(rdi);
							ret();
						}

						static VOID Generate(uintptr_t Target)
						{
							auto hook = new LooseFileStreamHook();
							GlobalRelocatorPtr->DetourJump(Target, (uintptr_t)hook->getCode());
						}
					};*/

					ScopeRelocator text;

					// Set new size class 0x168 to 0x178
					for (size_t i = 0; i < 3; i++)
						lpRelocator->Patch(_RELDATA_RAV(i), { 0x78 });
					// Init
					//LooseFileStreamHook::Generate(_RELDATA_RAV(3));
					// Ignoring the correctness check is not useful
					lpRelocator->Patch(_RELDATA_RAV(4), { 0xEB });
					lpRelocator->DetourJump(_RELDATA_RAV(5), (uintptr_t)&sub);
					lpRelocator->PatchNop(_RELDATA_RAV(6), 0xC);

					return true;
				}

				return false;
			}

			bool BSResourceLooseFilesPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			bool BSResourceLooseFilesPatch::sub(const char* fileName, uint64_t& fileSize)
			{
				WIN32_FILE_ATTRIBUTE_DATA fileData = { 0 };

				if (GetFileAttributesExA(fileName, GetFileExInfoStandard, &fileData)) {
					if ((fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY ||
						fileData.dwFileAttributes == INVALID_FILE_ATTRIBUTES)
						return false;

					fileSize = fileData.nFileSizeLow | ((uint64_t)fileData.nFileSizeHigh << 32);
					return fileSize > 0;
				}

				return false;
			}
		}
	}
}