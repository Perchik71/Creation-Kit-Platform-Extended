// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once
#pragma pack(push, 1)

#include "NiAPI/NiTArray.h"
#include "Core/Singleton.h"
#include "Editor API/BSTArray.h"
#include "Editor API/BSTList.h"
#include "TESForm.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Starfield
		{
			class BGSAddonNode : public TESForm {};

			class TESFormDetailFixed
			{
			public:
				virtual ~TESFormDetailFixed() = default;
			private:
				char data[0x108];
			};
			static_assert(sizeof(TESFormDetailFixed) == 0x110);

			class TESDataHandler
			{
			public:
				class TESFormArray : public BSTArrayEx<TESForm*>
				{
					char pad10[0x8];
					TESForm::FormType FormType;
					char pad11[0x7];
				public:
					inline TESForm::FormType GetFormType() const { return FormType; }
				};

				class TESRegionList : public BSSimpleList<TESForm*>
				{
				public:
					virtual ~TESRegionList() = default;
				private:
					uint64_t Unk18;
				};
			public:
				virtual ~TESDataHandler() = default;
			public:
				inline BSTArrayEx<TESForm*>& At(TESForm::FormType Type) const { return const_cast<TESDataHandler*>(this)->Forms[Type]; }
				inline BSTArrayEx<TESForm*>& operator[](TESForm::FormType Type) const { return const_cast<TESDataHandler*>(this)->Forms[Type]; }

				inline static Core::ISingleton<bool> UserModdedSingleton;
				inline static Core::ISingleton<TESDataHandler> Singleton;
			public:
				uint32_t Unk008;
				uint32_t Unk00C;
				void* Unk010;											// Contain EditorDataHandler maybe important
				uint32_t Unk018;
				uint32_t Unk01C;
				uint64_t Unk020;
				void* Unk028;											// vtable TESDataHandler offset + 0x28
				char Pad30[0xB0];
				TESFormDetailFixed FormDetail;
				TESFormArray Forms[TESForm::FormType::ftMax];
				char Pad1D30[0x10];
				TESRegionList* Regions;
				BSTArrayEx<TESForm*> Cells;
				char Pad1D58[0x8];
				NiAPI::NiTArray<BGSAddonNode*> AddonNodes;
				char Pad1D78[0x18];
				TESFile* ActivePlugin;									
				BSSimpleList<TESFile*> ModInfoList;
				BSTArrayEx<TESFile*> LoadedMods;
			};
			static_assert(sizeof(TESDataHandler::TESFormArray) == 0x20);
			static_assert(sizeof(TESDataHandler) == 0x1DB8);

			class TESDataHandler_CKPEIntf
			{
			public:
				virtual TESDataHandler* GetSingleton();
				virtual TESForm* GetFormById(uint32_t FormId);
			};
		}
	}
}

#pragma pack(pop)