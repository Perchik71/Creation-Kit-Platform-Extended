// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Singleton.h>
#include <CKPE.Common.Relocation.h>
#include <EditorAPI/IDs.h>
#include <EditorAPI/N/NiTObjectArray.h>
#include <EditorAPI/B/BSTList.h>
#include <EditorAPI/B/BGSLayer.h>
#include <EditorAPI/B/BGSColorForm.h>
#include <EditorAPI/T/TESObjectARMO.h>
#include <EditorAPI/T/TESObjectREFR.h>
#include <EditorAPI/T/TESObjectCELL.h>
#include <EditorAPI/T/TESObjectLAND.h>
#include <EditorAPI/T/TESObjectLIGH.h>
#include <EditorAPI/B/BGSVoiceType.h>
#include <EditorAPI/T/TESTopic.h>
#include <EditorAPI/T/TESFile.h>
#include <EditorAPI/T/TESRegionDataManager.h>
#include <EditorAPI/C/CheckInFormList.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			template<typename _Ty>
			using NiTPrimitiveArray = NiAPI::NiTObjectArray<_Ty>;
			using TESFormArray = BSTArray<Forms::TESForm*>;

			// 1.11.246.0: size 0x20, vtable RVA 0x302FD70, 3 virtual functions. A list of regions; frees them on destruction
			// when ownsRegionMemory is set (same members as CommonLibF4's TESRegionList).
			class TESRegionList
			{
				BSSimpleList<Forms::TESForm*> Region;						// 08
				std::uint8_t ownsRegionMemory;								// 18
				std::uint8_t pad19[0x7];									// 19
			public:
				virtual ~TESRegionList();

				// Calls Forms::TESRegion::CanCheckIn on every region, false as soon as one refuses (RVA 0x967B00).
				[[nodiscard]] virtual bool CanCheckIn(const Forms::CheckInFormList* Forms) const noexcept(true); // 01
				// Calls BuildCheckInDependencyMessage on every region that cannot be checked in (RVA 0x967B70).
				virtual void BuildCheckInDependencyMessage(const Forms::CheckInFormList* Forms, BSString& Message) const noexcept(true); // 02

				[[nodiscard]] inline const BSSimpleList<Forms::TESForm*>& GetRegions() const noexcept(true) { return Region; }
			};
			static_assert(sizeof(TESRegionList) == 0x20);

			class TESDataHandler
			{
			public:
				using TESFileList = BSSimpleList<TESFile>;
				using TESFileArray = BSTArray<TESFile*>;

				struct ModList {
					TESFileList modInfoList;							// 00
					TESFileArray loadedMods;							// 10
				};
				static_assert(sizeof(ModList) == 0x28);
				static_assert(offsetof(ModList, loadedMods) == 0x10);

				using UnkArray = BSTArray<LPVOID>;
				// BSTEventSource<TESDataHandlerEditorEvent> (the render window edit module manager registers as a
				// BSTEventSink<TESDataHandlerEditorEvent> on it, see sub_14008DAA5); same layout as CommonLibF4's BSTEventSource.
				std::uint32_t eventLock;								// 00
				std::uint32_t eventLockOwner;							// 04
				BSTArray<void*> eventSinks;								// 08
				BSTArray<void*> eventPendingRegisters;					// 20
				BSTArray<void*> eventPendingUnregisters;				// 38
				std::uint8_t eventNotifying;							// 50
				char pad51[0x17];										// 51 - BSTSingletonSDM<TESDataHandler> and the slot CommonLibF4 uses for objectList (neither set by the CK)
				TESFormArray arrNONE;									// 68 Form Type 0
				TESFormArray arrTES4;									// 80 Form Type 1
				TESFormArray arrGRUP;									// 98 Form Type 2
				TESFormArray arrGMST;									// B0 Form Type 3
				TESFormArray arrKYWD;									// C8 Form Type 4
				TESFormArray arrLCRT;									// E0 Form Type 5
				TESFormArray arrAACT;									// F8 Form Type 6
				BSTArray<Forms::BGSTransform*> arrTRNS;					// 110 Form Type 7
				TESFormArray arrCMPO;									// 128 Form Type 8
				TESFormArray arrTXST;									// 140 Form Type 9
				TESFormArray arrMICN;									// 158 Form Type 10
				TESFormArray arrGLOB;									// 170 Form Type 11
				TESFormArray arrDMGT;									// 188 Form Type 12
				TESFormArray arrCLAS;									// 1A0 Form Type 13
				TESFormArray arrFACT;									// 1B8 Form Type 14
				TESFormArray arrHDPT;									// 1D0 Form Type 15
				TESFormArray arrEYES;									// 1E8 Form Type 16
				TESFormArray arrRACE;									// 200 Form Type 17
				TESFormArray arrSOUN;									// 218 Form Type 18
				TESFormArray arrASPC;									// 230 Form Type 19
				TESFormArray arrSKIL;									// 248 Form Type 20
				TESFormArray arrMGEF;									// 260 Form Type 21
				TESFormArray arrSCPT;									// 278 Form Type 22
				TESFormArray arrLTEX;									// 290 Form Type 23
				TESFormArray arrENCH;									// 2A8 Form Type 24
				TESFormArray arrSPEL;									// 2C0 Form Type 25
				TESFormArray arrSCRL;									// 2D8 Form Type 26
				TESFormArray arrACTI;									// 2F0 Form Type 27
				TESFormArray arrTACT;									// 308 Form Type 28
				BSTArray<Forms::TESObjectARMO*> arrARMO;				// 320 Form Type 29
				TESFormArray arrBOOK;									// 338 Form Type 30
				TESFormArray arrCONT;									// 350 Form Type 31
				TESFormArray arrDOOR;									// 368 Form Type 32
				TESFormArray arrINGR;									// 380 Form Type 33
				TESFormArray arrLIGH;									// 398 Form Type 34
				TESFormArray arrMISC;									// 3B0 Form Type 35
				TESFormArray arrSTAT;									// 3C8 Form Type 36  
				TESFormArray arrSCOL;									// 3E0 Form Type 37 
				TESFormArray arrMSTT;									// 3F8 Form Type 38 
				TESFormArray arrGRAS;									// 410 Form Type 39
				TESFormArray arrTREE;									// 428 Form Type 40
				TESFormArray arrFLOR;									// 440 Form Type 41
				TESFormArray arrFURN;									// 458 Form Type 42
				TESFormArray arrWEAP;									// 470 Form Type 43
				TESFormArray arrAMMO;									// 488 Form Type 44
				TESFormArray arrNPC_;									// 4A0 Form Type 45
				TESFormArray arrLVLN;									// 4B8 Form Type 46
				TESFormArray arrKEYM;									// 4D0 Form Type 47
				TESFormArray arrALCH;									// 4E8 Form Type 48
				TESFormArray arrIDLM;									// 500 Form Type 49
				TESFormArray arrNOTE;									// 518 Form Type 50
				TESFormArray arrPROJ;									// 530 Form Type 51
				TESFormArray arrHAZD;									// 548 Form Type 52
				TESFormArray arrBNDS;									// 560 Form Type 53
				TESFormArray arrSLGM;									// 578 Form Type 54
				TESFormArray arrTERM;									// 590 Form Type 55
				TESFormArray arrLVLI;									// 5A8 Form Type 56
				TESFormArray arrWTHR;									// 5C0 Form Type 57
				TESFormArray arrCLMT;									// 5D8 Form Type 58
				TESFormArray arrSPGD;									// 5F0 Form Type 59
				TESFormArray arrRFCT;									// 608 Form Type 60
				TESFormArray arrREGN;									// 620 Form Type 61
				TESFormArray arrNAVI;									// 638 Form Type 62
				BSTArray<Forms::TESObjectCELL*> arrCELL;				// 650 Form Type 63
				BSTArray<Forms::TESObjectREFR*> arrREFR;				// 668 Form Type 64
				TESFormArray arrACHR;									// 680 Form Type 65
				TESFormArray arrPMIS;									// 698 Form Type 66
				TESFormArray arrPARW;									// 6B0 Form Type 67
				TESFormArray arrPGRE;									// 6C8 Form Type 68
				TESFormArray arrPBEA;									// 6E0 Form Type 69
				TESFormArray arrPFLA;									// 6F8 Form Type 70
				TESFormArray arrPCOM;									// 710 Form Type 71
				TESFormArray arrPBAR;									// 728 Form Type 72
				TESFormArray arrPHZD;									// 740 Form Type 73
				TESFormArray arrWRLD;									// 758 Form Type 74
				TESFormArray arrLAND;									// 770 Form Type 75
				TESFormArray arrNAVM;									// 788 Form Type 76
				TESFormArray arrTLOD;									// 7A0 Form Type 77
				BSTArray<Forms::TESTopic*> arrDIAL;						// 7B8 Form Type 78
				BSTArray<Forms::TESTopicInfo*> arrINFO;					// 7D0 Form Type 79
				BSTArray<Forms::TESQuest*> arrQUST;						// 7E8 Form Type 80
				TESFormArray arrIDLE;									// 800 Form Type 81
				TESFormArray arrPACK;									// 818 Form Type 82
				TESFormArray arrCSTY;									// 830 Form Type 83
				TESFormArray arrLSCR;									// 848 Form Type 84
				TESFormArray arrLVSP;									// 860 Form Type 85
				TESFormArray arrANIO;									// 878 Form Type 86
				TESFormArray arrWATR;									// 890 Form Type 87
				TESFormArray arrEFSH;									// 8A8 Form Type 88
				TESFormArray arrTOFT;									// 8C0 Form Type 89
				TESFormArray arrEXPL;									// 8D8 Form Type 90
				TESFormArray arrDEBR;									// 8F0 Form Type 91
				TESFormArray arrIMGS;									// 908 Form Type 92
				TESFormArray arrIMAD;									// 920 Form Type 93
				TESFormArray arrFLST;									// 938 Form Type 94
				TESFormArray arrPERK;									// 950 Form Type 95
				TESFormArray arrBPTD;									// 968 Form Type 96
				TESFormArray arrADDN;									// 980 Form Type 97
				BSTArray<Forms::TESActorValue*> arrAVIF;				// 998 Form Type 98 
				TESFormArray arrCAMS;									// 9B0 Form Type 99
				TESFormArray arrCPTH;									// 9C8 Form Type 100
				BSTArray<Forms::BGSVoiceType*> arrVTYP;					// 9E0 Form Type 101
				TESFormArray arrMATT;									// 9F8 Form Type 102
				TESFormArray arrIPCT;									// A10 Form Type 103
				TESFormArray arrIPDS;									// A28 Form Type 104
				TESFormArray arrARMA;									// A40 Form Type 105
				TESFormArray arrECZN;									// A58 Form Type 106
				TESFormArray arrLCTN;									// A70 Form Type 107
				TESFormArray arrMESG;									// A88 Form Type 108
				TESFormArray arrRGDL;									// AA0 Form Type 109
				TESFormArray arrDOBJ;									// AB8 Form Type 110
				TESFormArray arrDFOB;									// AD0 Form Type 111
				TESFormArray arrLGTM;									// AE8 Form Type 112
				TESFormArray arrMUSC;									// B00 Form Type 113
				TESFormArray arrFSTP;									// B18 Form Type 114
				TESFormArray arrFSTS;									// B30 Form Type 115
				TESFormArray arrSMBN;									// B48 Form Type 116
				TESFormArray arrSMQN;									// B60 Form Type 117
				TESFormArray arrSMEN;									// B78 Form Type 118
				TESFormArray arrDLBR;									// B90 Form Type 119
				TESFormArray arrMUST;									// BA8 Form Type 120
				TESFormArray arrDLVW;									// BC0 Form Type 121
				TESFormArray arrWOOP;									// BD8 Form Type 122
				TESFormArray arrSHOU;									// BF0 Form Type 123
				TESFormArray arrEQUP;									// C08 Form Type 124
				TESFormArray arrRELA;									// C20 Form Type 125
				TESFormArray arrSCEN;									// C38 Form Type 126
				TESFormArray arrASTP;									// C50 Form Type 127
				TESFormArray arrOTFT;									// C68 Form Type 128
				TESFormArray arrARTO;									// C80 Form Type 129
				TESFormArray arrMATO;									// C98 Form Type 130
				TESFormArray arrMOVT;									// CB0 Form Type 131
				TESFormArray arrSNDR;									// CC8 Form Type 132
				TESFormArray arrDUAL;									// CE0 Form Type 133
				TESFormArray arrSNCT;									// CF8 Form Type 134
				TESFormArray arrSOPM;									// D10 Form Type 135
				TESFormArray arrCOLL;									// D28 Form Type 136
				BSTArray<Forms::BGSColorForm*> arrCLFM;					// D40 Form Type 137
				TESFormArray arrREVB;									// D58 Form Type 138
				TESFormArray arrPKIN;									// D70 Form Type 139
				TESFormArray arrRFGP;									// D88 Form Type 140
				TESFormArray arrAMDL;									// DA0 Form Type 141
				BSTArray<Forms::BGSLayer*> arrLAYR;						// DB8 Form Type 142
				TESFormArray arrCOBJ;									// DD0 Form Type 143
				TESFormArray arrOMOD;									// DE8 Form Type 144
				BSTArray<Forms::BGSMaterialSwap*> arrMSWP;				// E00 Form Type 145
				TESFormArray arrZOOM;									// E18 Form Type 146
				TESFormArray arrINNR;									// E30 Form Type 147
				TESFormArray arrKSSM;									// E48 Form Type 148
				TESFormArray arrAECH;									// E60 Form Type 149
				TESFormArray arrSCCO;									// E78 Form Type 150
				TESFormArray arrAORU;									// E90 Form Type 151
				TESFormArray arrSCSN;									// EA8 Form Type 152
				TESFormArray arrSTAG;									// EC0 Form Type 153
				TESFormArray arrNOCM;									// ED8 Form Type 154
				TESFormArray arrLENS;									// EF0 Form Type 155
				TESFormArray arrLSPR;									// F08 Form Type 156
				TESFormArray arrGDRY;									// F20 Form Type 157
				TESFormArray arrOVIS;									// F38 Form Type 158
				TESRegionList* regionList;								// F50
				NiTPrimitiveArray<Forms::TESObjectCELL*> cellList;		// F58
				NiTPrimitiveArray<Forms::TESForm*> addonNodes;			// F70
				char unkF88[0x18];										// F88 - zeroed by the constructor (CommonLibF4: NiTList<TESForm*> badForms)
				std::uint32_t NextFormId;								// FA0 - 0x800 on construction
				std::uint32_t UnkDAC;									// FA4
				TESFile* activePlugin;									// FA8
				ModList modList;										// FB0
				BSTArray<TESFile*> smallMods;							// FD8 - light (ESL) plugins; loadedMods + smallMods form CommonLibF4's TESFileCollection at FC0
				std::uint8_t unkFF0;									// FF0
				std::uint8_t unkFF1;									// FF1 - LoadFiles / TESWorldSpace::InitItemImpl gate
				std::uint8_t buildingLOD;								// FF2 - set by the LOD build dispatcher for its duration, read by Save
				std::uint8_t unkFF3;									// FF3 - LoadFiles: loads the active file's TES info first
				std::uint8_t unkFF4;									// FF4 - gates TESForm file reassignment and NavMesh::LoadEdit
				std::uint8_t unkFF5;									// FF5 - SaveForm: skips forms of non-master files when set
				std::uint8_t clearingData;								// FF6 - set while the TES is destroyed; suppresses TESForm::SetAltered and UI updates
				std::uint8_t unkFF7;									// FF7 - 1 on construction
				std::uint8_t unkFF8;									// FF8 - gates the LightRadius lookup (sub_140C54630)
				std::uint8_t loadingFiles;								// FF9 - set around LoadFiles; the TESForm constructor does not allocate a form ID while set
				std::uint8_t unkFFA;									// FFA
				std::uint8_t unkFFB;									// FFB - written by LoadFileRecords from its bool argument
				std::uint8_t saveForGame;								// FFC - Save: with the game window open posts "FCF" to port 8080, else may launch Beta.exe with the plugin
				std::uint8_t saveForGameAlt;							// FFD - Save: posts "FCF" to port 4600 (set by the save handler for mode 4)
				std::uint8_t saveExtraNotify;							// FFE - Save: extra notification step (set by the save handler for mode 1)
				std::uint8_t unkFFF;									// FFF
				TESRegionDataManager* regionDataManager;				// 1000 - a TESRegionDataManagerEditor
				std::uint8_t unk1008;									// 1008 - cleared by the constructor
				char pad1009[0x7];										// 1009
			public:
				inline const TESFileList* GetMods() const { return &modList.modInfoList; }
				inline const TESFileArray* GetLoadedMods() const { return &modList.loadedMods; }
				inline const BSTArray<TESFile*>* GetSmallMods() const { return &smallMods; }
				inline TESFile* GetActiveMod() const { return activePlugin; }
				inline bool IsActiveMod() const { return activePlugin != nullptr; }

				inline bool AddForm(Forms::TESForm* form) noexcept(true)
				{
					using func_t = decltype(&TESDataHandler::AddForm);
					static Common::Relocation<func_t> func{
						EditorAPI::IDs::TESDataHandler::AddForm };
					return func(this, form);
				}

				inline void RemoveForm(Forms::TESForm* form) noexcept(true)
				{
					using func_t = decltype(&TESDataHandler::RemoveForm);
					static Common::Relocation<func_t> func{
						EditorAPI::IDs::TESDataHandler::RemoveForm };
					func(this, form);
				}

				inline bool LoadFiles(bool flag) noexcept(true)
				{
					using func_t = decltype(&TESDataHandler::LoadFiles);
					static Common::Relocation<func_t> func{
						IDs::Functions::TESDataHandler__LoadFiles };
					return func(this, flag);
				}

				inline bool LoadFileRecords(TESFile* file, bool flag) noexcept(true)
				{
					using func_t = decltype(&TESDataHandler::LoadFileRecords);
					static Common::Relocation<func_t> func{
						IDs::Functions::TESDataHandler__LoadFileRecords };
					return func(this, file, flag);
				}

				inline static ISingleton<bool> UserModdedSingleton;
				inline static ISingleton<TESDataHandler> Singleton;

				// File>Save core: pre-processes the forms, creates the temporary file, saves every form through SaveForm and
				// replaces the target file. FileName is the target plugin name. CK 1.11.246.0 0x1408675F0.
				inline bool Save(const char* FileName) noexcept(true)
				{
					using func_t = decltype(&TESDataHandler::Save);
					static Common::Relocation<func_t> func{ IDs::TESDataHandler::Save };
					return func(this, FileName);
				}

				// Per-form dispatch of the Save loop: skips deleted forms and forms of other files, then calls the form's
				// save virtual (vtable +0xE8 for master-flagged forms, +0xD8 otherwise). CK 1.11.246.0 0x140859540.
				inline bool SaveForm(Forms::TESForm* Form) noexcept(true)
				{
					using func_t = decltype(&TESDataHandler::SaveForm);
					static Common::Relocation<func_t> func{ IDs::TESDataHandler::SaveForm };
					return func(this, Form);
				}
			public:
				CKPE_READ_PROPERTY(GetMods) TESFileList* Mods;
				CKPE_READ_PROPERTY(GetLoadedMods) const TESFileArray* LoadedMods;
				CKPE_READ_PROPERTY(GetActiveMod) TESFile* ActiveMod;
			};
			static_assert(sizeof(TESDataHandler) == 0x1010);
			static_assert(offsetof(TESDataHandler, arrNONE) == 0x68);
			static_assert(offsetof(TESDataHandler, arrOVIS) == 0xF38);
			static_assert(offsetof(TESDataHandler, regionList) == 0xF50);
			static_assert(offsetof(TESDataHandler, NextFormId) == 0xFA0);
			static_assert(offsetof(TESDataHandler, activePlugin) == 0xFA8);
			static_assert(offsetof(TESDataHandler, modList) == 0xFB0);
			static_assert(offsetof(TESDataHandler, smallMods) == 0xFD8);
			static_assert(offsetof(TESDataHandler, buildingLOD) == 0xFF2);
			static_assert(offsetof(TESDataHandler, clearingData) == 0xFF6);
			static_assert(offsetof(TESDataHandler, loadingFiles) == 0xFF9);
			static_assert(offsetof(TESDataHandler, saveForGame) == 0xFFC);
			static_assert(offsetof(TESDataHandler, regionDataManager) == 0x1000);
		}
	}
}