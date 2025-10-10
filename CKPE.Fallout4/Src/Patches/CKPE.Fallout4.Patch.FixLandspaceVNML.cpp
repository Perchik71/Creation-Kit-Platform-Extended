// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <xbyak.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/Forms/TESObjectLAND.h>
#include <Patches/CKPE.Fallout4.Patch.FixLandspaceVNML.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			FixLandspaceVNML::FixLandspaceVNML() : Common::Patch()
			{
				SetName("Fix Landspace VNML");
			}

			bool FixLandspaceVNML::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixLandspaceVNML::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixLandspaceVNML::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixLandspaceVNML::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixLandspaceVNML::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() >= VersionLists::EDITOR_FALLOUT_C4_1_10_982_3;
			}

			bool FixLandspaceVNML::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// In versions CK 1.10.980+, normalization is incorrect
				// Do you know this is the second time in a row, previously it was with Skyrim, 
				// has the quality of programmers dropped so much that they cannot count the reverse root?
				// -- THIS BASE FOR MATH ---
				// Besides, everything worked fine for you, why spoil the working code?

				// Remove useless stuff
				auto Rva1 = __CKPE_OFFSET(0);
				SafeWrite::WriteNop(Rva1, 0x12B);

				class xNormalize_Hook : public Xbyak::CodeGenerator
				{
				public:
					xNormalize_Hook(std::uintptr_t Addr) : Xbyak::CodeGenerator()
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

					static VOID Generate(std::uintptr_t Target)
					{
						auto hook = new xNormalize_Hook(Target + 5);
						Detours::DetourJump(Target, (std::uintptr_t)hook->getCode());
					}
				};

				xNormalize_Hook::Generate(Rva1);

				Detours::DetourJump(__CKPE_OFFSET(1),
					(std::uintptr_t)&EditorAPI::Forms::TESObjectLAND::Layers::HKNormalize);

				return true;
			}
		}
	}
}