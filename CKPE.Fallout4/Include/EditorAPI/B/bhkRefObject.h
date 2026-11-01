// Copyright © 2020-2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/N/NiObject.h>

namespace CKPE::Fallout4::EditorAPI
{
	class hkReferencedObject;

	class bhkSerializable : public NiAPI::NiObject
	{
	public:
		/* 02 */ virtual NiAPI::NiRTTI* GetRTTI() const noexcept(true) override;
		/* 1B */ virtual void LoadBinary(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1C */ virtual void LinkObject(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1D */ virtual bool RegisterStreamables(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1E */ virtual void SaveBinary(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 20 */ virtual void GetViewerStringsArray(void* Strings) noexcept(true) override;
		/* 2A */ virtual void UnkSerializable2A();
		/* 2B */ virtual void UnkSerializable2B();
		/* 2C */ virtual void UnkSerializable2C();
		/* 2D */ virtual void UnkSerializable2D();
		/* 2E */ virtual void UnkSerializable2E();
		/* 2F */ virtual void UnkSerializable2F();
		/* 30 */ virtual void UnkSerializable30();

		/* 0x010 */ void* constructionInfo;
	};
	static_assert(sizeof(bhkSerializable) == 0x18);
	static_assert(offsetof(bhkSerializable, constructionInfo) == 0x10);

	class bhkRefObject : public bhkSerializable
	{
	public:
		/* 02 */ virtual NiAPI::NiRTTI* GetRTTI() const noexcept(true) override;
		/* 20 */ virtual void GetViewerStringsArray(void* Strings) noexcept(true) override;
		/* 31 */ virtual void SetReferencedObject(hkReferencedObject* Object);
		/* 32 */ virtual void UnkRefObject32();
		/* 33 */ virtual void UnkRefObject33();
		/* 34 */ virtual void UnkRefObject34();
		/* 35 */ virtual void UnkRefObject35();
		/* 36 */ virtual void UnkRefObject36();
		/* 37 */ virtual void UnkRefObject37();

		/* 0x018 */ hkReferencedObject* referencedObject;

		[[nodiscard]] inline hkReferencedObject* GetReferencedObject() const noexcept(true)
		{
			return referencedObject;
		}
	};
	static_assert(sizeof(bhkRefObject) == 0x20);
	static_assert(offsetof(bhkRefObject, referencedObject) == 0x18);
}
