// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "NiAPI\NiTArray.h"
#include "TESFile.h"
#include "TESObjectREFR.h"
#include "TESObjectCELL.h"
#include "TESObjectLAND.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			template<typename _Ty>
			using NiTPrimitiveArray = NiAPI::NiTArray<_Ty>;

			class TESDataHandler
			{
			public:
				using TESFileList = BSSimpleList<TESFile>;
				using TESFileArray = BSTArray<TESFile*>;

				struct ModList {
					TESFileList modInfoList;				// 00
					TESFileArray loadedMods;				// 10
				};

				using UnkArray = BSTArray<LPVOID>;
			private:
				UnkArray unk00;								// 00
				char pad18[0x50];							// 18
			public:
				TESFormArray arrNONE;						// 68 Form Type 0
				TESFormArray arrTES4;						// 80 Form Type 1
				TESFormArray arrGRUP;						// 98 Form Type 2
				TESFormArray arrGMST;						// B0 Form Type 3
				TESFormArray arrKYWD;						// C8 Form Type 4
				TESFormArray arrLCRT;						// E0 Form Type 5
				TESFormArray arrAACT;						// F8 Form Type 6
				TESFormArray arrTXST;						// 110 Form Type 7
				TESFormArray arrMICN;						// 128 Form Type 8
				TESFormArray arrGLOB;						// 140 Form Type 9
				TESFormArray arrCLAS;						// 158 Form Type 10
				TESFormArray arrFACT;						// 170 Form Type 11
				TESFormArray arrHDPT;						// 188 Form Type 12
				TESFormArray arrEYES;						// 1A0 Form Type 13
				TESFormArray arrRACE;						// 1B8 Form Type 14
				TESFormArray arrSOUN;						// 1D0 Form Type 15
				TESFormArray arrASPC;						// 1E8 Form Type 16
				TESFormArray arrSKIL;						// 200 Form Type 17
				TESFormArray arrMGEF;						// 218 Form Type 18
				TESFormArray arrSCPT;						// 230 Form Type 19
				TESFormArray arrLTEX;						// 248 Form Type 20
				TESFormArray arrENCH;						// 260 Form Type 21
				TESFormArray arrSPEL;						// 278 Form Type 22
				TESFormArray arrSCRL;						// 290 Form Type 23
				TESFormArray arrACTI;						// 2A8 Form Type 24
				TESFormArray arrTACT;						// 2C0 Form Type 25
				TESFormArray arrARMO;						// 2D8 Form Type 26
				TESFormArray arrBOOK;						// 2F0 Form Type 27
				TESFormArray arrCONT;						// 308 Form Type 28
				TESFormArray arrDOOR;						// 320 Form Type 29
				TESFormArray arrINGR;						// 338 Form Type 30
				TESFormArray arrLIGH;						// 350 Form Type 31
				TESFormArray arrMISC;						// 368 Form Type 32
				TESFormArray arrAPPA;						// 380 Form Type 33
				TESFormArray arrSTAT;						// 398 Form Type 34
				TESFormArray arrSCOL;						// 3B0 Form Type 35
				TESFormArray arrMSTT;						// 3C8 Form Type 36
				TESFormArray arrGRAS;						// 3E0 Form Type 37
				TESFormArray arrTREE;						// 3F8 Form Type 38
				TESFormArray arrFLOR;						// 410 Form Type 39
				TESFormArray arrFURN;						// 428 Form Type 40
				TESFormArray arrWEAP;						// 440 Form Type 41
				TESFormArray arrAMMO;						// 458 Form Type 42
				TESFormArray arrNPC_;						// 470 Form Type 43
				TESFormArray arrLVLN;						// 488 Form Type 44
				TESFormArray arrKEYM;						// 4A0 Form Type 45
				TESFormArray arrALCH;						// 4B8 Form Type 46
				TESFormArray arrIDLM;						// 4D0 Form Type 47
				TESFormArray arrNOTE;						// 4E8 Form Type 48
				TESFormArray arrCOBJ;						// 500 Form Type 49
				TESFormArray arrPROJ;						// 518 Form Type 50
				TESFormArray arrHAZD;						// 530 Form Type 51
				TESFormArray arrSLGM;						// 548 Form Type 52
				TESFormArray arrLVLI;						// 560 Form Type 53
				TESFormArray arrWTHR;						// 578 Form Type 54
				TESFormArray arrCLMT;						// 590 Form Type 55
				TESFormArray arrSPGD;						// 5A8 Form Type 56
				TESFormArray arrRFCT;						// 5C0 Form Type 57
				TESFormArray arrREGN;						// 5D8 Form Type 58
				TESFormArray arrNAVI;						// 5F0 Form Type 59
				BSTArray<TESObjectCELL*> arrCELL;			// 608 Form Type 60
				BSTArray<TESObjectREFR*> arrREFR;			// 620 Form Type 61
				TESFormArray arrACHR;						// 638 Form Type 62
				TESFormArray arrPMIS;						// 650 Form Type 63
				TESFormArray arrPARW;						// 668 Form Type 64
				TESFormArray arrPGRE;						// 680 Form Type 65
				TESFormArray arrPBEA;						// 698 Form Type 66
				TESFormArray arrPFLA;						// 6B0 Form Type 67
				TESFormArray arrPCON;						// 6C8 Form Type 68
				TESFormArray arrPBAR;						// 6E0 Form Type 69
				TESFormArray arrPHZD;						// 6F8 Form Type 70
				TESFormArray arrWRLD;						// 710 Form Type 71
				BSTArray<TESObjectLAND*> arrLAND;			// 728 Form Type 72 
				TESFormArray arrNAVM;						// 740 Form Type 73
				TESFormArray arrTLOD;						// 758 Form Type 74
				TESFormArray arrDIAL;						// 770 Form Type 75
				TESFormArray arrINFO;						// 788 Form Type 76
				TESFormArray arrQUST;						// 7A0 Form Type 77
				TESFormArray arrIDLE;						// 7B8 Form Type 78
				TESFormArray arrPACK;						// 7D0 Form Type 79
				TESFormArray arrCSTY;						// 7E8 Form Type 80
				TESFormArray arrLSCR;						// 800 Form Type 81
				TESFormArray arrLVSP;						// 818 Form Type 82
				TESFormArray arrANIO;						// 830 Form Type 83
				TESFormArray arrWATR;						// 848 Form Type 84
				TESFormArray arrEFSH;						// 860 Form Type 85
				TESFormArray arrTOFT;						// 878 Form Type 86
				TESFormArray arrEXPL;						// 890 Form Type 87
				TESFormArray arrDEBR;						// 8A8 Form Type 88
				TESFormArray arrIMGS;						// 8C0 Form Type 89
				TESFormArray arrIMAD;						// 8D8 Form Type 90
				TESFormArray arrFLST;						// 8F0 Form Type 91
				TESFormArray arrPERK;						// 908 Form Type 92
				TESFormArray arrBPTD;						// 920 Form Type 93
				TESFormArray arrADDN;						// 938 Form Type 94
				TESFormArray arrAVIF;						// 950 Form Type 95
				TESFormArray arrCAMS;						// 968 Form Type 96
				TESFormArray arrCPTH;						// 980 Form Type 97
				TESFormArray arrVTYP;						// 998 Form Type 98
				TESFormArray arrMATT;						// 9B0 Form Type 99
				TESFormArray arrIPCT;						// 9C8 Form Type 100
				TESFormArray arrIPDS;						// 9E0 Form Type 101
				TESFormArray arrARMA;						// 9F8 Form Type 102
				TESFormArray arrECZN;						// A10 Form Type 103
				TESFormArray arrLCTN;						// A28 Form Type 104
				TESFormArray arrMESG;						// A40 Form Type 105
				TESFormArray arrRGDL;						// A58 Form Type 106
				TESFormArray arrDOBJ;						// A70 Form Type 107
				TESFormArray arrLGTM;						// A88 Form Type 108
				TESFormArray arrMUSC;						// AA0 Form Type 109
				TESFormArray arrFSTP;						// AB8 Form Type 110
				TESFormArray arrFSTS;						// AD0 Form Type 111
				TESFormArray arrSMBN;						// AE8 Form Type 112
				TESFormArray arrSMQN;						// B00 Form Type 113
				TESFormArray arrSMEN;						// B18 Form Type 114
				TESFormArray arrDLBR;						// B30 Form Type 115
				TESFormArray arrMUST;						// B48 Form Type 116
				TESFormArray arrDLVW;						// B60 Form Type 117
				TESFormArray arrWOOP;						// B78 Form Type 118
				TESFormArray arrSHOU;						// B90 Form Type 119
				TESFormArray arrEQUP;						// BA8 Form Type 120
				TESFormArray arrRELA;						// BC0 Form Type 121
				TESFormArray arrSCEN;						// BD8 Form Type 122
				TESFormArray arrASTP;						// BF0 Form Type 123
				TESFormArray arrOTFT;						// C08 Form Type 124
				TESFormArray arrARTO;						// C20 Form Type 125
				TESFormArray arrMATO;						// C38 Form Type 126
				TESFormArray arrMOVT;						// C50 Form Type 127
				TESFormArray arrSNDR;						// C68 Form Type 128
				TESFormArray arrDUAL;						// C80 Form Type 129
				TESFormArray arrSNCT;						// C98 Form Type 130
				TESFormArray arrSOPM;						// CB0 Form Type 131
				TESFormArray arrCOLL;						// CC8 Form Type 132
				TESFormArray arrCLFM;						// CE0 Form Type 133
				TESFormArray arrREVB;						// CF8 Form Type 134
				TESFormArray arrLENS;						// D10 Form Type 135
				TESFormArray arrLSPR;						// D28 Form Type 136
				TESFormArray arrVOLI;						// D40 Form Type 137
			private:
				void* UnkPtrD48;							// D48
			public:
				NiTPrimitiveArray<TESForm*>	cellList;		// D50
				NiTPrimitiveArray<TESForm*>	addonNodes;		// D68
			private:
				char padD80[0x18];							// D80
				uint32_t NextFormId;						// DA8
				uint32_t UnkDAC;							// DAC
				TESFile* activePlugin;						// DB0
			public:
				ModList modList;							// DB8
			public:
				inline const TESFileList* GetMods() const { return &modList.modInfoList; }
				inline const TESFileArray* GetLoadedMods() const { return &modList.loadedMods; }
				inline TESFile* GetActiveMod() const { return activePlugin; }
				inline bool IsActiveMod() const { return activePlugin != nullptr; }
				inline static TESDataHandler* GetInstance() { return *InstancePtr; }
				inline static TESDataHandler** InstancePtr;
			public:
				READ_PROPERTY(GetMods) TESFileList* Mods;
				READ_PROPERTY(GetLoadedMods) const TESFileArray* LoadedMods;
				READ_PROPERTY(GetActiveMod) TESFile* ActiveMod;
			};
			static_assert(sizeof(TESDataHandler) == 0xDE0);
		}
	}
}