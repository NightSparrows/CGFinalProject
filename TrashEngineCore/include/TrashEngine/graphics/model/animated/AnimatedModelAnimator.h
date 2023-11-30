#pragma once

#include <TrashEngine/core/Base.h>
#include <TrashEngine/core/Time.h>
#include "AnimatedModelAnimation.h"

namespace TrashEngine {

	class AnimatedModelAnimator
	{
	public:
		virtual ~AnimatedModelAnimator() = default;

		NS_API virtual void update(Time delta) = 0;

		NS_API virtual void addAnimation(const std::string& name, Ref<AnimatedModelAnimation> animation) = 0;

		NS_API virtual void play(const std::string& name) = 0;

	};

}
