// Author: Nukem9
// https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/typeinfo/ms_rtti.h

#pragma once

#include <CKPE.Segment.h>
#include <CKPE.Common.Common.h>
#include <string_view>
#include <string>
#include <vector>

namespace CKPE
{
	namespace Common
	{
		class CKPE_COMMON_API RTTI
		{
			std::uintptr_t base{ 0 };

			Segment segtext{};
			Segment segdata{};
			Segment segrdata{};
		public:
			template<typename T>
			struct RVA
			{
				uint32_t offset;

				[[nodiscard]] std::uintptr_t Address(std::uintptr_t base) const noexcept(true)
				{ return (std::uintptr_t)base + offset; }
				[[nodiscard]] T Get(std::uintptr_t base) const noexcept(true)
				{ return (T)Address(base); }
			};

			// Also known as `class type_info`
			struct TypeDescriptor
			{
				void* vftable;			// const type_info::`vftable'
				std::uintptr_t unknown;	// CRT internal
				char name[1];
			};

			struct PMD
			{
				std::int32_t Mdisp;	// Member displacement (vftable offset in the class itself)
				std::int32_t Pdisp;	// Vbtable displacement (vbtable offset, -1: vftable is at displacement PMD.mdisp inside the class)
				std::int32_t Vdisp;	// Displacement inside vbtable
			};

			struct BaseClassDescriptor
			{
				enum : std::uint32_t
				{
					BCD_NotVisible				= 1 << 0,
					BCD_Ambiguous				= 1 << 1,
					BCD_Private					= 1 << 2,
					BCD_PrivOrProtBase			= 1 << 3,
					BCD_Virtual					= 1 << 4,
					BCD_Nonpolymorphic			= 1 << 5,
					BCD_HasHierarchyDescriptor	= 1 << 6,
				};

				RVA<TypeDescriptor*> TypeDescriptor;	// Type descriptor of the class
				std::uint32_t NumContainedBases;		// Number of nested classes following in the Base Class Array
				PMD Disp;								// Pointer-to-member displacement info
				std::uint32_t Attributes;				// Flags (BaseClassDescriptorFlags)
			};

#pragma warning(push)
#pragma warning(disable: 4200) // nonstandard extension used: zero-sized array in struct/union
			struct BaseClassArray
			{
				std::uint32_t ArrayOfBaseClassDescriptors[]; // BaseClassDescriptor *
			};
#pragma warning(pop)

			struct ClassHierarchyDescriptor
			{
				enum : std::uint32_t
				{
					HCD_NoInheritance			= 0,
					HCD_MultipleInheritance		= 1 << 0,
					HCD_VirtualInheritance		= 1 << 1,
					HCD_AmbiguousInheritance	= 1 << 2,
				};

				std::uint32_t Signature;				// Always zero or one
				std::uint32_t Attributes;				// Flags
				std::uint32_t NumBaseClasses;			// Number of classes in BaseClassArray
				RVA<BaseClassArray*> BaseClassArray;	// BaseClassArray
			};

			struct CompleteObjectLocator
			{
				enum : std::uint32_t
				{
					COL_Signature32 = 0,
					COL_Signature64 = 1,
				};

				std::uint32_t Signature;							// 32-bit zero, 64-bit one
				std::uint32_t Offset;								// Offset of this vtable in the complete class
				std::uint32_t CDOffset;								// Constructor displacement offset
				RVA<TypeDescriptor*> TypeDescriptor;				// TypeDescriptor of the complete class
				RVA<ClassHierarchyDescriptor*> ClassDescriptor;		// Describes inheritance hierarchy
			};

			struct Info
			{
				std::uintptr_t VTableAddress;			// Address in .rdata section
				std::uintptr_t VTableOffset;			// Offset of this vtable in complete class (from top)
				std::uint64_t VFunctionCount;			// Number of contiguous functions
				const char* Name;						// Demangled
				const char* RawName;					// Mangled
				CompleteObjectLocator* Locator;			//
			};
		private:
			RTTI(const RTTI&) = delete;
			RTTI& operator=(const RTTI&) = delete;

			[[nodiscard]] bool IsWithinDATA(std::uintptr_t addr) const noexcept(true);
			[[nodiscard]] bool IsWithinCODE(std::uintptr_t addr) const noexcept(true);
			[[nodiscard]] bool IsValidCOL(CompleteObjectLocator* Locator) const noexcept(true);
			[[nodiscard]] std::uint32_t GetCountVFunc(std::uintptr_t addr) const noexcept(true);
		public:
			RTTI() noexcept(true) = default;

			[[nodiscard]] static RTTI* GetSingleton() noexcept(true);

			void Initialize() noexcept(true);
			virtual void Dump(const std::wstring& fname) const noexcept(true);
			virtual const Info* Find(const std::string_view& name) const noexcept(true);
			virtual void* Cast(const void* InPtr, long VfDelta,
				const char* lpstrFromType, const char* lpstrTargetType, bool isReference = false);
		};
	}

	inline static void* _DYNAMIC_CAST(const void* InPtr, long VfDelta, const char* lpstrFromType,
		const char* lpstrTargetType, bool isReference = false)
	{
		return Common::RTTI::GetSingleton()->Cast(InPtr, VfDelta, lpstrFromType, lpstrTargetType, isReference);
	}
}