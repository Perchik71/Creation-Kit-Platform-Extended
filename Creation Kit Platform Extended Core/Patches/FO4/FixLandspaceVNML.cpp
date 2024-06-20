// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/FO4/TESObjectLAND.h"
#include "FixLandspaceVNML.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			FixLandspaceVNMLPatch::FixLandspaceVNMLPatch() : Module(GlobalEnginePtr)
			{}

			bool FixLandspaceVNMLPatch::HasOption() const
			{
				return false;
			}

			bool FixLandspaceVNMLPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixLandspaceVNMLPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixLandspaceVNMLPatch::GetName() const
			{
				return "Fix Landspace VNML";
			}

			bool FixLandspaceVNMLPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixLandspaceVNMLPatch::GetDependencies() const
			{
				return {};
			}

			bool FixLandspaceVNMLPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_1_10_982_3;
			}

			bool FixLandspaceVNMLPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// In versions CK 1.10.980+, normalization is incorrect
					// Do you know this is the second time in a row, previously it was with Skyrim, 
					// has the quality of programmers dropped so much that they cannot count the reverse root?
					// -- THIS BASE FOR MATH ---
					// Besides, everything worked fine for you, why spoil the working code?

					// Remove useless stuff
					uintptr_t Rva1 = _RELDATA_RAV(0);
					lpRelocator->PatchNop(Rva1, 0x12B);

					class xNormalize_Hook : public Xbyak::CodeGenerator
					{
					public:
						xNormalize_Hook(uintptr_t Addr) : Xbyak::CodeGenerator()
						{
							// get points
							movups(xmm0, ptr[rsp + 0x30]);
							movups(xmm1, ptr[rsp + 0x40]);
							movups(xmm2, ptr[rsp + 0x20]);
							// get vectors
							subps(xmm0, xmm2);
							subps(xmm1, xmm2);
							// cross
							movaps(xmm2, xmm0);
							shufps(xmm2, xmm0, 0xC9);
							mulps(xmm2, xmm1);
							shufps(xmm1, xmm1, 0xC9);
							mulps(xmm0, xmm1);
							subps(xmm0, xmm2);
							shufps(xmm0, xmm0, 0xC9);
							// get mag
							movaps(xmm1, xmm0);
							mulps(xmm0, xmm0);
							pshufd(xmm2, xmm0, 0x55);
							addss(xmm2, xmm0);
							shufps(xmm2, xmm2, 0x0);
							shufps(xmm0, xmm0, 0xAA);
							addps(xmm0, xmm2);
							// normalize
							rsqrtps(xmm0, xmm0);
							mulps(xmm0, xmm1);
							// for original code
							movss(xmm8, xmm0);
							shufps(xmm0, xmm0, 0xE5);
							movss(xmm7, xmm0);
							movhlps(xmm6, xmm0);
							// return back (ret)
							jmp(ptr[rip]);
							dq(Addr);
						}

						static VOID Generate(uintptr_t Target)
						{
							auto hook = new xNormalize_Hook(GlobalRelocatorPtr->Rav2Off(Target + 5));
							GlobalRelocatorPtr->DetourJump(Target, (uintptr_t)hook->getCode());
						}
					};

					xNormalize_Hook::Generate(Rva1);

					lpRelocator->DetourJump(_RELDATA_RAV(1),
						(uintptr_t)&EditorAPI::Fallout4::TESObjectLAND::Layers::HKNormalize);

					return true;
				}

				return false;
			}

			bool FixLandspaceVNMLPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

		}
	}
}

