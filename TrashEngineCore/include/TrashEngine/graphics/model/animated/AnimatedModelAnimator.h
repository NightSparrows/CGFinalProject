#pragma once

#include <TrashEngine/core/Base.h>
#include <TrashEngine/core/Time.h>
#include "AnimatedModelAnimation.h"

namespace TrashEngine {

	class AnimatedModelAnimator
	{
	public:
		struct AnimationState
		{
			Ref<AnimatedModelAnimation> animation;
			float currentTime = 0;		// current time relative to animation time
			bool loop = false;
			bool running = false;		// Whether it is influence by time, if loop is false, after animation is done, the running is false
			bool invert = false;
			float speedFactor = 1.f;
		};

	public:
		virtual ~AnimatedModelAnimator() = default;

		NS_API virtual AnimationState* getStateByName(const std::string& name) = 0;

		NS_API virtual void play(const std::string& name, float transitTime, bool resetTime = true) = 0;

		NS_API virtual void start(const std::string& name, float transitTime) = 0;

		NS_API virtual void setLooping(const std::string& name, bool looping) = 0;

		NS_API virtual void update(Time delta) = 0;

		NS_API virtual void addAnimation(const std::string& name, Ref<AnimatedModelAnimation> animation) = 0;

	};

}
