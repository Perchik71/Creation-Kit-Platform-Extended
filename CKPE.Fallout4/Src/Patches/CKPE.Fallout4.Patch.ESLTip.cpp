// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <EditorAPI/IDs.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.AddressLibrary.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/T/TESDataHandler.h>
#include <Patches/CKPE.Fallout4.Patch.ESLTip.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			constexpr const char* sNotEslCapable = " <NOT ESL Capable - no more IDs remaining>";
			constexpr const char* sEslCapable = " <ESL Capable - %d IDs remaining>";

			struct UnknownDataLoaded
			{
				EditorAPI::Forms::TESForm** arrayForms;
				uint64_t unk;
				uint32_t total;
			};

			ESLTip::ESLTip() : Common::Patch()
			{
				SetName("ESL Tip");
			}

			bool ESLTip::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* ESLTip::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool ESLTip::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> ESLTip::GetDependencies() const noexcept(true)
			{
				return { "Console" };
			}

			bool ESLTip::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() < VersionLists::EDITOR_FALLOUT_C4_1_11_137_0;
			}

			bool ESLTip::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				Relocation(EditorAPI::IDs::Functions::MainWindow_UpdateTitle, Offset{ 0xE1 }).WriteCall(&sub);

				return true;
			}

			void ESLTip::sub(char* dst, std::size_t max_size, char* src) noexcept(true)
			{
				strcat_s(dst, max_size, src);

				// Checking for the current active plugin
				auto singleton = EditorAPI::TESDataHandler::GetSingleton();
				if (!singleton) return;

				Common::Relocation<UnknownDataLoaded**> dataLoaded(Common::ID(440461));
				auto data = *dataLoaded;
				 
				auto active_plugin = singleton->ActiveMod;
				if (!active_plugin) return;

				if (!data || !data->total || !data->arrayForms)
					strcat_s(dst, max_size, sNotEslCapable);
				else
				{
					uint32_t total = 0;
					for (size_t i = 0; i < data->total; i++)
					{
						const auto form = data->arrayForms[i];
						if (form->GetFile(0) == active_plugin)
							total++;
					}

					auto avail = 0xFFF - total;
					if (avail >= 0xFFF)
						strcat_s(dst, max_size, sNotEslCapable);
					else
					{
						char szBuf[64] = { 0 };
						sprintf_s(szBuf, sEslCapable, avail);
						strcat_s(dst, max_size, szBuf);
					}
				}
			}
		}
	}
}