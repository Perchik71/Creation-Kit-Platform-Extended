// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <xbyak.h>

#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.FileUtils.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Starfield.VersionLists.h>
#include <EditorAPI/BSResources.h>
#include <EditorAPI/BSString.h>
#include <Patches/CKPE.Starfield.Patch.LooseFileStream.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
#if 0
			static void BSResource_Initialize64BitData(EditorAPI::BSResource::LooseFileStream* Stream, 
				const char* FileName) noexcept(true)
			{
				Stream->dw64FileSize = 0;
				Stream->dw64Position = 0;

				auto path = EditorAPI::BSString::Utils::GetApplicationPath() + FileName;
				if (EditorAPI::BSString::Utils::FileExists(path))
					Stream->dw64FileSize = FileUtils::GetFileSize(*path);
				else
					Stream->dw64FileSize = Stream->GetTotalSize();
			}

			static void BSResource_CreateInstance(const char* FileName, 
				EditorAPI::BSResource::LooseFileStream** Instance) noexcept(true)
			{
				if (!FileName || !Instance || !(*Instance))
					return;

				auto stream = *Instance;
				BSResource_Initialize64BitData(stream, FileName);
			}
#endif

			LooseFileStream::LooseFileStream() : Common::Patch()
			{
				SetName("BSResource Loose Files");
			}

			bool LooseFileStream::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* LooseFileStream::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool LooseFileStream::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> LooseFileStream::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool LooseFileStream::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() >= VersionLists::EDITOR_STARFIELD_1_14_74_0;
			}

			bool LooseFileStream::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto verPatch = db->GetVersion();

				if ((verPatch != 1) && (verPatch != 2))
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// Cutting a lot is faster this way
				auto stext = interface->GetApplication()->GetSegment(Segment::text);
				ScopeSafeWrite text(stext.GetAddress(), stext.GetSize());

#if 0
				// I will add initialization to fill in the new fields with data
				class LooseFileStreamHook : public Xbyak::CodeGenerator
				{
				public:
					LooseFileStreamHook() : Xbyak::CodeGenerator()
					{
						push(rdi);
						mov(rcx, rdi);
						mov(rdx, r13);
						sub(rsp, 0x40);
						mov(rax, (std::uintptr_t)&BSResource_CreateInstance);
						call(rax);
						add(rsp, 0x40);
						pop(rdi);
						ret();
					}

					static void Generate(std::uintptr_t Target)
					{
						auto hook = new LooseFileStreamHook();
						Detours::DetourJump(Target, (std::uintptr_t)hook->getCode());
					}
				};

				// Set new size class 0x168 (0x170 for v1.15) to 0x180
				for (std::uint32_t i = 0; i < 3; i++)
					text.Write(__CKPE_OFFSET(i), { 0x80 });

				// Init
				LooseFileStreamHook::Generate(__CKPE_OFFSET(3));
#endif
				// Ignoring the correctness check is not useful
				text.Write(__CKPE_OFFSET(4), { 0xEB });
				Detours::DetourJump(__CKPE_OFFSET(5), (uintptr_t)&sub);

				if (verPatch == 1)
					text.WriteNop(__CKPE_OFFSET(6), 0xC);
				else if (verPatch == 2)
					text.WriteNop(__CKPE_OFFSET(6), 0xA);

				return true;
			}

			bool LooseFileStream::sub(const char* fileName, uint64_t& fileSize) noexcept(true)
			{
				fileSize = FileUtils::GetFileSize(fileName);
				return fileSize > 0;
			}
		}
	}
}