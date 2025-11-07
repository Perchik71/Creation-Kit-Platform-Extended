// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BaseFormComponent.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSTransform;
			}

			class BGSPreviewTransform : public BaseFormComponent
			{
				Forms::BGSTransform* _Transform;
			public:
				BGSPreviewTransform() = default;
				BGSPreviewTransform(Forms::BGSTransform& transform) : _Transform(&transform) {}
				BGSPreviewTransform(Forms::BGSTransform* transform) : _Transform(transform) {}
				BGSPreviewTransform(const BGSPreviewTransform& Rhs) : _Transform(Rhs._Transform) {}

				BGSPreviewTransform& operator=(const BGSPreviewTransform& Rhs) noexcept(true) { _Transform = Rhs._Transform; return *this; }

				constexpr bool operator==(const BGSPreviewTransform& Rhs) const noexcept(true) { return _Transform == Rhs._Transform; }
				constexpr bool operator!=(const BGSPreviewTransform& Rhs) const noexcept(true) { return _Transform != Rhs._Transform; }

				virtual ~BGSPreviewTransform() = default;
			public:
				[[nodiscard]] inline Forms::BGSTransform* GetTransform() const noexcept(true) { return _Transform; }
				inline void SetTransform(Forms::BGSTransform* transform) noexcept(true) { _Transform = transform; }
			public:
				CKPE_PROPERTY(GetTransform, SetTransform) Forms::BGSTransform* Transform;
			};
			static_assert(sizeof(BGSPreviewTransform) == 0x10);
		}
	}
}