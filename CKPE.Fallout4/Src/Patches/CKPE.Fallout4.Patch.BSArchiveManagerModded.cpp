// Copyright © 2024-2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Utils.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/TESFile.h>
#include <EditorAPI/BSResourceArchive2.h>
#include <Patches/CKPE.Fallout4.Patch.BSArchiveManagerModded.h>
#include <xbyak.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace BSResource
			{
				extern bool NoTextureLoad;
				extern std::uintptr_t pointer_Archive2_sub1;
				extern std::uintptr_t pointer_Archive2_sub2;
				extern std::uintptr_t pointer_Archive2_sub3;
			}
		}

		namespace Patch
		{
			std::vector<const EditorAPI::TESFile*> g_SelectedFilesArray;
			std::uintptr_t pointer_BSArchiveManagerModded_sub = 0;
			bool IsLoaded;
			std::uint8_t supportedBA2Version = 8;

			static void AttachBA2File(const char* _filename) noexcept(true)
			{
				if (EditorAPI::BSResource::Archive2::IsAvailableForLoad(_filename))
					goto attach_ba2;
				return;
			attach_ba2:
				EditorAPI::BSResource::Archive2::LoadArchive(_filename);
			}

			static void LoadTesFile(const EditorAPI::TESFile* load_file) noexcept(true)
			{
				IsLoaded = false;
				// Sometimes duplicated
				if (std::find(g_SelectedFilesArray.begin(), g_SelectedFilesArray.end(), load_file) ==
					g_SelectedFilesArray.end())
				{
					if (load_file->IsActive())
						_CONSOLE("Load active file %s...", load_file->GetFileName().c_str());
					else if (load_file->IsMaster() || load_file->IsSmallMaster())
						_CONSOLE("Load master file %s...", load_file->GetFileName().c_str());
					else
						_CONSOLE("Load file %s...", load_file->GetFileName().c_str());

					g_SelectedFilesArray.push_back(load_file);
				}

				auto sname = load_file->GetFileName();
				sname.Copy(0, sname.FindLastOf('.'));

				AttachBA2File(*(sname + " - Main.ba2"));
				if (!EditorAPI::BSResource::NoTextureLoad) AttachBA2File(*(sname + " - Textures.ba2"));

				fast_call<void>(pointer_BSArchiveManagerModded_sub, load_file);
			}

			static void LoadTesFileFinal(HWND hWnd, UINT uMsg, LPARAM lParam, WPARAM wParam) noexcept(true)
			{
				Common::EditorUI::Hook::HKSendMessageA(hWnd, uMsg, lParam, wParam);
				g_SelectedFilesArray.clear();
				IsLoaded = true;
			}

			static bool HasLoaded() noexcept(true)
			{
				return IsLoaded;
			}

			BSArchiveManagerModded::BSArchiveManagerModded() : Common::Patch()
			{
				SetName("BSArchiveManager Modded");
			}

			bool BSArchiveManagerModded::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* BSArchiveManagerModded::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool BSArchiveManagerModded::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> BSArchiveManagerModded::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool BSArchiveManagerModded::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool BSArchiveManagerModded::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				EditorAPI::BSResource::pointer_Archive2_sub1 = ID{ 399667, 1624099 }.Address();
				EditorAPI::BSResource::pointer_Archive2_sub2 = ID{ 20873, 1940294 }.Address();
				EditorAPI::BSResource::pointer_Archive2_sub3 = Relocation(ID(44564)).WriteJump
					(&EditorAPI::BSResource::Archive2::HKLoadArchive);
				EditorAPI::BSResource::Archive2::Initialize(); 

				const auto target = ID(44564);
				const auto rel1 = Relocation(target, Offset{ 0x557, 0x4C3, 0x4D9, 0x434 });

				if (VersionLists::GetEditorVersion() == VersionLists::EDITOR_FALLOUT_C4_1_10_162_0)
					rel1.WriteCall(&EditorAPI::BSResource::Archive2::HKLoadStreamArchive);
				else if (VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_1_11_137_0)
					rel1.WriteCall(&EditorAPI::BSResource::Archive2::HKLoadStreamArchiveEx);
				else
				{
					class HookLoadA : public Xbyak::CodeGenerator
					{
					public:
						HookLoadA() : Xbyak::CodeGenerator()
						{
							sub(rsp, 0x38);
							mov(ptr[rsp + 0x20], rdi);
							mov(rax, (uintptr_t)&EditorAPI::BSResource::Archive2::HKLoadStreamArchiveEx2);
							call(rax);
							add(rsp, 0x38);
							ret();
						}
					};

					auto hook = new HookLoadA();
					rel1.WriteCall((uintptr_t)hook->getCode());
				}

				pointer_BSArchiveManagerModded_sub = ID{ 289312, 1493949 }.Address();
				Relocation(ID{ 261467, 1943828 }, Offset{ 0x17, 0x1C }).WriteCall(&LoadTesFile);
				Relocation(ID(192751)).WriteJump(&LoadTesFileFinal);

				// Так как разница между первой и 8 версией лишь, то что был удалён GNF формат для PlayStation.
				// То очевидно, 8 версии с GNF форматом просто не будет, то вполне безопасно, открывать любые версии архивы.
				if (VersionLists::GetEditorVersion() == VersionLists::EDITOR_FALLOUT_C4_1_10_162_0)
					// Первая версия патча для 1.10.162.0
					Relocation(target, 0x45E).Write(&supportedBA2Version, 1);

				return true;
			}
		}
	}
}