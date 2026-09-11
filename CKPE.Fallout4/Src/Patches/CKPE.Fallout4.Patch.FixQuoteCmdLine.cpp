// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.StringUtils.h>
#include <CKPE.PathUtils.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.Relocation.h>
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
				using namespace Common;

				const auto target = ID{ 632480, 1993061 };

				//	Add support quote to command line with -GeneratePreCombined
				//	Should be: -GeneratePreCombined:"<ESMFilename>" [clean, filtered] [all, other, main, ints]

				Relocation(target, Offset{ 0xE33, 0x1085, 0x1085, 0x10C8 }).WriteCall(&sub);
				Relocation(target, Offset{ 0xE53, 0x10A3, 0x10A3, 0x10E6 }).WriteCall(&sub);
				Relocation(target, Offset{ 0xE6C, 0x10BC, 0x10BC, 0x10FF }).WriteCall(&sub);

				//	-GeneratePreVisData
				//	This command into the code section -GeneratePreVisData.
				//	Should be: -GeneratePreVisData:"<ESMFilename>" [clean, filtered] [all, other, main, ints]

				Relocation(target, Offset{ 0xF6E, 0x11CC, 0x11CC, 0x120F }).WriteCall(&sub);
				Relocation(target, Offset{ 0xF8C, 0x11EA, 0x11EA, 0x122D }).WriteCall(&sub);

				//	Add support quote to command line with -CheckInPlugin
				//	Should be: -CheckInPlugin:"<PluginFilename>" "<ESMFilename>"
				Relocation(target, Offset{ 0x76B, 0x819, 0x819, 0x856 }).WriteCall(&sub);
				Relocation(target, Offset{ 0x78D, 0x83B, 0x83B, 0x878 }).WriteCall(&sub);

				//	Add support quote to command line with -ConvertToESL
				//	Should be: -ConvertToESL:"<PluginFilename>"
				Relocation(target, Offset{ 0x1954, 0x1B9B, 0x1B9B, 0x1BDB }).WriteCall(&sub);

				//	Add support quote to command line with -DumpNeededFiles
				//	Should be: -DumpNeededFiles:"<ESMFilename>" "<OutputFilepath>"
				Relocation(target, Offset{ 0x89E, 0xADB, 0xADB, 0xB18 }).WriteCall(&sub);
				Relocation(target, Offset{ 0x8BC, 0xAF9, 0xAF9, 0xB36 }).WriteCall(&sub);

				//	Add support quote to command line with -SaveDefaultPlugin
				//	Should be: -SaveDefaultPlugin:"<PluginFilename>" "<ESMFilename>"
				Relocation(target, Offset{ 0x6D8, 0x786, 0x786, 0x7C3 }).WriteCall(&sub);
				Relocation(target, Offset{ 0x6F6, 0x7A4, 0x7A4, 0x7E1 }).WriteCall(&sub);

				//	Add support quote to command line with -SaveDefaultPlugin
				//	Should be: -ExportDismemberData:"<ESMFilename>" <XB1|X64|PS4|W32>
				Relocation(target, Offset{ 0x604, 0x6B2, 0x6B2, 0x6EF }).WriteCall(&sub);
				Relocation(target, Offset{ 0x622, 0x6D0, 0x6D0, 0x70D }).WriteCall(&sub);

				//	Add support quote to command line with -UpdateModelData
				//	Should be: -UpdateModelData:"<ESMFilename>"
				Relocation(target, Offset{ 0x599, 0x647, 0x647, 0x684 }).WriteCall(&sub);

				//	Add support quote to command line with -OutputAreaArt
				//	Should be: -OutputAreaArt:"<ESMFilename>" "<AreasFilename>" "<OutputFilename>"
				Relocation(target, Offset{ 0x4DF, 0x58D, 0x58D, 0x5CA }).WriteCall(&sub);
				Relocation(target, Offset{ 0x4FD, 0x5AB, 0x5AB, 0x5E8 }).WriteCall(&sub);
				Relocation(target, Offset{ 0x51B, 0x5C9, 0x5C9, 0x606 }).WriteCall(&sub);

				//	Add support quote to command line with -CompileTextExport
				//	Should be: -CompileTextExport:"<ESMFilename>" "<language>" "<PathToTextExport>" ["<PathBackupToTextExport>"]
				Relocation(target, Offset{ 0x397, 0x443, 0x443, 0x47D }).WriteCall(&sub);
				Relocation(target, Offset{ 0x3B5, 0x461, 0x461, 0x49B }).WriteCall(&sub);

				//	Add support quote to command line with -ExportFaceGenData
				//	Should be: -ExportFaceGenData:"<ESMFilename>" <XB1|X64|PS4|W32>
				Relocation(target, Offset{ 0x231, 0x234, 0x234, 0x235 }).WriteCall(&sub);
				Relocation(target, Offset{ 0x24F, 0x252, 0x252, 0x253 }).WriteCall(&sub);

				//	Add support quote to command line with -GenerateAnimInfo
				//	Should be: -GenerateAnimInfo:"<ESMFilename>" "<DataFilepath>" "<OutputFilepath>" [%s] [%s] [%s]
				Relocation(target, Offset{ 0x93A, 0xB78, 0xB78, 0xBB5 }).WriteCall(&sub);
				Relocation(target, Offset{ 0x958, 0xB96, 0xB96, 0xBD3 }).WriteCall(&sub);
				Relocation(target, Offset{ 0x976, 0xBB4, 0xBB4, 0xBF1 }).WriteCall(&sub);
				Relocation(target, Offset{ 0x994, 0xBD2, 0xBD2, 0xC0F }).WriteCall(&sub);
				Relocation(target, Offset{ 0xAF4, 0xD36, 0xD36, 0xD76 }).WriteCall(&sub);

				//if (VersionLists::GetEditorVersion() > VersionLists::EDITOR_FALLOUT_C4_1_10_982_3)
				//{
				//	//	Add support quote to command line with -GenerateSingleLip
				//	//	Should be: -GenerateSingleLip:"<WavFilename>" "<Text>"
				//	//	Warning: The .lip file will be created next to the audio file.
				//	Relocation(target, Offset{ 0xF29 }).WriteCall(&sub2);
				//}

				//	Add support quote to command line with -GenerateStaticCollections
				//	Should be: -GenerateStaticCollections:"<ESMFilename>" <XB1|X64|PS4|W32>
				Relocation(target, Offset{ 0xD6E, 0xFC0, 0xFC0, 0x1003 }).WriteCall(&sub);
				Relocation(target, Offset{ 0xD8C, 0xFDE, 0xFDE, 0x1021 }).WriteCall(&sub);

				//	Add support quote to command line with -DepersistRefs
				//	Should be: -DepersistRefs:"<ESMFilename>"
				Relocation(target, Offset{ 0x12E2, 0x1535, 0x1535, 0x1578 }).WriteCall(&sub);

				//	Add support quote to command line with -MapMarker
				//	Should be: -MapMarker:"<ESMFilename>" "<Worldspace|Interior>"
				Relocation(target, Offset{ 0x135E, 0x15B1, 0x15B1, 0x15F4 }).WriteCall(&sub);
				Relocation(target, Offset{ 0x137C, 0x15CF, 0x15CF, 0x1612 }).WriteCall(&sub);

				//	Add support quote to command line with -MapInfo
				//	Should be: -MapInfo:"<ESMFilename>" "<Worldspace|Interior>" ["<%s>"]
				Relocation(target, Offset{ 0x13E7, 0x163A, 0x163A, 0x167D }).WriteCall(&sub);
				Relocation(target, Offset{ 0x1405, 0x1658, 0x1658, 0x169B }).WriteCall(&sub);
				Relocation(target, Offset{ 0x1423, 0x1676, 0x1676, 0x16B9 }).WriteCall(&sub);

				//	Add support quote to command line with -ImportScalingData
				//	Should be: -ImportScalingData:"<ESMFilename>"
				Relocation(target, Offset{ 0x156D, 0x17BD, 0x17BD, 0x17FD }).WriteCall(&sub);

				//	Add support quote to command line with -CompressPSG
				//	Should be: -CompressPSG:"<ESMFilename>"
				Relocation(target, Offset{ 0x15D8, 0x1828, 0x1828, 0x1868 }).WriteCall(&sub);

				//	Add support quote to command line with -BuildCDX
				//	Should be: -BuildCDX:"<ESMFilename>"
				Relocation(target, Offset{ 0x1764, 0x19C0, 0x19C0, 0x1A00 }).WriteCall(&sub);

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