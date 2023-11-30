#include "nspch.h"
#include <core/Logger.h>
#include "OpenGLAnimatedModelAnimator.h"

namespace TrashEngine {
	
	OpenGLAnimatedModelAnimator::OpenGLAnimatedModelAnimator(Ref<OpenGLAnimatedModel> model) :
		m_model(model)
	{
		this->m_boneTransformations.resize(model->getBones().size());
	}

	void OpenGLAnimatedModelAnimator::update(Time delta)
	{
		this->calculateBoneTransform(delta);
	}

	void OpenGLAnimatedModelAnimator::addAnimation(const std::string& name, Ref<AnimatedModelAnimation> animation)
	{
		AnimationState state{};
		state.animation = animation;
		state.currentTime = 0;
		state.invert = false;
		state.loop = true;
		state.running = false;
		state.speedFactor = 1.f;
		this->m_animations[name] = state;
	}

	void OpenGLAnimatedModelAnimator::play(const std::string& name)
	{
		auto it = this->m_animations.find(name);
		if (it == this->m_animations.end()) {
			NS_CORE_WARN("No animation name: {0}", name);
			return;
		}
		auto& state = it->second;
		state.running = true;
	}

	void OpenGLAnimatedModelAnimator::calculateBoneTransform(Time time)
	{
		for (glm::mat4& matrix : this->m_boneTransformations) {
			matrix = glm::identity<glm::mat4>();
		}

		std::map<std::string, glm::mat4> currentPose;
		for (auto& [name, state] : this->m_animations) {
			if (!state.running)
				continue;

			// dealing with time
			if (state.invert) {
				state.currentTime -= time.asSecond() * state.speedFactor;
				if (state.currentTime <= 0) {
					if (!state.loop) {
						state.running = false;
						state.currentTime = 0;
						continue;
					}
					else {
						state.currentTime = state.animation->getDuration() + state.currentTime;
					}
				}
			}
			else {
				state.currentTime += time.asSecond() * state.speedFactor;

				if (state.currentTime >= state.animation->getDuration()) {
					if (!state.loop) {
						state.running = false;
						state.currentTime = 0;
						continue;
					}
					else {
						do {
							state.currentTime -= state.animation->getDuration();
						} while (state.currentTime >= state.animation->getDuration());
					}
				}
			}
			// end dealing with time

			const auto& frames = state.animation->getKeyFrames();
			auto previousFrame = frames[0];
			auto nextFrame = frames[0];
			for (uint32_t i = 1; i < frames.size(); i++) {
				nextFrame = frames[i];
				if (nextFrame->timeStamp > state.currentTime)
					break;
				previousFrame = nextFrame;
			}
			float totalTime = nextFrame->timeStamp - previousFrame->timeStamp;
			float currentTime = state.currentTime - previousFrame->timeStamp;
			float progression = currentTime / totalTime;


			for (const auto& [boneName, boneTransform] : previousFrame->pose) {
				
				const auto& nextBoneTransform = nextFrame->pose.find(boneName)->second;
				Transform transform = Transform::Interpolate(boneTransform, nextBoneTransform, progression);
				transform = Transform::Interpolate(currentPose[boneName], transform, state.animatedFactor);
				currentPose[boneName] = transform;
			}

			// calculate the current pose, apply to transformation matrices

		}
		this->applyPoseToBones(currentPose, this->m_model->getRootJoint(), glm::identity<glm::mat4>());
	}

	void OpenGLAnimatedModelAnimator::applyPoseToBones(const std::map<std::string, glm::mat4>& currentPose, Ref<const OpenGLAnimatedModelJoint> joint, const glm::mat4& parentTransform)
	{
		glm::mat4 currentLocalTransform = glm::identity<glm::mat4>();

		auto it = currentPose.find(joint->getName());
		if (it != currentPose.end()) {
			currentLocalTransform = it->second;
		}
		glm::mat4 globalTransform = parentTransform * currentLocalTransform;

		// apply to the transformation
		if (joint->getBoneID() != UINT32_MAX) {
			const auto* currentBone = &this->m_model->getBones()[joint->getBoneID()];
			this->m_boneTransformations[joint->getBoneID()] = globalTransform  * currentBone->offsetMatrix/* TODO: transit animation */;
		}
		for (auto childJoint : joint->getChildren()) {
			applyPoseToBones(currentPose, childJoint, globalTransform);
		}

	}

}
