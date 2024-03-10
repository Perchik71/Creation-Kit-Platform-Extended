// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixQuoteCmdLine.h"
#include "Editor API/BSString.h"
#include "Patches/Windows/FO4/MainWindowF4.h"
#include "Patches/Windows/FO4/ResponseWindowF4.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			uintptr_t pointer_FixQuoteCmdLine_sub1 = 0;
			uintptr_t pointer_FixQuoteCmdLine_sub2 = 0;
			uintptr_t pointer_FixQuoteCmdLine_data = 0;

			FixQuoteCmdLinePatch::FixQuoteCmdLinePatch() : Module(GlobalEnginePtr)
			{}

			bool FixQuoteCmdLinePatch::HasOption() const
			{
				return false;
			}

			bool FixQuoteCmdLinePatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixQuoteCmdLinePatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixQuoteCmdLinePatch::GetName() const
			{
				return "Fixed quote to cmdline";
			}

			bool FixQuoteCmdLinePatch::HasDependencies() const
			{
				return true;
			}

			Array<String> FixQuoteCmdLinePatch::GetDependencies() const
			{
				return { "Main Window", "Response Window" };
			}

			bool FixQuoteCmdLinePatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixQuoteCmdLinePatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//	Add support quote to command line with -GeneratePreCombined
					//	Should be: -GeneratePreCombined:"<ESMFilename>" [clean, filtered] [all, other, main, ints]

					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(1), (uintptr_t)&sub);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(2), (uintptr_t)&sub);

					//	-GeneratePreVisData
					//	This command into the code section -GeneratePreCombined.
					//	Should be: -GeneratePreVisData:"<ESMFilename>" [clean, filtered] [all, other, main, ints]

					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(3), (uintptr_t)&sub);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(4), (uintptr_t)&sub);

					//	Add support quote to command line with -CheckInPlugin
					//	Should be: -CheckInPlugin:"<PluginFilename>" "<ESMFilename>"

					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(5), (uintptr_t)&sub);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(6), (uintptr_t)&sub);

					//	Add support quote to command line with -ConvertToESL
					//	Should be: -ConvertToESL:"<PluginFilename>"

					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(7), (uintptr_t)&sub);

					//	Add support quote to command line with -DumpNeededFiles
					//	Should be: -DumpNeededFiles:"<ESMFilename>" "<OutputFilepath>"

					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(8), (uintptr_t)&sub);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(9), (uintptr_t)&sub);

					//	Add support quote to command line with -SaveDefaultPlugin
					//	Should be: -SaveDefaultPlugin:"<PluginFilename>" "<ESMFilename>"

					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(10), (uintptr_t)&sub);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(11), (uintptr_t)&sub);

					//	Add support quote to command line with -SaveDefaultPlugin
					//	Should be: -ExportDismemberData:"<ESMFilename>" <XB1|X64|PS4|W32>

					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(12), (uintptr_t)&sub);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(13), (uintptr_t)&sub);

					//	Add support quote to command line with -UpdateModelData
					//	Should be: -UpdateModelData:"<ESMFilename>"

					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(14), (uintptr_t)&sub);

					//	Add support quote to command line with -OutputAreaArt
					//	Should be: -OutputAreaArt:"<ESMFilename>" "<AreasFilename>" "<OutputFilename>"

					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(15), (uintptr_t)&sub);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(16), (uintptr_t)&sub);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(17), (uintptr_t)&sub);

					//	Add support quote to command line with -CompileTextExport
					//	Should be: -CompileTextExport:"<ESMFilename>" "<language>" "<PathToTextExport>" ["<PathBackupToTextExport>"]

					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(18), (uintptr_t)&sub);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(19), (uintptr_t)&sub);

					//	Add support quote to command line with -ExportFaceGenData
					//	Should be: -ExportFaceGenData:"<ESMFilename>" <XB1|X64|PS4|W32>

					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(20), (uintptr_t)&sub);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(21), (uintptr_t)&sub);

					//	Add support quote to command line with -GenerateAnimInfo
					//	Should be: -GenerateAnimInfo:"<ESMFilename>" "<DataFilepath>" "<OutputFilepath>" [%s] [%s] [%s]

					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(22), (uintptr_t)&sub);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(23), (uintptr_t)&sub);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(24), (uintptr_t)&sub);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(25), (uintptr_t)&sub);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(26), (uintptr_t)&sub);

					//	Add support quote to command line with -GenerateSingleLip
					//	Should be: -GenerateSingleLip:"<WavFilename>" "<Text>"
					//	Warning: The .lip file will be created next to the audio file.

					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(27), (uintptr_t)&sub2);

					//	Add support quote to command line with -GenerateStaticCollections
					//	Should be: -GenerateStaticCollections:"<ESMFilename>" <XB1|X64|PS4|W32>

					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(28), (uintptr_t)&sub);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(29), (uintptr_t)&sub);

					//	Add support quote to command line with -DepersistRefs
					//	Should be: -DepersistRefs:"<ESMFilename>"

					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(30), (uintptr_t)&sub);

					//	Add support quote to command line with -MapMarker
					//	Should be: -MapMarker:"<ESMFilename>" "<Worldspace|Interior>"

					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(31), (uintptr_t)&sub);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(32), (uintptr_t)&sub);

					//	Add support quote to command line with -MapInfo
					//	Should be: -MapInfo:"<ESMFilename>" "<Worldspace|Interior>" ["<%s>"]

					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(33), (uintptr_t)&sub);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(34), (uintptr_t)&sub);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(35), (uintptr_t)&sub);

					//	Add support quote to command line with -ImportScalingData
					//	Should be: -ImportScalingData:"<ESMFilename>"

					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(36), (uintptr_t)&sub);

					//	Add support quote to command line with -CompressPSG
					//	Should be: -CompressPSG:"<ESMFilename>"

					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(37), (uintptr_t)&sub);

					//	Add support quote to command line with -BuildCDX
					//	Should be: -BuildCDX:"<ESMFilename>"

					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(38), (uintptr_t)&sub);

					pointer_FixQuoteCmdLine_sub1 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(39));
					pointer_FixQuoteCmdLine_sub2 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(40));
					pointer_FixQuoteCmdLine_data = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(41));

					return true;
				}

				return false;
			}

			bool FixQuoteCmdLinePatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			char* FixQuoteCmdLinePatch::sub(char* str, const char* delim, char** next_token)
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

						if (lpRes && !Utils::Trim(lpRes).length())
							lpRes = strtok_s(nullptr, "\"", next_token);

						return lpRes;
					}
					else
						return strtok_s(nullptr, " ", next_token);
				}
				else
					return strtok_s(str, delim, next_token);
			}

			void FixQuoteCmdLinePatch::sub2(char* lpCmdLine, char* arg2)
			{
				char* next_token = nullptr;
				char* token = sub(lpCmdLine, " ", &next_token);
				if (token) 
				{
					char* filename = token;
					token = sub(nullptr, " ", &next_token);
					if (token) 
					{
						Patches::Fallout4::ResponseWindow::ModuleInit();
						auto pathAudioFile = EditorAPI::BSString::Utils::ChangeFileExt(filename, ".wav");
						auto pathLipFile = EditorAPI::BSString::Utils::ChangeFileExt(filename, ".lip");
						Patches::Fallout4::ResponseWindow::GenerationLip(pathAudioFile.c_str(), pathLipFile.c_str(), token);
						Utils::Quit();
					}
				}

				MessageBoxA(0,
					"Error parsing command-line. Format should be -GenerateSingleLip:\"WAVFilePath\" \"Text For WAV File.\"",
					"Error", 
					MB_OK | MB_ICONERROR);

				Utils::Quit();
			}
		}
	}
}