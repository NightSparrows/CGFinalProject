#pragma once

#include <TrashEngine/core/Base.h>
#include <TrashEngine/graphics/model/animated/AnimatedModelAnimator.h>

namespace TrashEngine {

	struct AnimatedModelAnimatorComponent
	{
		Ref<AnimatedModelAnimator> animator;

		AnimatedModelAnimatorComponent(Ref<AnimatedModelAnimator> animator)
		{
			this->animator = animator;
		}

		AnimatedModelAnimatorComponent() = default;
		AnimatedModelAnimatorComponent(const AnimatedModelAnimatorComponent&) = default;

	};

}
