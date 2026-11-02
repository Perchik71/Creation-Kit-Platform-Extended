// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkMemory.h>
#include <EditorAPI/H/hkReflectionTypes.h>
#include <cstdio>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE::Fallout4::EditorAPI
{
	template<class T>
	class hkSingleton {};
	static_assert(sizeof(hkSingleton<void>) == 0x1);

	class hkBaseSystem;

	class hkBallAndSocketConstraintCinfo;
	class hkBallSocketChainCinfo;
	class hkBinaryPackfileWriter;
	class hkBinaryTagfileReader;
	class hkBinaryTagfileWriter;
	class hkBlockStreamAllocator;
	class hkBlockStreamCommandWriter;
	class hkBreakableConstraintCinfo;
	class hkBsdSocket;
	class hkBufferedStreamReader;
	class hkBuiltinTypeRegistry;
	class hkClassNameRegistry;
	class hkCommandRouter;
	class hkConstraintChainCinfo;
	class hkConstraintCinfo;
	class hkDataArrayImpl;
	class hkDataArrayNative;
	class hkDataClassDict;
	class hkDataClassImpl;
	class hkDataClassNative;
	class hkDataObjectDict;
	class hkDataObjectImpl;
	class hkDataObjectNative;
	class hkDataRefCounted;
	class hkDataWorld;
	class hkDataWorldDict;
	class hkDataWorldNative;
	class hkDebugCommandProcessor;
	class hkDebugDisplay;
	class hkDebugDisplayHandler;
	class hkDefaultBuiltinTypeRegistry;
	class hkDefaultClassNameRegistry;
	class hkDefaultClassWrapper;
	class hkDefaultError;
	class hkDefaultImage;
	class hkDisplayAABB;
	class hkDisplayBox;
	class hkDisplayCapsule;
	class hkDisplayCone;
	class hkDisplayConvex;
	class hkDisplayCylinder;
	class hkDisplayGeometry;
	class hkDisplayGeometryBuilder;
	class hkDisplayMesh;
	class hkDisplayPlane;
	class hkDisplaySemiCircle;
	class hkDisplaySerializeIStream;
	class hkDisplaySerializeOStream;
	class hkDisplaySphere;
	class hkDisplayTaperedCapsule;
	class hkDisplayWireframe;
	class hkDrawUtil;
	class hkDummySingleton;
	class hkDynamicClassNameRegistry;
	class hkErrStream;
	class hkError;
	class hkFileSystem;
	class hkFixedBlockStreamAllocator;
	class hkFixedConstraintCinfo;
	class hkForwardingDisplayGeometryBuilder;
	class hkGenericConstraintCinfo;
	class hkHardwareInfo;
	class hkHingeConstraintCinfo;
	class hkHingeLimitsConstraintCinfo;
	class hkIArchive;
	class hkImage;
	class hkIstream;
	class hkLimitedHingeConstraintCinfo;
	class hkLoader;
	class hkLocalFrameCollector;
	class hkMalleableConstraintCinfo;
	class hkMallocAllocator;
	class hkMemoryStreamReader;
	class hkMemoryTrackStreamReader;
	class hkOArchive;
	class hkObjectCopier;
	class hkObjectResource;
	class hkObjectWriter;
	class hkOffsetOnlyStreamWriter;
	class hkPackfileData;
	class hkPackfileWriter;
	class hkParserBuffer;
	class hkPlatformObjectWriter;
	class hkPointToPathConstraintCinfo;
	class hkPrismaticConstraintCinfo;
	class hkProcess;
	class hkProcessContext;
	class hkProcessFactory;
	class hkProcessHandler;
	class hkPruneRedundantMaterialRegistry;
	class hkRagdollConstraintCinfo;
	class hkRagdollLimitsConstraintCinfo;
	class hkReferencedObjectLock;
	class hkRemoteObjectClientSideListener;
	class hkRemoteObjectServerSideListener;
	class hkReplayStreamReader;
	class hkResource;
	class hkSecondaryCommandDispatcher;
	class hkSerializeDeprecated;
	class hkSlot;
	class hkSocket;
	class hkSolverAllocator;
	class hkStackTracer;
	class hkStaticClassNameRegistry;
	class hkStdioStreamReader;
	class hkStiffSpringConstraintCinfo;
	class hkSubStreamWriter;
	class hkTagfileReader;
	class hkTagfileWriter;
	class hkTask;
	class hkThreadPool;
	class hkTraceStream;
	class hkTrackerArrayLayoutHandler;
	class hkTrackerEnumLayoutHandler;
	class hkTrackerFlagsLayoutHandler;
	class hkTrackerJobQueueDynamicDataLayoutHandler;
	class hkTrackerJobQueueLayoutHandler;
	class hkTrackerLayoutCalculator;
	class hkTrackerLayoutHandler;
	class hkTrackerPadSpuLayoutHandler;
	class hkTrackerPointerMapLayoutHandler;
	class hkTrackerQueueLayoutHandler;
	class hkTrackerRefPtrLayoutHandler;
	class hkTrackerScanSnapshot;
	class hkTrackerSetLayoutHandler;
	class hkTrackerStringMapLayoutHandler;
	class hkTrackerStringPtrLayoutHandler;
	class hkTrackerTypeLayout;
	class hkTrackerTypeTreeCache;
	class hkTypeInfoRegistry;
	class hkTypeManager;
	class hkUuidPseudoRandomGenerator;
	class hkVdbCommandWriter;
	class hkVersionPatchManager;
	class hkVisualDebugger;
	class hkVtableClassRegistry;
	class hkWheelConstraintCinfo;
	class hkWin32FileSystem;
	class hkWin32StreamWriter;
	class hkXmlLexAnalyzer;
	class hkXmlParser;
	class hkXmlStreamParser;
	class hkXmlTagfileReader;
	class hkXmlTagfileWriter;

	class hkConstraintCinfo
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkConstraintCinfo;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkConstraintCinfo[0];

		virtual ~hkConstraintCinfo();
		virtual void hkConstraintCinfo_Unk01();
		virtual void hkConstraintCinfo_Unk02();
		virtual void hkConstraintCinfo_Unk03();
		virtual void hkConstraintCinfo_Unk04();
		virtual void hkConstraintCinfo_Unk05();
		/* 0x008 */ std::byte pad008[0x20];
	};
	static_assert(sizeof(hkConstraintCinfo) == 0x28);

	class hkBallAndSocketConstraintCinfo : public hkConstraintCinfo
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkBallAndSocketConstraintCinfo;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkBallAndSocketConstraintCinfo[0];

	};
	static_assert(sizeof(hkBallAndSocketConstraintCinfo) == 0x28);

	class hkConstraintChainCinfo : public hkConstraintCinfo
	{
	public:
		/* 0x028 */ std::byte pad028[0x10];
	};
	static_assert(sizeof(hkConstraintChainCinfo) == 0x38);

	class hkBallSocketChainCinfo : public hkConstraintChainCinfo
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkBallSocketChainCinfo;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkBallSocketChainCinfo[0];

		/* 0x038 */ std::byte pad038[0x10];
	};
	static_assert(sizeof(hkBallSocketChainCinfo) == 0x48);

	class hkPackfileWriter : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkPackfileWriter;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkPackfileWriter[0];

		class AddObjectListener : public hkReferencedObject
		{
		public:
			virtual void AddObjectListener_Unk04();
		};
		static_assert(sizeof(AddObjectListener) == 0x10);

		virtual void hkPackfileWriter_Unk04();
		virtual void hkPackfileWriter_Unk05();
		virtual void hkPackfileWriter_Unk06();
		virtual void hkPackfileWriter_Unk07();
		virtual void hkPackfileWriter_Unk08();
		virtual void hkPackfileWriter_Unk09();
		virtual void hkPackfileWriter_Unk0A();
		virtual void hkPackfileWriter_Unk0B();
		/* 0x010 */ std::byte pad010[0x128];
	};
	static_assert(sizeof(hkPackfileWriter) == 0x138);

	class hkBinaryPackfileWriter : public hkPackfileWriter
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkBinaryPackfileWriter;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkBinaryPackfileWriter[0];

		/* 0x138 */ std::byte pad138[0x8];
	};
	static_assert(sizeof(hkBinaryPackfileWriter) == 0x140);

	class hkTagfileReader : public hkReferencedObject
	{
	public:
		virtual void hkTagfileReader_Unk04();
	};
	static_assert(sizeof(hkTagfileReader) == 0x10);

	class hkBinaryTagfileReader : public hkTagfileReader
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkBinaryTagfileReader;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkBinaryTagfileReader[0];

	};
	static_assert(sizeof(hkBinaryTagfileReader) == 0x10);

	class hkTagfileWriter : public hkReferencedObject
	{
	public:
		class AddDataObjectListener : public hkReferencedObject
		{
		public:
			virtual void AddDataObjectListener_Unk04();
		};
		static_assert(sizeof(AddDataObjectListener) == 0x10);
		virtual void hkTagfileWriter_Unk04();
	};
	static_assert(sizeof(hkTagfileWriter) == 0x10);

	class hkBinaryTagfileWriter : public hkTagfileWriter
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkBinaryTagfileWriter;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkBinaryTagfileWriter[0];

	};
	static_assert(sizeof(hkBinaryTagfileWriter) == 0x10);

	class hkBlockStreamAllocator : public hkReferencedObject
	{
	public:
		virtual void hkBlockStreamAllocator_Unk04();
		virtual void hkBlockStreamAllocator_Unk05();
		virtual void hkBlockStreamAllocator_Unk06();
		virtual void hkBlockStreamAllocator_Unk07();
		virtual void hkBlockStreamAllocator_Unk08();
		virtual void hkBlockStreamAllocator_Unk09();
		virtual void hkBlockStreamAllocator_Unk0A();
	};
	static_assert(sizeof(hkBlockStreamAllocator) == 0x10);

	class hkSecondaryCommandDispatcher : public hkReferencedObject
	{
	public:
		virtual void hkSecondaryCommandDispatcher_Unk04();
		virtual void hkSecondaryCommandDispatcher_Unk05();
	};
	static_assert(sizeof(hkSecondaryCommandDispatcher) == 0x10);

	class hkBlockStreamCommandWriter : public hkSecondaryCommandDispatcher
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkBlockStreamCommandWriter;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkBlockStreamCommandWriter[0];

		/* 0x010 */ std::byte pad010[0x28];
	};
	static_assert(sizeof(hkBlockStreamCommandWriter) == 0x38);

	class hkBreakableConstraintCinfo : public hkConstraintCinfo
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkBreakableConstraintCinfo;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkBreakableConstraintCinfo[0];

	};
	static_assert(sizeof(hkBreakableConstraintCinfo) == 0x28);

	class hkSocket : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkSocket;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkSocket[0];

		class ReaderAdapter : public hkStreamReader
		{
		public:
			constexpr static Common::ID RTTI = IDs::RTTI::hkSocket__ReaderAdapter;
			constexpr static Common::ID VTABLE = IDs::VTABLE::hkSocket__ReaderAdapter[0];

			/* 0x010 */ std::byte pad010[0x8];
		};
		static_assert(sizeof(ReaderAdapter) == 0x18);
		class WriterAdapter : public hkStreamWriter
		{
		public:
			constexpr static Common::ID RTTI = IDs::RTTI::hkSocket__WriterAdapter;
			constexpr static Common::ID VTABLE = IDs::VTABLE::hkSocket__WriterAdapter[0];

			/* 0x010 */ std::byte pad010[0x8];
		};
		static_assert(sizeof(WriterAdapter) == 0x18);
		virtual void hkSocket_Unk04();
		virtual void hkSocket_Unk05();
		virtual void hkSocket_Unk06();
		virtual void hkSocket_Unk07();
		virtual void hkSocket_Unk08();
		virtual void hkSocket_Unk09();
		virtual void hkSocket_Unk0A();
		virtual void hkSocket_Unk0B();
		virtual void hkSocket_Unk0C();
		virtual void hkSocket_Unk0D();
		/* 0x010 */ std::byte pad010[0x30];
	};
	static_assert(sizeof(hkSocket) == 0x40);

	class hkBsdSocket : public hkSocket
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkBsdSocket;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkBsdSocket[0];

		/* 0x040 */ std::byte pad040[0x8];
	};
	static_assert(sizeof(hkBsdSocket) == 0x48);

	class hkBufferedStreamReader : public hkSeekableStreamReader
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkBufferedStreamReader;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkBufferedStreamReader[0];

		/* 0x010 */ std::byte pad010[0x28];
	};
	static_assert(sizeof(hkBufferedStreamReader) == 0x38);

	class hkBuiltinTypeRegistry : public hkReferencedObject
	{
	public:
		virtual void hkBuiltinTypeRegistry_Unk04();
		virtual void hkBuiltinTypeRegistry_Unk05();
		virtual void hkBuiltinTypeRegistry_Unk06();
		virtual void hkBuiltinTypeRegistry_Unk07();
		virtual void hkBuiltinTypeRegistry_Unk08();
	};
	static_assert(sizeof(hkBuiltinTypeRegistry) == 0x10);

	class hkClassNameRegistry : public hkReferencedObject
	{
	public:
		virtual void hkClassNameRegistry_Unk04();
		virtual void hkClassNameRegistry_Unk05();
		virtual void hkClassNameRegistry_Unk06();
	};
	static_assert(sizeof(hkClassNameRegistry) == 0x10);

	class hkCommandRouter : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkCommandRouter;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkCommandRouter[0];

		/* 0x010 */ std::byte pad010[0x18];
	};
	static_assert(sizeof(hkCommandRouter) == 0x28);

	class hkDataRefCounted
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDataRefCounted;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDataRefCounted[0];

		virtual ~hkDataRefCounted();
		/* 0x008 */ std::byte pad008[0x8];
	};
	static_assert(sizeof(hkDataRefCounted) == 0x10);

	class hkDataArrayImpl : public hkDataRefCounted
	{
	public:
		virtual void hkDataArrayImpl_Unk01();
		virtual void hkDataArrayImpl_Unk02();
		virtual void hkDataArrayImpl_Unk03();
		virtual void hkDataArrayImpl_Unk04();
		virtual void hkDataArrayImpl_Unk05();
		virtual void hkDataArrayImpl_Unk06();
		virtual void hkDataArrayImpl_Unk07();
		virtual void hkDataArrayImpl_Unk08();
		virtual void hkDataArrayImpl_Unk09();
		virtual void hkDataArrayImpl_Unk0A();
		virtual void hkDataArrayImpl_Unk0B();
		virtual void hkDataArrayImpl_Unk0C();
		virtual void hkDataArrayImpl_Unk0D();
		virtual void hkDataArrayImpl_Unk0E();
		virtual void hkDataArrayImpl_Unk0F();
		virtual void hkDataArrayImpl_Unk10();
		virtual void hkDataArrayImpl_Unk11();
		virtual void hkDataArrayImpl_Unk12();
		virtual void hkDataArrayImpl_Unk13();
		virtual void hkDataArrayImpl_Unk14();
		virtual void hkDataArrayImpl_Unk15();
		virtual void hkDataArrayImpl_Unk16();
		virtual void hkDataArrayImpl_Unk17();
		virtual void hkDataArrayImpl_Unk18();
		virtual void hkDataArrayImpl_Unk19();
		virtual void hkDataArrayImpl_Unk1A();
		virtual void hkDataArrayImpl_Unk1B();
		virtual void hkDataArrayImpl_Unk1C();
		virtual void hkDataArrayImpl_Unk1D();
		virtual void hkDataArrayImpl_Unk1E();
		virtual void hkDataArrayImpl_Unk1F();
		virtual void hkDataArrayImpl_Unk20();
		virtual void hkDataArrayImpl_Unk21();
		virtual void hkDataArrayImpl_Unk22();
		virtual void hkDataArrayImpl_Unk23();
		virtual void hkDataArrayImpl_Unk24();
		virtual void hkDataArrayImpl_Unk25();
		virtual void hkDataArrayImpl_Unk26();
	};
	static_assert(sizeof(hkDataArrayImpl) == 0x10);

	class hkDataArrayNative : public hkDataArrayImpl
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDataArrayNative;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDataArrayNative[0];

		/* 0x010 */ std::byte pad010[0x40];
	};
	static_assert(sizeof(hkDataArrayNative) == 0x50);

	class hkDataClassImpl : public hkDataRefCounted
	{
	public:
		virtual void hkDataClassImpl_Unk01();
		virtual void hkDataClassImpl_Unk02();
		virtual void hkDataClassImpl_Unk03();
		virtual void hkDataClassImpl_Unk04();
		virtual void hkDataClassImpl_Unk05();
		virtual void hkDataClassImpl_Unk06();
		virtual void hkDataClassImpl_Unk07();
		virtual void hkDataClassImpl_Unk08();
		virtual void hkDataClassImpl_Unk09();
		virtual void hkDataClassImpl_Unk0A();
		virtual void hkDataClassImpl_Unk0B();
		virtual void hkDataClassImpl_Unk0C();
	};
	static_assert(sizeof(hkDataClassImpl) == 0x10);

	class hkDataClassDict : public hkDataClassImpl
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDataClassDict;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDataClassDict[0];

		/* 0x010 */ std::byte pad010[0x50];
	};
	static_assert(sizeof(hkDataClassDict) == 0x60);

	class hkDataClassNative : public hkDataClassImpl
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDataClassNative;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDataClassNative[0];

		/* 0x010 */ std::byte pad010[0x10];
	};
	static_assert(sizeof(hkDataClassNative) == 0x20);

	class hkDataObjectImpl : public hkDataRefCounted
	{
	public:
		virtual void hkDataObjectImpl_Unk01();
		virtual void hkDataObjectImpl_Unk02();
		virtual void hkDataObjectImpl_Unk03();
		virtual void hkDataObjectImpl_Unk04();
		virtual void hkDataObjectImpl_Unk05();
		virtual void hkDataObjectImpl_Unk06();
		virtual void hkDataObjectImpl_Unk07();
		virtual void hkDataObjectImpl_Unk08();
		virtual void hkDataObjectImpl_Unk09();
		virtual void hkDataObjectImpl_Unk0A();
		virtual void hkDataObjectImpl_Unk0B();
		virtual void hkDataObjectImpl_Unk0C();
		virtual void hkDataObjectImpl_Unk0D();
		virtual void hkDataObjectImpl_Unk0E();
		virtual void hkDataObjectImpl_Unk0F();
		virtual void hkDataObjectImpl_Unk10();
		virtual void hkDataObjectImpl_Unk11();
		virtual void hkDataObjectImpl_Unk12();
		virtual void hkDataObjectImpl_Unk13();
		virtual void hkDataObjectImpl_Unk14();
		virtual void hkDataObjectImpl_Unk15();
		virtual void hkDataObjectImpl_Unk16();
		virtual void hkDataObjectImpl_Unk17();
		virtual void hkDataObjectImpl_Unk18();
		virtual void hkDataObjectImpl_Unk19();
		virtual void hkDataObjectImpl_Unk1A();
		virtual void hkDataObjectImpl_Unk1B();
		virtual void hkDataObjectImpl_Unk1C();
		virtual void hkDataObjectImpl_Unk1D();
		virtual void hkDataObjectImpl_Unk1E();
	};
	static_assert(sizeof(hkDataObjectImpl) == 0x10);

	class hkDataObjectDict : public hkDataObjectImpl
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDataObjectDict;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDataObjectDict[0];

		/* 0x010 */ std::byte pad010[0x20];
	};
	static_assert(sizeof(hkDataObjectDict) == 0x30);

	class hkDataObjectNative : public hkDataObjectImpl
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDataObjectNative;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDataObjectNative[0];

		/* 0x010 */ std::byte pad010[0x20];
	};
	static_assert(sizeof(hkDataObjectNative) == 0x30);

	class hkDataWorld : public hkReferencedObject
	{
	public:
		virtual void hkDataWorld_Unk04();
		virtual void hkDataWorld_Unk05();
		virtual void hkDataWorld_Unk06();
		virtual void hkDataWorld_Unk07();
		virtual void hkDataWorld_Unk08();
		virtual void hkDataWorld_Unk09();
		virtual void hkDataWorld_Unk0A();
		virtual void hkDataWorld_Unk0B();
		virtual void hkDataWorld_Unk0C();
		virtual void hkDataWorld_Unk0D();
		virtual void hkDataWorld_Unk0E();
		virtual void hkDataWorld_Unk0F();
		virtual void hkDataWorld_Unk10();
		virtual void hkDataWorld_Unk11();
		virtual void hkDataWorld_Unk12();
		virtual void hkDataWorld_Unk13();
		virtual void hkDataWorld_Unk14();
		virtual void hkDataWorld_Unk15();
		virtual void hkDataWorld_Unk16();
		virtual void hkDataWorld_Unk17();
		virtual void hkDataWorld_Unk18();
	};
	static_assert(sizeof(hkDataWorld) == 0x10);

	class hkDataWorldDict : public hkDataWorld
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDataWorldDict;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDataWorldDict[0];

		/* 0x010 */ std::byte pad010[0x110];
	};
	static_assert(sizeof(hkDataWorldDict) == 0x120);

	class hkDataWorldNative : public hkDataWorld
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDataWorldNative;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDataWorldNative[0];

		virtual void hkDataWorldNative_Unk19();
		/* 0x010 */ std::byte pad010[0x158];
	};
	static_assert(sizeof(hkDataWorldNative) == 0x168);

	class hkDebugCommandProcessor : public hkSecondaryCommandDispatcher
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDebugCommandProcessor;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDebugCommandProcessor[0];

	};
	static_assert(sizeof(hkDebugCommandProcessor) == 0x10);

	class hkDebugDisplay : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDebugDisplay;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDebugDisplay[0];

		/* 0x010 */ std::byte pad010[0x18];
	};
	static_assert(sizeof(hkDebugDisplay) == 0x28);

	class hkDebugDisplayHandler
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDebugDisplayHandler;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDebugDisplayHandler[0];

		virtual ~hkDebugDisplayHandler();
		virtual void hkDebugDisplayHandler_Unk01();
		virtual void hkDebugDisplayHandler_Unk02();
		virtual void hkDebugDisplayHandler_Unk03();
		virtual void hkDebugDisplayHandler_Unk04();
		virtual void hkDebugDisplayHandler_Unk05();
		virtual void hkDebugDisplayHandler_Unk06();
		virtual void hkDebugDisplayHandler_Unk07();
		virtual void hkDebugDisplayHandler_Unk08();
		virtual void hkDebugDisplayHandler_Unk09();
		virtual void hkDebugDisplayHandler_Unk0A();
		virtual void hkDebugDisplayHandler_Unk0B();
		virtual void hkDebugDisplayHandler_Unk0C();
		virtual void hkDebugDisplayHandler_Unk0D();
		virtual void hkDebugDisplayHandler_Unk0E();
		virtual void hkDebugDisplayHandler_Unk0F();
		virtual void hkDebugDisplayHandler_Unk10();
		virtual void hkDebugDisplayHandler_Unk11();
		virtual void hkDebugDisplayHandler_Unk12();
		virtual void hkDebugDisplayHandler_Unk13();
		virtual void hkDebugDisplayHandler_Unk14();
		virtual void hkDebugDisplayHandler_Unk15();
		virtual void hkDebugDisplayHandler_Unk16();
		virtual void hkDebugDisplayHandler_Unk17();
		virtual void hkDebugDisplayHandler_Unk18();
		virtual void hkDebugDisplayHandler_Unk19();
		virtual void hkDebugDisplayHandler_Unk1A();
		virtual void hkDebugDisplayHandler_Unk1B();
		virtual void hkDebugDisplayHandler_Unk1C();
		virtual void hkDebugDisplayHandler_Unk1D();
		virtual void hkDebugDisplayHandler_Unk1E();
		virtual void hkDebugDisplayHandler_Unk1F();
		virtual void hkDebugDisplayHandler_Unk20();
		virtual void hkDebugDisplayHandler_Unk21();
		virtual void hkDebugDisplayHandler_Unk22();
		virtual void hkDebugDisplayHandler_Unk23();
		virtual void hkDebugDisplayHandler_Unk24();
	};
	static_assert(sizeof(hkDebugDisplayHandler) == 0x8);

	class hkDefaultBuiltinTypeRegistry : public hkBuiltinTypeRegistry
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDefaultBuiltinTypeRegistry;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDefaultBuiltinTypeRegistry[0];

	};
	static_assert(sizeof(hkDefaultBuiltinTypeRegistry) == 0x10);

	class hkDynamicClassNameRegistry : public hkClassNameRegistry
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDynamicClassNameRegistry;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDynamicClassNameRegistry[0];

		virtual void hkDynamicClassNameRegistry_Unk07();
		virtual void hkDynamicClassNameRegistry_Unk08();
		virtual void hkDynamicClassNameRegistry_Unk09();
		virtual void hkDynamicClassNameRegistry_Unk0A();
		virtual void hkDynamicClassNameRegistry_Unk0B();
		/* 0x010 */ std::byte pad010[0x20];
	};
	static_assert(sizeof(hkDynamicClassNameRegistry) == 0x30);

	class hkDefaultClassNameRegistry : public hkDynamicClassNameRegistry
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDefaultClassNameRegistry;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDefaultClassNameRegistry[0];

		/* 0x030 */ std::byte pad030[0x8];
	};
	static_assert(sizeof(hkDefaultClassNameRegistry) == 0x38);

	class hkVersionPatchManager : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkVersionPatchManager;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkVersionPatchManager[0];

		class ClassWrapper : public hkReferencedObject
		{
		public:
			virtual void ClassWrapper_Unk04();
		};
		static_assert(sizeof(ClassWrapper) == 0x10);
		/* 0x010 */ std::byte pad010[0x50];
	};
	static_assert(sizeof(hkVersionPatchManager) == 0x60);

	class hkDefaultClassWrapper : public hkVersionPatchManager::ClassWrapper
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDefaultClassWrapper;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDefaultClassWrapper[0];

		/* 0x010 */ std::byte pad010[0x8];
	};
	static_assert(sizeof(hkDefaultClassWrapper) == 0x18);

	class hkError : public hkReferencedObject
	{
	public:
		virtual void hkError_Unk04();
		virtual void hkError_Unk05();
		virtual void hkError_Unk06();
		virtual void hkError_Unk07();
		virtual void hkError_Unk08();
		virtual void hkError_Unk09();
		virtual void hkError_Unk0A();
		virtual void hkError_Unk0B();
	};
	static_assert(sizeof(hkError) == 0x10);

	class hkDefaultError : public hkError
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDefaultError;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDefaultError[0];

		virtual void hkDefaultError_Unk0C();
		/* 0x010 */ std::byte pad010[0x38];
	};
	static_assert(sizeof(hkDefaultError) == 0x48);

	class hkImage : public hkReferencedObject
	{
	public:
		virtual void hkImage_Unk04();
		virtual void hkImage_Unk05();
	};
	static_assert(sizeof(hkImage) == 0x10);

	class hkDefaultImage : public hkImage
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDefaultImage;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDefaultImage[0];

		/* 0x010 */ std::byte pad010[0x28];
	};
	static_assert(sizeof(hkDefaultImage) == 0x38);

	class hkDisplayGeometry : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDisplayGeometry;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDisplayGeometry[0];

		virtual void hkDisplayGeometry_Unk04();
		virtual void hkDisplayGeometry_Unk05();
		/* 0x010 */ std::byte pad010[0x60];
	};
	static_assert(sizeof(hkDisplayGeometry) == 0x70);

	class hkDisplayAABB : public hkDisplayGeometry
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDisplayAABB;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDisplayAABB[0];

		/* 0x070 */ std::byte pad070[0x20];
	};
	static_assert(sizeof(hkDisplayAABB) == 0x90);

	class hkDisplayBox : public hkDisplayGeometry
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDisplayBox;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDisplayBox[0];

		/* 0x070 */ std::byte pad070[0x10];
	};
	static_assert(sizeof(hkDisplayBox) == 0x80);

	class hkDisplayCapsule : public hkDisplayGeometry
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDisplayCapsule;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDisplayCapsule[0];

		/* 0x070 */ std::byte pad070[0x30];
	};
	static_assert(sizeof(hkDisplayCapsule) == 0xA0);

	class hkDisplayCone : public hkDisplayGeometry
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDisplayCone;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDisplayCone[0];

		/* 0x070 */ std::byte pad070[0x30];
	};
	static_assert(sizeof(hkDisplayCone) == 0xA0);

	class hkDisplayConvex : public hkDisplayGeometry
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDisplayConvex;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDisplayConvex[0];

	};
	static_assert(sizeof(hkDisplayConvex) == 0x70);

	class hkDisplayCylinder : public hkDisplayGeometry
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDisplayCylinder;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDisplayCylinder[0];

		/* 0x070 */ std::byte pad070[0x30];
	};
	static_assert(sizeof(hkDisplayCylinder) == 0xA0);

	class hkDisplayGeometryBuilder : public hkReferencedObject
	{
	public:
		virtual void hkDisplayGeometryBuilder_Unk04();
		virtual void hkDisplayGeometryBuilder_Unk05();
		virtual void hkDisplayGeometryBuilder_Unk06();
	};
	static_assert(sizeof(hkDisplayGeometryBuilder) == 0x10);

	class hkDisplayMesh : public hkDisplayGeometry
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDisplayMesh;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDisplayMesh[0];

		/* 0x070 */ std::byte pad070[0x20];
	};
	static_assert(sizeof(hkDisplayMesh) == 0x90);

	class hkDisplayPlane : public hkDisplayGeometry
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDisplayPlane;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDisplayPlane[0];

		/* 0x070 */ std::byte pad070[0x40];
	};
	static_assert(sizeof(hkDisplayPlane) == 0xB0);

	class hkDisplaySemiCircle : public hkDisplayGeometry
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDisplaySemiCircle;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDisplaySemiCircle[0];

		/* 0x070 */ std::byte pad070[0x40];
	};
	static_assert(sizeof(hkDisplaySemiCircle) == 0xB0);

	class hkIArchive : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkIArchive;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkIArchive[0];

		/* 0x010 */ std::byte pad010[0x10];
	};
	static_assert(sizeof(hkIArchive) == 0x20);

	class hkDisplaySerializeIStream : public hkIArchive
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDisplaySerializeIStream;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDisplaySerializeIStream[0];

	};
	static_assert(sizeof(hkDisplaySerializeIStream) == 0x20);

	class hkOArchive : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkOArchive;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkOArchive[0];

		/* 0x010 */ std::byte pad010[0x10];
	};
	static_assert(sizeof(hkOArchive) == 0x20);

	class hkDisplaySerializeOStream : public hkOArchive
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDisplaySerializeOStream;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDisplaySerializeOStream[0];

	};
	static_assert(sizeof(hkDisplaySerializeOStream) == 0x20);

	class hkDisplaySphere : public hkDisplayGeometry
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDisplaySphere;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDisplaySphere[0];

		/* 0x070 */ std::byte pad070[0x20];
	};
	static_assert(sizeof(hkDisplaySphere) == 0x90);

	class hkDisplayTaperedCapsule : public hkDisplayGeometry
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDisplayTaperedCapsule;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDisplayTaperedCapsule[0];

		/* 0x070 */ std::byte pad070[0x30];
	};
	static_assert(sizeof(hkDisplayTaperedCapsule) == 0xA0);

	class hkDisplayWireframe : public hkDisplayGeometry
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDisplayWireframe;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDisplayWireframe[0];

		/* 0x070 */ std::byte pad070[0x10];
	};
	static_assert(sizeof(hkDisplayWireframe) == 0x80);

	class hkDrawUtil : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDrawUtil;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDrawUtil[0];

		/* 0x010 */ std::byte pad010[0x8];
	};
	static_assert(sizeof(hkDrawUtil) == 0x18);

	class hkDummySingleton : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDummySingleton;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDummySingleton[0];

		virtual void hkDummySingleton_Unk04();
	};
	static_assert(sizeof(hkDummySingleton) == 0x10);

	class hkErrStream : public hkOstream
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkErrStream;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkErrStream[0];

	};
	static_assert(sizeof(hkErrStream) == 0x18);

	class hkFileSystem : public hkReferencedObject
	{
	public:
		class Iterator
		{
		public:
		class Impl : public hkReferencedObject
		{
		public:
			virtual void Impl_Unk04();
		};
		static_assert(sizeof(Impl) == 0x10);
		};
		virtual void hkFileSystem_Unk04();
		virtual void hkFileSystem_Unk05();
		virtual void hkFileSystem_Unk06();
		virtual void hkFileSystem_Unk07();
		virtual void hkFileSystem_Unk08();
		virtual void hkFileSystem_Unk09();
	};
	static_assert(sizeof(hkFileSystem) == 0x10);

	class hkFixedBlockStreamAllocator : public hkBlockStreamAllocator
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkFixedBlockStreamAllocator;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkFixedBlockStreamAllocator[0];

		/* 0x010 */ std::byte pad010[0x60];
	};
	static_assert(sizeof(hkFixedBlockStreamAllocator) == 0x70);

	class hkGenericConstraintCinfo : public hkConstraintCinfo
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkGenericConstraintCinfo;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkGenericConstraintCinfo[0];

	};
	static_assert(sizeof(hkGenericConstraintCinfo) == 0x28);

	class hkFixedConstraintCinfo : public hkGenericConstraintCinfo
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkFixedConstraintCinfo;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkFixedConstraintCinfo[0];

		/* 0x028 */ std::byte pad028[0x10];
	};
	static_assert(sizeof(hkFixedConstraintCinfo) == 0x38);

	class hkForwardingDisplayGeometryBuilder : public hkDisplayGeometryBuilder
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkForwardingDisplayGeometryBuilder;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkForwardingDisplayGeometryBuilder[0];

	};
	static_assert(sizeof(hkForwardingDisplayGeometryBuilder) == 0x10);

	class hkHardwareInfo : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkHardwareInfo;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkHardwareInfo[0];

		/* 0x010 */ std::byte pad010[0x8];
	};
	static_assert(sizeof(hkHardwareInfo) == 0x18);

	class hkHingeConstraintCinfo : public hkConstraintCinfo
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkHingeConstraintCinfo;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkHingeConstraintCinfo[0];

	};
	static_assert(sizeof(hkHingeConstraintCinfo) == 0x28);

	class hkHingeLimitsConstraintCinfo : public hkConstraintCinfo
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkHingeLimitsConstraintCinfo;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkHingeLimitsConstraintCinfo[0];

	};
	static_assert(sizeof(hkHingeLimitsConstraintCinfo) == 0x28);

	class hkIstream : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkIstream;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkIstream[0];

		/* 0x010 */ std::byte pad010[0x8];
	};
	static_assert(sizeof(hkIstream) == 0x18);

	class hkLimitedHingeConstraintCinfo : public hkConstraintCinfo
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkLimitedHingeConstraintCinfo;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkLimitedHingeConstraintCinfo[0];

	};
	static_assert(sizeof(hkLimitedHingeConstraintCinfo) == 0x28);

	class hkLoader : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkLoader;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkLoader[0];

		virtual void hkLoader_Unk04();
		virtual void hkLoader_Unk05();
		virtual void hkLoader_Unk06();
		virtual void hkLoader_Unk07();
		virtual void hkLoader_Unk08();
		virtual void hkLoader_Unk09();
		virtual void hkLoader_Unk0A();
		virtual void hkLoader_Unk0B();
		/* 0x010 */ std::byte pad010[0x10];
	};
	static_assert(sizeof(hkLoader) == 0x20);

	class hkLocalFrameCollector : public hkReferencedObject
	{
	public:
		virtual void hkLocalFrameCollector_Unk04();
	};
	static_assert(sizeof(hkLocalFrameCollector) == 0x10);

	class hkMalleableConstraintCinfo : public hkConstraintCinfo
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkMalleableConstraintCinfo;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkMalleableConstraintCinfo[0];

	};
	static_assert(sizeof(hkMalleableConstraintCinfo) == 0x28);

	class hkMallocAllocator : public hkMemoryAllocator
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkMallocAllocator;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkMallocAllocator[0];

		/* 0x008 */ std::byte pad008[0x10];
	};
	static_assert(sizeof(hkMallocAllocator) == 0x18);

	class hkMemoryStreamReader : public hkSeekableStreamReader
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkMemoryStreamReader;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkMemoryStreamReader[0];

		/* 0x010 */ const std::byte* m_buffer;
		/* 0x018 */ std::int32_t m_position;
		/* 0x01C */ std::int32_t m_size;
		/* 0x020 */ std::byte pad020[0x4];
		/* 0x024 */ hkBool m_error;
		/* 0x025 */ std::byte pad025[0x3];
	};
	static_assert(sizeof(hkMemoryStreamReader) == 0x28);

	static_assert(offsetof(hkMemoryStreamReader, m_buffer) == 0x10);
	static_assert(offsetof(hkMemoryStreamReader, m_position) == 0x18);
	static_assert(offsetof(hkMemoryStreamReader, m_size) == 0x1C);
	static_assert(offsetof(hkMemoryStreamReader, m_error) == 0x24);
	class hkMemoryTrackStreamReader : public hkStreamReader
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkMemoryTrackStreamReader;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkMemoryTrackStreamReader[0];

		/* 0x010 */ std::byte pad010[0x18];
	};
	static_assert(sizeof(hkMemoryTrackStreamReader) == 0x28);

	class hkObjectCopier : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkObjectCopier;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkObjectCopier[0];

		virtual void hkObjectCopier_Unk04();
		virtual void hkObjectCopier_Unk05();
		virtual void hkObjectCopier_Unk06();
		/* 0x010 */ std::byte pad010[0x10];
	};
	static_assert(sizeof(hkObjectCopier) == 0x20);

	class hkResource : public hkReferencedObject
	{
	public:
		virtual void hkResource_Unk04();
		virtual void hkResource_Unk05();
		virtual void hkResource_Unk06();
		virtual void hkResource_Unk07();
		virtual void hkResource_Unk08();
	};
	static_assert(sizeof(hkResource) == 0x10);

	class hkObjectResource : public hkResource
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkObjectResource;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkObjectResource[0];

		virtual void hkObjectResource_Unk09();
		/* 0x010 */ std::byte pad010[0x20];
	};
	static_assert(sizeof(hkObjectResource) == 0x30);

	class hkObjectWriter : public hkReferencedObject
	{
	public:
		virtual void hkObjectWriter_Unk04();
		virtual void hkObjectWriter_Unk05();
	};
	static_assert(sizeof(hkObjectWriter) == 0x10);

	class hkOffsetOnlyStreamWriter : public hkStreamWriter
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkOffsetOnlyStreamWriter;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkOffsetOnlyStreamWriter[0];

		/* 0x010 */ std::byte pad010[0x8];
	};
	static_assert(sizeof(hkOffsetOnlyStreamWriter) == 0x18);

	class hkPackfileData : public hkResource
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkPackfileData;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkPackfileData[0];

		/* 0x010 */ std::byte pad010[0x98];
	};
	static_assert(sizeof(hkPackfileData) == 0xA8);

	class hkParserBuffer : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkParserBuffer;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkParserBuffer[0];

		/* 0x010 */ std::byte pad010[0x30];
	};
	static_assert(sizeof(hkParserBuffer) == 0x40);

	class hkPlatformObjectWriter : public hkObjectWriter
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkPlatformObjectWriter;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkPlatformObjectWriter[0];

		class Cache : public hkReferencedObject
		{
		public:
			constexpr static Common::ID RTTI = IDs::RTTI::hkPlatformObjectWriter__Cache;
			constexpr static Common::ID VTABLE = IDs::VTABLE::hkPlatformObjectWriter__Cache[0];

			/* 0x010 */ std::byte pad010[0x30];
		};
		static_assert(sizeof(Cache) == 0x40);
		/* 0x010 */ std::byte pad010[0x10];
	};
	static_assert(sizeof(hkPlatformObjectWriter) == 0x20);

	class hkPointToPathConstraintCinfo : public hkConstraintCinfo
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkPointToPathConstraintCinfo;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkPointToPathConstraintCinfo[0];

	};
	static_assert(sizeof(hkPointToPathConstraintCinfo) == 0x28);

	class hkPrismaticConstraintCinfo : public hkConstraintCinfo
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkPrismaticConstraintCinfo;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkPrismaticConstraintCinfo[0];

	};
	static_assert(sizeof(hkPrismaticConstraintCinfo) == 0x28);

	class hkProcess
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkProcess;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkProcess[0];

		virtual ~hkProcess();
		virtual void hkProcess_Unk01();
		virtual void hkProcess_Unk02();
		virtual void hkProcess_Unk03();
		virtual void hkProcess_Unk04();
		virtual void hkProcess_Unk05();
		virtual void hkProcess_Unk06();
		virtual void hkProcess_Unk07();
		/* 0x008 */ std::byte pad008[0x28];
	};
	static_assert(sizeof(hkProcess) == 0x30);

	class hkProcessContext
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkProcessContext;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkProcessContext[0];

		virtual ~hkProcessContext();
		virtual void hkProcessContext_Unk01();
		virtual void hkProcessContext_Unk02();
		/* 0x008 */ std::byte pad008[0x88];
	};
	static_assert(sizeof(hkProcessContext) == 0x90);

	class hkProcessFactory : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkProcessFactory;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkProcessFactory[0];

		/* 0x010 */ std::byte pad010[0x20];
	};
	static_assert(sizeof(hkProcessFactory) == 0x30);

	class hkProcessHandler
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkProcessHandler;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkProcessHandler[0];

		virtual ~hkProcessHandler();
		virtual void hkProcessHandler_Unk01();
		virtual void hkProcessHandler_Unk02();
		virtual void hkProcessHandler_Unk03();
		virtual void hkProcessHandler_Unk04();
		virtual void hkProcessHandler_Unk05();
		virtual void hkProcessHandler_Unk06();
		virtual void hkProcessHandler_Unk07();
		virtual void hkProcessHandler_Unk08();
	};
	static_assert(sizeof(hkProcessHandler) == 0x8);

	class hkPruneRedundantMaterialRegistry : public hkDefaultMeshMaterialRegistry
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkPruneRedundantMaterialRegistry;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkPruneRedundantMaterialRegistry[0];

		/* 0x020 */ std::byte pad020[0x10];
	};
	static_assert(sizeof(hkPruneRedundantMaterialRegistry) == 0x30);

	class hkRagdollConstraintCinfo : public hkConstraintCinfo
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkRagdollConstraintCinfo;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkRagdollConstraintCinfo[0];

	};
	static_assert(sizeof(hkRagdollConstraintCinfo) == 0x28);

	class hkRagdollLimitsConstraintCinfo : public hkConstraintCinfo
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkRagdollLimitsConstraintCinfo;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkRagdollLimitsConstraintCinfo[0];

	};
	static_assert(sizeof(hkRagdollLimitsConstraintCinfo) == 0x28);

	class hkReferencedObjectLock : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkReferencedObjectLock;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkReferencedObjectLock[0];

		/* 0x010 */ std::byte pad010[0x40];
	};
	static_assert(sizeof(hkReferencedObjectLock) == 0x50);

	class hkRemoteObjectClientSideListener
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkRemoteObjectClientSideListener;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkRemoteObjectClientSideListener[0];

		virtual ~hkRemoteObjectClientSideListener();
		virtual void hkRemoteObjectClientSideListener_Unk01();
		/* 0x008 */ std::byte pad008[0x8];
	};
	static_assert(sizeof(hkRemoteObjectClientSideListener) == 0x10);

	class hkRemoteObjectServerSideListener
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkRemoteObjectServerSideListener;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkRemoteObjectServerSideListener[0];

		virtual ~hkRemoteObjectServerSideListener();
		virtual void hkRemoteObjectServerSideListener_Unk01();
	};
	static_assert(sizeof(hkRemoteObjectServerSideListener) == 0x8);

	class hkReplayStreamReader : public hkStreamReader
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkReplayStreamReader;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkReplayStreamReader[0];

		/* 0x010 */ std::byte pad010[0x20];
	};
	static_assert(sizeof(hkReplayStreamReader) == 0x30);

	class hkSerializeDeprecated : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkSerializeDeprecated;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkSerializeDeprecated[0];

		virtual void hkSerializeDeprecated_Unk04();
		virtual void hkSerializeDeprecated_Unk05();
		virtual void hkSerializeDeprecated_Unk06();
		virtual void hkSerializeDeprecated_Unk07();
		virtual void hkSerializeDeprecated_Unk08();
		virtual void hkSerializeDeprecated_Unk09();
	};
	static_assert(sizeof(hkSerializeDeprecated) == 0x10);

	class hkSlot
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkSlot;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkSlot[0];

		virtual ~hkSlot();
		virtual void hkSlot_Unk01();
		/* 0x008 */ std::byte pad008[0x10];
	};
	static_assert(sizeof(hkSlot) == 0x18);

	class hkSolverAllocator : public hkMemoryAllocator
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkSolverAllocator;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkSolverAllocator[0];

		/* 0x008 */ std::byte pad008[0x460];
	};
	static_assert(sizeof(hkSolverAllocator) == 0x468);

	class hkStackTracer
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkStackTracer;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkStackTracer[0];

		virtual ~hkStackTracer();
	};
	static_assert(sizeof(hkStackTracer) == 0x8);

	class hkStaticClassNameRegistry : public hkClassNameRegistry
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkStaticClassNameRegistry;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkStaticClassNameRegistry[0];

		/* 0x010 */ std::byte pad010[0x18];
	};
	static_assert(sizeof(hkStaticClassNameRegistry) == 0x28);

	class hkStdioStreamReader : public hkSeekableStreamReader
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkStdioStreamReader;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkStdioStreamReader[0];

		/* 0x010 */ std::FILE* m_file;
		/* 0x018 */ hkBool m_isOk;
		/* 0x019 */ std::byte pad019[0x7];
	};
	static_assert(sizeof(hkStdioStreamReader) == 0x20);

	static_assert(offsetof(hkStdioStreamReader, m_file) == 0x10);
	static_assert(offsetof(hkStdioStreamReader, m_isOk) == 0x18);
	class hkStiffSpringConstraintCinfo : public hkConstraintCinfo
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkStiffSpringConstraintCinfo;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkStiffSpringConstraintCinfo[0];

	};
	static_assert(sizeof(hkStiffSpringConstraintCinfo) == 0x28);

	class hkSubStreamWriter : public hkStreamWriter
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkSubStreamWriter;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkSubStreamWriter[0];

		/* 0x010 */ std::byte pad010[0x10];
	};
	static_assert(sizeof(hkSubStreamWriter) == 0x20);

	class hkTask : public hkReferencedObject
	{
	public:
		virtual void hkTask_Unk04();
		virtual void hkTask_Unk05();
		virtual void hkTask_Unk06();
	};
	static_assert(sizeof(hkTask) == 0x10);

	class hkThreadPool : public hkReferencedObject
	{
	public:
		virtual void hkThreadPool_Unk04();
		virtual void hkThreadPool_Unk05();
		virtual void hkThreadPool_Unk06();
		virtual void hkThreadPool_Unk07();
		virtual void hkThreadPool_Unk08();
		virtual void hkThreadPool_Unk09();
		virtual void hkThreadPool_Unk0A();
		virtual void hkThreadPool_Unk0B();
		virtual void hkThreadPool_Unk0C();
	};
	static_assert(sizeof(hkThreadPool) == 0x10);

	class hkTraceStream : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkTraceStream;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkTraceStream[0];

		/* 0x010 */ std::byte pad010[0x20];
	};
	static_assert(sizeof(hkTraceStream) == 0x30);

	class hkTrackerLayoutHandler : public hkReferencedObject
	{
	public:
		virtual void hkTrackerLayoutHandler_Unk04();
		virtual void hkTrackerLayoutHandler_Unk05();
	};
	static_assert(sizeof(hkTrackerLayoutHandler) == 0x10);

	class hkTrackerArrayLayoutHandler : public hkTrackerLayoutHandler
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkTrackerArrayLayoutHandler;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkTrackerArrayLayoutHandler[0];

	};
	static_assert(sizeof(hkTrackerArrayLayoutHandler) == 0x10);

	class hkTrackerEnumLayoutHandler : public hkTrackerLayoutHandler
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkTrackerEnumLayoutHandler;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkTrackerEnumLayoutHandler[0];

	};
	static_assert(sizeof(hkTrackerEnumLayoutHandler) == 0x10);

	class hkTrackerFlagsLayoutHandler : public hkTrackerLayoutHandler
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkTrackerFlagsLayoutHandler;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkTrackerFlagsLayoutHandler[0];

	};
	static_assert(sizeof(hkTrackerFlagsLayoutHandler) == 0x10);

	class hkTrackerJobQueueDynamicDataLayoutHandler : public hkTrackerLayoutHandler
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkTrackerJobQueueDynamicDataLayoutHandler;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkTrackerJobQueueDynamicDataLayoutHandler[0];

	};
	static_assert(sizeof(hkTrackerJobQueueDynamicDataLayoutHandler) == 0x10);

	class hkTrackerJobQueueLayoutHandler : public hkTrackerLayoutHandler
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkTrackerJobQueueLayoutHandler;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkTrackerJobQueueLayoutHandler[0];

	};
	static_assert(sizeof(hkTrackerJobQueueLayoutHandler) == 0x10);

	class hkTrackerLayoutCalculator : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkTrackerLayoutCalculator;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkTrackerLayoutCalculator[0];

		/* 0x010 */ std::byte pad010[0x50];
	};
	static_assert(sizeof(hkTrackerLayoutCalculator) == 0x60);

	class hkTrackerPadSpuLayoutHandler : public hkTrackerLayoutHandler
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkTrackerPadSpuLayoutHandler;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkTrackerPadSpuLayoutHandler[0];

	};
	static_assert(sizeof(hkTrackerPadSpuLayoutHandler) == 0x10);

	class hkTrackerPointerMapLayoutHandler : public hkTrackerLayoutHandler
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkTrackerPointerMapLayoutHandler;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkTrackerPointerMapLayoutHandler[0];

	};
	static_assert(sizeof(hkTrackerPointerMapLayoutHandler) == 0x10);

	class hkTrackerQueueLayoutHandler : public hkTrackerLayoutHandler
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkTrackerQueueLayoutHandler;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkTrackerQueueLayoutHandler[0];

	};
	static_assert(sizeof(hkTrackerQueueLayoutHandler) == 0x10);

	class hkTrackerRefPtrLayoutHandler : public hkTrackerLayoutHandler
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkTrackerRefPtrLayoutHandler;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkTrackerRefPtrLayoutHandler[0];

	};
	static_assert(sizeof(hkTrackerRefPtrLayoutHandler) == 0x10);

	class hkTrackerScanSnapshot : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkTrackerScanSnapshot;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkTrackerScanSnapshot[0];

		/* 0x010 */ std::byte pad010[0x148];
	};
	static_assert(sizeof(hkTrackerScanSnapshot) == 0x158);

	class hkTrackerSetLayoutHandler : public hkTrackerLayoutHandler
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkTrackerSetLayoutHandler;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkTrackerSetLayoutHandler[0];

	};
	static_assert(sizeof(hkTrackerSetLayoutHandler) == 0x10);

	class hkTrackerStringMapLayoutHandler : public hkTrackerLayoutHandler
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkTrackerStringMapLayoutHandler;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkTrackerStringMapLayoutHandler[0];

	};
	static_assert(sizeof(hkTrackerStringMapLayoutHandler) == 0x10);

	class hkTrackerStringPtrLayoutHandler : public hkTrackerLayoutHandler
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkTrackerStringPtrLayoutHandler;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkTrackerStringPtrLayoutHandler[0];

	};
	static_assert(sizeof(hkTrackerStringPtrLayoutHandler) == 0x10);

	class hkTrackerTypeLayout : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkTrackerTypeLayout;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkTrackerTypeLayout[0];

		/* 0x010 */ std::byte pad010[0x30];
	};
	static_assert(sizeof(hkTrackerTypeLayout) == 0x40);

	class hkTrackerTypeTreeCache : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkTrackerTypeTreeCache;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkTrackerTypeTreeCache[0];

		/* 0x010 */ std::byte pad010[0x160];
	};
	static_assert(sizeof(hkTrackerTypeTreeCache) == 0x170);

	class hkTypeInfoRegistry : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkTypeInfoRegistry;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkTypeInfoRegistry[0];

		virtual void hkTypeInfoRegistry_Unk04();
		virtual void hkTypeInfoRegistry_Unk05();
		virtual void hkTypeInfoRegistry_Unk06();
		virtual void hkTypeInfoRegistry_Unk07();
		virtual void hkTypeInfoRegistry_Unk08();
		/* 0x010 */ std::byte pad010[0x20];
	};
	static_assert(sizeof(hkTypeInfoRegistry) == 0x30);

	class hkTypeManager : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkTypeManager;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkTypeManager[0];

		/* 0x010 */ std::byte pad010[0xF0];
	};
	static_assert(sizeof(hkTypeManager) == 0x100);

	class hkUuidPseudoRandomGenerator : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkUuidPseudoRandomGenerator;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkUuidPseudoRandomGenerator[0];

		/* 0x010 */ std::byte pad010[0x10];
	};
	static_assert(sizeof(hkUuidPseudoRandomGenerator) == 0x20);

	class hkVdbCommandWriter : public hkStreamWriter
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkVdbCommandWriter;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkVdbCommandWriter[0];

		/* 0x010 */ std::byte pad010[0x20];
	};
	static_assert(sizeof(hkVdbCommandWriter) == 0x30);

	class hkVisualDebugger : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkVisualDebugger;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkVisualDebugger[0];

		virtual void hkVisualDebugger_Unk04();
		virtual void hkVisualDebugger_Unk05();
		/* 0x010 */ std::byte pad010[0xC8];
	};
	static_assert(sizeof(hkVisualDebugger) == 0xD8);

	class hkVtableClassRegistry : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkVtableClassRegistry;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkVtableClassRegistry[0];

		virtual void hkVtableClassRegistry_Unk04();
		virtual void hkVtableClassRegistry_Unk05();
		virtual void hkVtableClassRegistry_Unk06();
		virtual void hkVtableClassRegistry_Unk07();
		/* 0x010 */ std::byte pad010[0x10];
	};
	static_assert(sizeof(hkVtableClassRegistry) == 0x20);

	class hkWheelConstraintCinfo : public hkConstraintCinfo
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkWheelConstraintCinfo;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkWheelConstraintCinfo[0];

	};
	static_assert(sizeof(hkWheelConstraintCinfo) == 0x28);

	class hkWin32FileSystem : public hkFileSystem
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkWin32FileSystem;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkWin32FileSystem[0];

	};
	static_assert(sizeof(hkWin32FileSystem) == 0x10);

	class hkWin32StreamWriter : public hkStreamWriter
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkWin32StreamWriter;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkWin32StreamWriter[0];

		/* 0x010 */ void* m_handle;
	};
	static_assert(sizeof(hkWin32StreamWriter) == 0x18);

	static_assert(offsetof(hkWin32StreamWriter, m_handle) == 0x10);
	class hkXmlLexAnalyzer : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkXmlLexAnalyzer;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkXmlLexAnalyzer[0];

		/* 0x010 */ std::byte pad010[0x48];
	};
	static_assert(sizeof(hkXmlLexAnalyzer) == 0x58);

	class hkXmlParser : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkXmlParser;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkXmlParser[0];

		class Node : public hkReferencedObject
		{
		public:
			/* 0x010 */ std::byte pad010[0x8];
		};
		static_assert(sizeof(Node) == 0x18);
		class Characters : public Node
		{
		public:
			constexpr static Common::ID RTTI = IDs::RTTI::hkXmlParser__Characters;
			constexpr static Common::ID VTABLE = IDs::VTABLE::hkXmlParser__Characters[0];

			/* 0x018 */ std::byte pad018[0x8];
		};
		static_assert(sizeof(Characters) == 0x20);
		class EndElement : public Node
		{
		public:
			constexpr static Common::ID RTTI = IDs::RTTI::hkXmlParser__EndElement;
			constexpr static Common::ID VTABLE = IDs::VTABLE::hkXmlParser__EndElement[0];

			/* 0x018 */ std::byte pad018[0x8];
		};
		static_assert(sizeof(EndElement) == 0x20);
		class StartElement : public Node
		{
		public:
			constexpr static Common::ID RTTI = IDs::RTTI::hkXmlParser__StartElement;
			constexpr static Common::ID VTABLE = IDs::VTABLE::hkXmlParser__StartElement[0];

			/* 0x018 */ std::byte pad018[0x18];
		};
		static_assert(sizeof(StartElement) == 0x30);
		virtual void hkXmlParser_Unk04();
		virtual void hkXmlParser_Unk05();
		virtual void hkXmlParser_Unk06();
		/* 0x010 */ std::byte pad010[0x18];
	};
	static_assert(sizeof(hkXmlParser) == 0x28);

	class hkXmlStreamParser : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkXmlStreamParser;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkXmlStreamParser[0];

		/* 0x010 */ std::byte pad010[0xA8];
	};
	static_assert(sizeof(hkXmlStreamParser) == 0xB8);

	class hkXmlTagfileReader : public hkTagfileReader
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkXmlTagfileReader;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkXmlTagfileReader[0];

	};
	static_assert(sizeof(hkXmlTagfileReader) == 0x10);

	class hkXmlTagfileWriter : public hkTagfileWriter
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkXmlTagfileWriter;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkXmlTagfileWriter[0];

	};
	static_assert(sizeof(hkXmlTagfileWriter) == 0x10);

	class hkMemoryTrackStreamWriter : public hkStreamWriter
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkMemoryTrackStreamWriter;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkMemoryTrackStreamWriter[0];

		virtual void hkMemoryTrackStreamWriter_Unk0B();

		/* 0x010 */ std::byte pad010[0x10];
	};
	static_assert(sizeof(hkMemoryTrackStreamWriter) == 0x20);

	class hkMemorySnapshotProcess : public hkReferencedObject, public hkProcess
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkMemorySnapshotProcess;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkMemorySnapshotProcess[0];
	};
	static_assert(sizeof(hkMemorySnapshotProcess) == 0x40);

	class hkDummyMemoryTrackerImpl : public hkMemoryTracker
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkDummyMemoryTrackerImpl;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkDummyMemoryTrackerImpl[0];
	};
	static_assert(sizeof(hkDummyMemoryTrackerImpl) == 0x8);

}
