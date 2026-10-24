// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkInfrastructure.h>

namespace CKPE::Fallout4::EditorAPI
{
	class hkResourceMap
	{
	public:
		virtual void hkResourceMap_Unk00();
		virtual ~hkResourceMap();
	};
	static_assert(sizeof(hkResourceMap) == 0x8);

	class hkContainerResourceMap : public hkResourceMap
	{
	public:
		/* 0x008 */ std::byte pad008[0x18];
	};
	static_assert(sizeof(hkContainerResourceMap) == 0x20);

	class hkCrc32StreamWriter : public hkStreamWriter
	{
	public:
		/* 0x010 */ std::byte pad010[0x8];
	};
	static_assert(sizeof(hkCrc32StreamWriter) == 0x18);

	class hkGeometryUtils
	{
	public:
		class IVertices
		{
		public:
			virtual ~IVertices();
			virtual void IVertices_Unk01();
			virtual void IVertices_Unk02();
			virtual void IVertices_Unk03();
		};
		static_assert(sizeof(IVertices) == 0x8);
	};

	class hkSignal0
	{
	public:
		class Slot : public hkSlot
		{
		public:
			virtual void hkSignal0Slot_Unk02();
		};
		static_assert(sizeof(Slot) == 0x18);
	};

	class hkTaskQueue : public hkReferencedObject
	{
	public:
		class EmptyTask : public hkTask {};
		static_assert(sizeof(EmptyTask) == 0x10);

		/* 0x010 */ std::byte pad010[0xD0];
	};
	static_assert(sizeof(hkTaskQueue) == 0xE0);

	class hkDebugDisplayProcess :
		public hkReferencedObject, public hkProcess, public hkDebugDisplayHandler {};
	static_assert(sizeof(hkDebugDisplayProcess) == 0x48);

	class hkInspectProcess : public hkReferencedObject, public hkProcess
	{
	public:
		/* 0x040 */ std::byte pad040[0x20];
	};
	static_assert(sizeof(hkInspectProcess) == 0x60);

	class hkRemoteObjectProcess : public hkReferencedObject, public hkProcess
	{
	public:
		/* 0x040 */ std::byte pad040[0x10];
	};
	static_assert(sizeof(hkRemoteObjectProcess) == 0x50);

	class hkServerDebugDisplayHandler :
		public hkReferencedObject, public hkProcess, public hkDebugDisplayHandler
	{
	public:
		/* 0x048 */ std::byte pad048[0x50];
	};
	static_assert(sizeof(hkServerDebugDisplayHandler) == 0x98);

	class hkServerProcessHandler :
		public hkReferencedObject, public hkProcessHandler, public hkProcess
	{
	public:
		/* 0x048 */ std::byte pad048[0x48];
	};
	static_assert(sizeof(hkServerProcessHandler) == 0x90);

	class hkStatisticsProcess : public hkReferencedObject, public hkProcess
	{
	public:
		/* 0x040 */ std::byte pad040[0x30];
	};
	static_assert(sizeof(hkStatisticsProcess) == 0x70);
}
