// Author: Nukem9
// https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/typeinfo/ms_rtti.cpp

#include <windows.h>
#include <rttidata.h>
#include <CKPE.Application.h>
#include <CKPE.HashUtils.h>
#include <CKPE.Stream.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.RTTI.h>
#include <CKPE.ErrorHandler.h>
#include <CKPE.Common.MemoryManager.h>
#include <unordered_map>
#include <format>

extern "C"
{
	typedef void* (*malloc_func_t)(size_t);
	typedef void(*free_func_t)(void*);
	char* __unDNameEx(char* outputString, const char* name, int maxStringLength, malloc_func_t pAlloc, free_func_t pFree, 
		char* (__fastcall* pGetParameter)(int), unsigned int disableFlags);
}

namespace CKPE
{
	namespace Common
	{
		static RTTI srtti;

		std::unordered_map<std::uint32_t, RTTI::Info> srtti_data;

		bool RTTI::IsWithinDATA(std::uintptr_t addr) const noexcept(true)
		{
			return ((addr >= segrdata.GetAddress()) && (addr <= segrdata.GetEndAddress())) ||
				((addr >= segdata.GetAddress()) && (addr <= segdata.GetEndAddress()));
		}

		bool RTTI::IsWithinCODE(std::uintptr_t addr) const noexcept(true)
		{
			return (addr >= segtext.GetAddress()) && (addr <= segtext.GetEndAddress());
		}

		bool RTTI::IsValidCOL(CompleteObjectLocator* Locator) const noexcept(true)
		{
			return (Locator->Signature == CompleteObjectLocator::COL_Signature64) && 
				IsWithinDATA(Locator->TypeDescriptor.Address(base));
		}

		std::uint32_t RTTI::GetCountVFunc(std::uintptr_t addr) const noexcept(true)
		{
			std::uint32_t r = 0;
			// Determine number of virtual functions
			for (std::uintptr_t j = addr; j < (segrdata.GetEndAddress() - sizeof(std::uintptr_t)); j += sizeof(std::uintptr_t))
			{
				if (!IsWithinCODE(*(std::uintptr_t*)j)) break;
				r++;
			}
			return r;
		}

		[[nodiscard]] RTTI* RTTI::GetSingleton() noexcept(true)
		{
			return &srtti;
		}

		void RTTI::Initialize() noexcept(true)
		{
			auto _sapp = Interface::GetSingleton()->GetApplication();
			
			base     = _sapp->GetBase();
			segrdata = _sapp->GetSegment(Segment::rdata);
			segdata  = _sapp->GetSegment(Segment::data);
			segtext  = _sapp->GetSegment(Segment::text);

			srtti_data.clear();

			for (uintptr_t i = segrdata.GetAddress(); 
				i < (segrdata.GetEndAddress() - (sizeof(uintptr_t) << 1)); 
				i++)
			{
				// Skip all non-2-aligned addresses. Not sure if this is OK or it skips tables.
				if (i % 2 != 0)
					continue;

				// This might be a valid RTTI entry, so check if:
				// - The COL points to somewhere in .rdata
				// - The COL has a valid signature
				// - The first virtual function points to .text
				//
				uintptr_t addr = *(uintptr_t*)i;
				uintptr_t vfuncAddr = *(uintptr_t*)(i + sizeof(uintptr_t));

				if (!IsWithinDATA(addr) || !IsWithinCODE(vfuncAddr))
					continue;

				auto locator = reinterpret_cast<CompleteObjectLocator*>(addr);
				if (!IsValidCOL(locator))
					continue;
				
				// skip childs
				if (locator->Offset != 0)
					continue;

				auto rawName = locator->TypeDescriptor.Get(base)->name;

				Info info{
					.VTableAddress = i + sizeof(uintptr_t),
					.VTableOffset = locator->Offset,
					.VFunctionCount = 0,
					.RawName = rawName,
					.Locator = locator,
				};

				info.Name = __unDNameEx(nullptr, rawName + 1, 0, malloc, free, nullptr, 0x2800);
				info.VFunctionCount = GetCountVFunc(info.VTableAddress);

				srtti_data.insert({ HashUtils::MurmurHash32(info.Name), info});
			}
		}

		void RTTI::Dump(const std::wstring& fname) const noexcept(true)
		{
			TextFileStream stream(fname, FileStream::fmCreate);

			for (const auto& i : srtti_data)
			{
				auto& info = i.second;
				stream.WriteLine("`%s`: VTable [0x%p, 0x%p offset, %lld functions] `%s`", info.Name,
					info.VTableAddress - base, info.VTableOffset, info.VFunctionCount, info.RawName);
				
				// perchik71: output info about parent class

				auto parent_classes = info.Locator->ClassDescriptor.Get(base);
				if (parent_classes && (parent_classes->Attributes != ClassHierarchyDescriptor::HCD_NoInheritance))
				{
					// 0 - same object
					for (std::uint32_t i = 1; i < parent_classes->NumBaseClasses; i++)
					{
						auto desc =
							RVA<BaseClassDescriptor*>
							(parent_classes->BaseClassArray.Get(base)->ArrayOfBaseClassDescriptors[i]).Get(base);
	
						auto rawName = desc->TypeDescriptor.Get(base)->name;
						auto Name = __unDNameEx(nullptr, rawName + 1, 0, malloc, free, nullptr, 0x2800);

						stream.WriteLine("\t\t#%u: flags(%x) `%s`:`%s`", desc->NumContainedBases, desc->Attributes, 
							Name, rawName);
					}
				}
			}
		}

		const RTTI::Info* RTTI::Find(const std::string_view& name, bool error_if_no_found) const noexcept(true)
		{
			auto it = srtti_data.find(HashUtils::MurmurHash32(name.data(), name.length()));
			if (it == srtti_data.end())
			{
				if (error_if_no_found)
					ErrorHandler::Trigger(std::format("RTTI::Find \"{}\" had no results", name));
				return nullptr;
			}
			return &(it->second);
		}

		const RTTI::Info* RTTI::Find(const std::uintptr_t address, bool error_if_no_found) const noexcept(true)
		{
			for (const auto& info : srtti_data)
				if (info.second.VTableAddress == address)
					return &info.second;

			if (error_if_no_found)
				ErrorHandler::Trigger(std::format("RTTI::Find \"{:x}\" had no results", address));
			return nullptr;
		}

		void* RTTI::Cast(const void* InPtr, long VfDelta, const char* lpstrFromType, const char* lpstrTargetType,
			bool isReference)
		{
			auto FromInfo = Find(lpstrFromType);
			auto TargetInfo = Find(lpstrTargetType);
			if (!FromInfo || !TargetInfo)
				return nullptr;
			return __RTDynamicCast((PVOID)InPtr, VfDelta,
				(PVOID)FromInfo->Locator->TypeDescriptor.Get(base),
				(PVOID)TargetInfo->Locator->TypeDescriptor.Get(base),
				isReference);
		}
	}
}