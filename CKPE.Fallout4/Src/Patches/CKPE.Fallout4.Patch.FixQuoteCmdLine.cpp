// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.StringUtils.h>
#include <CKPE.PathUtils.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.ResponseWindow.h>
#include <Patches/CKPE.Fallout4.Patch.FixQuoteCmdLine.h>

#include <CKPE.MessageBox.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			FixQuoteCmdLine::FixQuoteCmdLine() : Common::Patch()
			{
				SetName("Fixed quote to cmdline");
			}

			bool FixQuoteCmdLine::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixQuoteCmdLine::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixQuoteCmdLine::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> FixQuoteCmdLine::GetDependencies() const noexcept(true)
			{
				return { "Response Window" };
			}

			bool FixQuoteCmdLine::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixQuoteCmdLine::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//	Add support quote to command line with -GeneratePreCombined
				//	Should be: -GeneratePreCombined:"<ESMFilename>" [clean, filtered] [all, other, main, ints]

				Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&sub);
				Detours::DetourCall(__CKPE_OFFSET(1), (std::uintptr_t)&sub);
				Detours::DetourCall(__CKPE_OFFSET(2), (std::uintptr_t)&sub);

				//	-GeneratePreVisData
				//	This command into the code section -GeneratePreVisData.
				//	Should be: -GeneratePreVisData:"<ESMFilename>" [clean, filtered] [all, other, main, ints]

				Detours::DetourCall(__CKPE_OFFSET(3), (std::uintptr_t)&sub);
				Detours::DetourCall(__CKPE_OFFSET(4), (std::uintptr_t)&sub);

				//	Add support quote to command line with -CheckInPlugin
				//	Should be: -CheckInPlugin:"<PluginFilename>" "<ESMFilename>"

				Detours::DetourCall(__CKPE_OFFSET(5), (std::uintptr_t)&sub);
				Detours::DetourCall(__CKPE_OFFSET(6), (std::uintptr_t)&sub);

				//	Add support quote to command line with -ConvertToESL
				//	Should be: -ConvertToESL:"<PluginFilename>"

				Detours::DetourCall(__CKPE_OFFSET(7), (std::uintptr_t)&sub);

				//	Add support quote to command line with -DumpNeededFiles
				//	Should be: -DumpNeededFiles:"<ESMFilename>" "<OutputFilepath>"

				Detours::DetourCall(__CKPE_OFFSET(8), (std::uintptr_t)&sub);
				Detours::DetourCall(__CKPE_OFFSET(9), (std::uintptr_t)&sub);

				//	Add support quote to command line with -SaveDefaultPlugin
				//	Should be: -SaveDefaultPlugin:"<PluginFilename>" "<ESMFilename>"

				Detours::DetourCall(__CKPE_OFFSET(10), (std::uintptr_t)&sub);
				Detours::DetourCall(__CKPE_OFFSET(11), (std::uintptr_t)&sub);

				//	Add support quote to command line with -SaveDefaultPlugin
				//	Should be: -ExportDismemberData:"<ESMFilename>" <XB1|X64|PS4|W32>

				Detours::DetourCall(__CKPE_OFFSET(12), (std::uintptr_t)&sub);
				Detours::DetourCall(__CKPE_OFFSET(13), (std::uintptr_t)&sub);

				//	Add support quote to command line with -UpdateModelData
				//	Should be: -UpdateModelData:"<ESMFilename>"

				Detours::DetourCall(__CKPE_OFFSET(14), (std::uintptr_t)&sub);

				//	Add support quote to command line with -OutputAreaArt
				//	Should be: -OutputAreaArt:"<ESMFilename>" "<AreasFilename>" "<OutputFilename>"

				Detours::DetourCall(__CKPE_OFFSET(15), (std::uintptr_t)&sub);
				Detours::DetourCall(__CKPE_OFFSET(16), (std::uintptr_t)&sub);
				Detours::DetourCall(__CKPE_OFFSET(17), (std::uintptr_t)&sub);

				//	Add support quote to command line with -CompileTextExport
				//	Should be: -CompileTextExport:"<ESMFilename>" "<language>" "<PathToTextExport>" ["<PathBackupToTextExport>"]

				Detours::DetourCall(__CKPE_OFFSET(18), (std::uintptr_t)&sub);
				Detours::DetourCall(__CKPE_OFFSET(19), (std::uintptr_t)&sub);

				//	Add support quote to command line with -ExportFaceGenData
				//	Should be: -ExportFaceGenData:"<ESMFilename>" <XB1|X64|PS4|W32>

				Detours::DetourCall(__CKPE_OFFSET(20), (std::uintptr_t)&sub);
				Detours::DetourCall(__CKPE_OFFSET(21), (std::uintptr_t)&sub);

				//	Add support quote to command line with -GenerateAnimInfo
				//	Should be: -GenerateAnimInfo:"<ESMFilename>" "<DataFilepath>" "<OutputFilepath>" [%s] [%s] [%s]

				Detours::DetourCall(__CKPE_OFFSET(22), (std::uintptr_t)&sub);
				Detours::DetourCall(__CKPE_OFFSET(23), (std::uintptr_t)&sub);
				Detours::DetourCall(__CKPE_OFFSET(24), (std::uintptr_t)&sub);
				Detours::DetourCall(__CKPE_OFFSET(25), (std::uintptr_t)&sub);
				Detours::DetourCall(__CKPE_OFFSET(26), (std::uintptr_t)&sub);

				//	Add support quote to command line with -GenerateSingleLip
				//	Should be: -GenerateSingleLip:"<WavFilename>" "<Text>"
				//	Warning: The .lip file will be created next to the audio file.

				Detours::DetourCall(__CKPE_OFFSET(27), (std::uintptr_t)&sub2);

				//	Add support quote to command line with -GenerateStaticCollections
				//	Should be: -GenerateStaticCollections:"<ESMFilename>" <XB1|X64|PS4|W32>

				Detours::DetourCall(__CKPE_OFFSET(28), (std::uintptr_t)&sub);
				Detours::DetourCall(__CKPE_OFFSET(29), (std::uintptr_t)&sub);

				//	Add support quote to command line with -DepersistRefs
				//	Should be: -DepersistRefs:"<ESMFilename>"

				Detours::DetourCall(__CKPE_OFFSET(30), (std::uintptr_t)&sub);

				//	Add support quote to command line with -MapMarker
				//	Should be: -MapMarker:"<ESMFilename>" "<Worldspace|Interior>"

				Detours::DetourCall(__CKPE_OFFSET(31), (std::uintptr_t)&sub);
				Detours::DetourCall(__CKPE_OFFSET(32), (std::uintptr_t)&sub);

				//	Add support quote to command line with -MapInfo
				//	Should be: -MapInfo:"<ESMFilename>" "<Worldspace|Interior>" ["<%s>"]

				Detours::DetourCall(__CKPE_OFFSET(33), (std::uintptr_t)&sub);
				Detours::DetourCall(__CKPE_OFFSET(34), (std::uintptr_t)&sub);
				Detours::DetourCall(__CKPE_OFFSET(35), (std::uintptr_t)&sub);

				//	Add support quote to command line with -ImportScalingData
				//	Should be: -ImportScalingData:"<ESMFilename>"

				Detours::DetourCall(__CKPE_OFFSET(36), (std::uintptr_t)&sub);

				//	Add support quote to command line with -CompressPSG
				//	Should be: -CompressPSG:"<ESMFilename>"

				Detours::DetourCall(__CKPE_OFFSET(37), (std::uintptr_t)&sub);

				//	Add support quote to command line with -BuildCDX
				//	Should be: -BuildCDX:"<ESMFilename>"

				Detours::DetourCall(__CKPE_OFFSET(38), (std::uintptr_t)&sub);

				return true;
			}

			char* FixQuoteCmdLine::sub(char* str, const char* delim, char** next_token) noexcept(true)
			{
				if (str) {
					while (*str == ' ') {
						if (*str == '\0')
							return nullptr;
						str++;
					}
					if (*str == '\"')
						return strtok_s(++str, "\"", next_token);
					else
						return strtok_s(str, " ", next_token);
				}
				else if (next_token && *next_token) {
					if (strchr(*next_token, '\"')) {
						char* lpRes = strtok_s(nullptr, "\"", next_token);

						if (lpRes && !StringUtils::Trim(lpRes).length())
							lpRes = strtok_s(nullptr, "\"", next_token);

						return lpRes;
					}
					else
						return strtok_s(nullptr, " ", next_token);
				}
				else
					return strtok_s(str, delim, next_token);
			}

			void FixQuoteCmdLine::sub2(char* lpCmdLine, char* arg2) noexcept(true)
			{
				char* next_token = nullptr;
				char* token = sub(lpCmdLine, " ", &next_token);
				if (token)
				{
					char* filename = token;
					token = sub(nullptr, " ", &next_token);
					if (token)
					{
						auto pathAudioFile = PathUtils::ChangeFileExt(filename, ".wav");
						auto pathLipFile = PathUtils::ChangeFileExt(filename, ".lip");
						ResponseWindow::GenerationLip(pathAudioFile.c_str(), pathLipFile.c_str(), token);
						Common::Interface::GetSingleton()->GetApplication()->Terminate();
					}
				}

				MessageBox::OpenError("Error parsing command-line. Format should be"
					" -GenerateSingleLip:\"WAVFilePath\" \"Text For WAV File.\"");
				Common::Interface::GetSingleton()->GetApplication()->Terminate();
			}
		}
	}
}