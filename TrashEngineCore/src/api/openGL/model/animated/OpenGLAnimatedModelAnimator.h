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
		// takes in a loaded animated model, must be!!
		OpenGLAnimatedModelAnimator(Ref<OpenGLAnimatedModel> model);

		void update(Time delta) override;

		void addAnimation(const std::string& name, Ref<AnimatedModelAnimation> animation) override;

		AnimationState* getStateByName(const std::string& name);

		void start(const std::string& name, float transitTime = 0.5f);

		void setLooping(const std::string& name, bool looping);

		void play(const std::string& name, float transitTime) override;

		void calculateBoneTransform(Time time);

		Ref<OpenGLAnimatedModel> getModel() { return this->m_model; }

		const std::vector<glm::mat4>& getBoneTransformations() const { return this->m_boneTransformations; }

	protected:
		void applyPoseToBones(const std::map<std::string, glm::mat4>& currentPose, Ref<const OpenGLAnimatedModelJoint> joint, const glm::mat4& parentTransform);

	private:
		Ref<OpenGLAnimatedModel> m_model;

		std::map<std::string, AnimationState> m_animations;

		std::string m_currentAnimation;
		std::string m_targetAnimation;

		float m_currentTransitProgress;
		float m_transitTime;

		std::vector<glm::mat4> m_boneTransformations;

	};
}
