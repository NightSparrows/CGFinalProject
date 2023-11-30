#pragma once

#include <map>

#include <TrashEngine/core/Time.h>

#include <TrashEngine/graphics/model/animated/AnimatedModelAnimator.h>
#include <api/openGL/model/animated/OpenGLAnimatedModel.h>

#include <TrashEngine/graphics/model/animated/AnimatedModelAnimation.h>

namespace TrashEngine {
	class OpenGLAnimatedModelAnimator : public AnimatedModelAnimator
	{
	public:
		struct AnimationState
		{
			Ref<AnimatedModelAnimation> animation;
			float currentTime = 0;		// current time relative to animation time
			bool running = false;		// is running, if running is false it pause
			bool loop = false;
			bool invert = false;
			float speedFactor = 1.f;
			float animatedFactor = 1.f;					// the animation factor of this [0, 1]
			float animatedTransitFactor = 0.f;			// draw by time, modify the animated factor
		};

	public:
		// takes in a loaded animated model, must be!!
		OpenGLAnimatedModelAnimator(Ref<OpenGLAnimatedModel> model);

		void update(Time delta) override;

		void addAnimation(const std::string& name, Ref<AnimatedModelAnimation> animation) override;

		void play(const std::string& name) override;

		void calculateBoneTransform(Time time);

		Ref<OpenGLAnimatedModel> getModel() { return this->m_model; }

		const std::vector<glm::mat4>& getBoneTransformations() const { return this->m_boneTransformations; }

	protected:
		void applyPoseToBones(const std::map<std::string, glm::mat4>& currentPose, Ref<const OpenGLAnimatedModelJoint> joint, const glm::mat4& parentTransform);

	private:
		Ref<OpenGLAnimatedModel> m_model;

		std::map<std::string, AnimationState> m_animations;

		std::vector<glm::mat4> m_boneTransformations;

	};
}
