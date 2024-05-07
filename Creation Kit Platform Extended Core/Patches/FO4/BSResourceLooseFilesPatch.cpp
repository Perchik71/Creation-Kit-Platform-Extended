// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "BSResourceLooseFilesPatch.h"
#include "Editor API/FO4/BSResourceLooseFilesF4.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			using namespace EditorAPI::Fallout4;

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
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool BSResourceLooseFilesPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				// I will add initialization to fill in the new fields with data
				class LooseFileStreamHook : public Xbyak::CodeGenerator
				{
				public:
					LooseFileStreamHook(VOID) : Xbyak::CodeGenerator()
					{
						push(r11);
						push(rax);
						mov(rcx, ptr[r11 + 0x18]);
						mov(rdx, rbp);
						sub(rsp, 0x40);
						mov(rax, (uintptr_t)&BSResource::LooseFileStream::CreateInstance);
						call(rax);
						add(rsp, 0x40);
						pop(rax);
						pop(r11);
						mov(rbx, ptr[r11 + 0x10]);
						mov(rbp, ptr[r11 + 0x18]);
						mov(rsi, ptr[r11 + 0x20]);
						mov(rsp, r11);
						pop(rdi);
						ret();
					}

					static VOID Generate(uintptr_t Target)
					{
						auto hook = new LooseFileStreamHook();
						GlobalRelocatorPtr->DetourJump(Target, (uintptr_t)hook->getCode());
					}
				};

				auto verPatch = lpRelocationDatabaseItem->Version();
				if (verPatch == 1)
				{
					// Set new size class 0x160 to 0x180
					lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0x80 });

					LooseFileStreamHook::Generate(lpRelocationDatabaseItem->At(1));
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(8), (uintptr_t)&sub);

					ScopeRelocator text;

					// As I understand it, CK evaluates the amount of useful data with the file size, 
					// since I will now adjust the 64-bit size, sacrificing checking for NullPtr.
					//
					// mov rax, qword ptr ds:[rcx+0x170]
					// jmp -> cmp rsi, rax
					lpRelocator->Patch(lpRelocationDatabaseItem->At(2), 
						{ 0x48, 0x8B, 0x81, 0x70, 0x01, 0x00, 0x00, 0xEB, 0x1D });

					// Ignoring the correctness check is not useful
					lpRelocator->Patch(lpRelocationDatabaseItem->At(3), { 0xEB });
					lpRelocator->Patch(lpRelocationDatabaseItem->At(4), { 0xEB });
					lpRelocator->Patch(lpRelocationDatabaseItem->At(5), { 0xEB });
					lpRelocator->Patch(lpRelocationDatabaseItem->At(6), { 0xC3 });
					lpRelocator->Patch(lpRelocationDatabaseItem->At(7), { 0xC3 });

					return true;
				}
				else if (verPatch == 2)
				{
					{
						ScopeRelocator text;

						// Ignoring the correctness check is not useful

						for (uint32_t i = 0; i < 4; i++)
							lpRelocator->Patch(lpRelocationDatabaseItem->At(i), { 0xEB });

						// As I understand it, CK evaluates the amount of useful data with the file size, 
						// since I will now adjust the 64-bit size, sacrificing checking for NullPtr.
						//
						// mov rax, qword ptr ds:[rcx+0x170]
						// jmp -> cmp rbx, rax
						lpRelocator->Patch(lpRelocationDatabaseItem->At(5),
							{ 0x48, 0x8B, 0x81, 0x70, 0x01, 0x00, 0x00, 0xEB, 0x19 });

						// Set new size class 0x168 to 0x180
						lpRelocator->Patch(lpRelocationDatabaseItem->At(6), { 0x80 });
					}

					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(4), (uintptr_t)&sub);
					LooseFileStreamHook::Generate(lpRelocationDatabaseItem->At(7));
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