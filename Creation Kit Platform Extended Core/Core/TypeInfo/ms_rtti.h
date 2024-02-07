// Author: Nukem9
// https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/typeinfo/ms_rtti.h

#pragma once

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		namespace MSRTTI
		{
			namespace detail { struct CompleteObjectLocator; }

			struct Info
			{
				uintptr_t VTableAddress;				// Address in .rdata section
				uintptr_t VTableOffset;					// Offset of this vtable in complete class (from top)
				uint64_t VFunctionCount;				// Number of contiguous functions
				const char* Name;						// Demangled
				const char* RawName;					// Mangled
				detail::CompleteObjectLocator* Locator;	//
			};

			void Initialize();
			void Dump(FILE* File);
			const Info* Find(const char* Name, bool Exact = true);
			Array<const Info*> FindAll(const char* Name, bool Exact = true);

			namespace detail
			{
				struct TypeDescriptor;
				struct PMD;
				struct CompleteObjectLocator;
				struct ClassHierarchyDescriptor;
				struct BaseClassArray;
				struct BaseClassDescriptor;

				template<typename T>
				struct RVA
				{
					uint32_t Offset;

					uintptr_t Address()
					{
						return (uintptr_t)GetModuleHandle(nullptr) + Offset;
					}

					T Get()
					{
						return (T)Address();
					}
				};

				// Also known as `class type_info`
				struct TypeDescriptor
				{
					void* vftable;		// const type_info::`vftable'
					uint64_t unknown;	// CRT internal
					char name[1];
				};

				struct PMD
				{
					int Mdisp;// Member displacement (vftable offset in the class itself)
					int Pdisp;// Vbtable displacement (vbtable offset, -1: vftable is at displacement PMD.mdisp inside the class)
					int Vdisp;// Displacement inside vbtable
				};

				struct CompleteObjectLocator
				{
					enum
					{
						COL_Signature32 = 0,
						COL_Signature64 = 1,
					};

					uint32_t Signature;								// 32-bit zero, 64-bit one
					uint32_t Offset;								// Offset of this vtable in the complete class
					uint32_t CDOffset;								// Constructor displacement offset
					RVA<TypeDescriptor*> TypeDescriptor;			// TypeDescriptor of the complete class
					RVA<ClassHierarchyDescriptor*> ClassDescriptor;// Describes inheritance hierarchy
				};

				struct ClassHierarchyDescriptor
				{
					enum
					{
						HCD_NoInheritance = 0,
						HCD_MultipleInheritance = 1,
						HCD_VirtualInheritance = 2,
						HCD_AmbiguousInheritance = 4,
					};

					uint32_t Signature;						// Always zero or one
					uint32_t Attributes;					// Flags
					uint32_t NumBaseClasses;				// Number of classes in BaseClassArray
					RVA<BaseClassArray*> BaseClassArray;	// BaseClassArray
				};

#pragma warning(push)
#pragma warning(disable: 4200) // nonstandard extension used: zero-sized array in struct/union
				struct BaseClassArray
				{
					uint32_t ArrayOfBaseClassDescriptors[]; // BaseClassDescriptor *
				};
#pragma warning(pop)

				struct BaseClassDescriptor
				{
					enum
					{
						BCD_NotVisible = 1,
						BCD_Ambiguous = 2,
						BCD_Private = 4,
						BCD_PrivOrProtBase = 8,
						BCD_Virtual = 16,
						BCD_Nonpolymorphic = 32,
						BCD_HasHierarchyDescriptor = 64,
					};

					RVA<TypeDescriptor*> TypeDescriptor;	// Type descriptor of the class
					uint32_t NumContainedBases;				// Number of nested classes following in the Base Class Array
					PMD Disp;								// Pointer-to-member displacement info
					uint32_t Attributes;					// Flags (BaseClassDescriptorFlags)
				};

				bool IsWithinRDATA(uintptr_t Address);
				bool IsWithinCODE(uintptr_t Address);
				bool IsValidCOL(CompleteObjectLocator* Locator);
				const char* strcasestr(const char* String, const char* Substring);
			}
		}
	}
}