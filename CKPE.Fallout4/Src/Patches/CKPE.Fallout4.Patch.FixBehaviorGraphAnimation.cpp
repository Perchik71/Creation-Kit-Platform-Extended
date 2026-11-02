// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Utils.h>
#include <CKPE.Detours.h>
#include <EditorAPI/IDs.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.AddressLibrary.h>
#include <CKPE.Common.Relocation.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.FixBehaviorGraphAnimation.h>

#include <EditorAPI/T/TESObjectREFR.h>
#include <EditorAPI/N/NiAVObject.h>
#include <EditorAPI/N/NiNode.h>
#include <EditorAPI/N/NiControllerManager.h>
#include <EditorAPI/N/NiControllerSequence.h>

#include <cstdint>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			// The animation init, InitNonNPCAnimation, Get3D, the controller manager lookup and the sequence update
			// have the same code (and the same class layouts / vtable slots) in 1.10.162.0, 1.10.982.3, 1.11.137.0,
			// 1.11.240.0 and 1.11.246.0; they were compared instruction by instruction, 1.10.162.0 only differs in
			// register allocation.
			using InitAnimation_t = std::int64_t(__fastcall*)(EditorAPI::Forms::TESObjectREFR* Ref);
			static InitAnimation_t OriginalInitAnimation = nullptr;

			static EditorAPI::NiAPI::NiControllerManager* FindControllerManager(const EditorAPI::NiAPI::NiAVObject* Root) noexcept(true)
			{
				for (auto controller = Root->GetControllers(); controller; controller = controller->GetNext())
				{
					if (auto manager = controller->IsNiControllerManager())
						return manager;
				}
				return nullptr;
			}

			static void StartSequencesIfIdle(EditorAPI::Forms::TESObjectREFR* Ref) noexcept(true)
			{
				using namespace EditorAPI;

				if (!Ref)
					return;

				const auto base = Ref->GetParent();
				if (!base || !base->Is(Forms::TESForm::ftStatic, Forms::TESForm::ftMovableStatic))
					return;

				const auto root = Ref->Get3D();
				if (!root)
					return;

				const auto manager = FindControllerManager(root);
				if (!manager)
					return;

				const auto count = manager->GetSequenceCount();
				if (!count)
					return;

				// A sequence is already running: the animation init started it (references without a behavior graph).
				for (std::uint16_t i = 0; i < count; ++i)
				{
					const auto sequence = manager->GetSequence(i);
					if (sequence && !sequence->IsInactive())
						return;
				}

				const auto node = root->IsNode();
				if (!node)
					return;

				// The animation init skips the NiControllerManager start when it created a behavior graph, and the CK never
				// updates behavior graphs. Its own start (InitNonNPCAnimation) activates only the first sequence, which
				// ends at once for these statics ("Autoplay" is clamped, "autoLoop" is the looping one), then leaves the
				// manager inactive. Start the idle sequences here with the same arguments and keep the manager active so
				// the scene update advances them.
				(void)Ref->InitNonNPCAnimation(node);

				bool started = false;
				for (std::uint16_t i = 0; i < count; ++i)
				{
					const auto sequence = manager->GetSequence(i);
					if (!sequence || !sequence->IsInactive())
						continue;
					if (sequence->Activate(0, false, 1.0f, 0.0f, nullptr, false))
						sequence->ResetOffset();
					started = true;
				}

				if (started)
					manager->SetActive(true);
			}

			static std::int64_t __fastcall HookInitAnimation(EditorAPI::Forms::TESObjectREFR* Ref)
			{
				const auto result = OriginalInitAnimation(Ref);
				StartSequencesIfIdle(Ref);
				return result;
			}

			FixBehaviorGraphAnimation::FixBehaviorGraphAnimation() : Common::Patch()
			{
				SetName("Fix behavior graph animation");
			}

			bool FixBehaviorGraphAnimation::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixBehaviorGraphAnimation::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixBehaviorGraphAnimation::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixBehaviorGraphAnimation::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixBehaviorGraphAnimation::DoQuery() const noexcept(true)
			{
				const auto ver = VersionLists::GetEditorVersion();
				return (ver != VersionLists::EDITOR_FALLOUT_C4_1_10_943_1) && (ver <= VersionLists::EDITOR_FALLOUT_C4_LAST);
			}

			bool FixBehaviorGraphAnimation::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				const auto init_animation = Relocation(EditorAPI::IDs::Functions::TESObjectREFR__InitAnimation).Address();

				OriginalInitAnimation = (InitAnimation_t)Detours::DetourJump(init_animation, (std::uintptr_t)&HookInitAnimation);
				return OriginalInitAnimation != nullptr;
			}
		}
	}
}
