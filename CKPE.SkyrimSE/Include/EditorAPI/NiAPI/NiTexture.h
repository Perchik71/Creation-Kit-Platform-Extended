// Copyright © 2024 aka perchik71.All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

//#include "NiObject.h"
//#include "Editor API/SSE/BSFixedString.h"
//#include "Editor API/SSE/BSEntryString.h"
//#include "Editor API/FO4/BSFixedString.h"
//#include "Editor API/FO4/BSEntryString.h"
//
//#pragma pack(push, 8)
//
//namespace CreationKitPlatformExtended
//{
//	namespace EditorAPI
//	{
//		namespace SkyrimSpectialEdition
//		{
//			namespace BSGraphics
//			{
//				struct Texture;
//			}
//		}
//	}
//
//	namespace NiAPI
//	{
//		namespace SkyrimSpectialEdition
//		{
//			using namespace EditorAPI::SkyrimSpectialEdition;
//
//			class NiTexture : public NiObject
//			{
//			public:
//				inline BSFixedString GetFileNameFS() const noexcept { return _FileName; }
//				inline const char* GetFileName() const noexcept { return _FileName.c_str(); }
//				inline NiTexture* GetPrev() const noexcept { return _PrevTexture; }
//				inline NiTexture* GetNext() const noexcept { return _NextTexture; }
//				inline BSGraphics::Texture* GetTexture() const noexcept { return _Texture; }
//				inline void SetTexture(BSGraphics::Texture* Texture) noexcept { _Texture = Texture; }
//			private:
//				uint32_t Unk10[4];				// 10
//				BSFixedString _FileName;		// 20
//				char _Pad28[8];					// 28
//				NiTexture* _PrevTexture;		// 30
//				NiTexture* _NextTexture;		// 38
//				char _Pad30[8];					// 40
//				BSGraphics::Texture* _Texture;	// 48
//				char _Pad40[8];					// 50
//			};
//			static_assert(sizeof(NiTexture) == 0x58);
//		}
//
//		namespace Fallout4
//		{
//			using namespace EditorAPI::Fallout4;
//
//			class NiTexture : public NiObject
//			{
//			public:
//				inline BSFixedString GetFileNameFS() const noexcept { return _FileName; }
//				inline const char* GetFileName() const noexcept { return _FileName.c_str(); }
//				inline NiTexture* GetPrev() const noexcept { return _PrevTexture; }
//				inline NiTexture* GetNext() const noexcept { return _NextTexture; }
//				inline void* GetTexture() const noexcept { return _Texture; }
//				inline void SetTexture(void* Texture) noexcept { _Texture = Texture; }
//
//				inline static NiTexture* (*CreateTexture)(BSEntryString*, uint32_t, uint32_t);
//				inline static void (*CreateTexture2)(const char*, NiTexture*&, uint32_t);
//			private:
//				BSFixedString	_FileName;		// 10
//				char			_Pad18[8];		// 18
//				NiTexture*		_PrevTexture;	// 20
//				NiTexture*		_NextTexture;	// 28
//				char			_Pad30[8];		// 30
//				void*			_Texture;		// 38
//				char			_Pad40[8];		// 40
//			};
//			static_assert(sizeof(NiTexture) == 0x48);
//		}
//	}
//}

#pragma pack(pop)