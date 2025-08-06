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
		if (this->m_targetAnimation.empty() && this->m_currentAnimation.empty()) {
			for (uint32_t i = 0; i < m_model->getBones().size(); i++) {
				this->m_boneTransformations[i] = glm::identity<glm::mat4>();
			}
			return;
		}

		if (this->m_targetAnimation == this->m_currentAnimation) {
			this->m_targetAnimation.clear();
		}
		this->calculateBoneTransform(delta);
	}

	void OpenGLAnimatedModelAnimator::addAnimation(const std::string& name, Ref<AnimatedModelAnimation> animation)
	{
		AnimationState state{};
		state.animation = animation;
		state.currentTime = 0;
		state.invert = false;
		state.loop = true;
		state.speedFactor = 1.f;
		this->m_animations[name] = state;
	}

	AnimatedModelAnimator::AnimationState* OpenGLAnimatedModelAnimator::getStateByName(const std::string& name)
	{
		auto it = this->m_animations.find(name);
		if (it == this->m_animations.end()) {
			NS_CORE_WARN("No animation called: {0}", name);
			return nullptr;
		}
		auto& state = it->second;
		return &state;
	}

	void OpenGLAnimatedModelAnimator::start(const std::string& name, float transitTime)
	{
		auto it = this->m_animations.find(name);
		if (it == this->m_animations.end()) {
			NS_CORE_WARN("No animation called: {0}", name);
			return;
		}
		this->m_targetAnimation = name;
		this->m_transitTime = transitTime;
		this->m_currentTransitProgress = 0;
		auto& state = it->second;
		state.currentTime = 0;
		state.running = true;
	}

	void OpenGLAnimatedModelAnimator::setLooping(const std::string& name, bool looping)
	{
		auto it = this->m_animations.find(name);
		if (it == this->m_animations.end()) {
			NS_CORE_WARN("No animation called: {0}", name);
			return;
		}
		auto& state = it->second;
		state.loop = looping;
	}

	void OpenGLAnimatedModelAnimator::play(const std::string& name, float transitTime, bool resetTime)
	{
		auto it = this->m_animations.find(name);
		if (it == this->m_animations.end()) {
			NS_CORE_WARN("No animation name: {0}", name);
			return;
		}
		if (this->m_currentAnimation == name && this->m_targetAnimation != name) {		// the previous transition not done yet, reverse it
			this->m_currentAnimation = this->m_targetAnimation;
			this->m_targetAnimation = name;
			this->m_currentTransitProgress = 1.f - this->m_currentTransitProgress;
			this->m_transitTime = transitTime;
			return;
		}
		else if (this->m_targetAnimation == name) {
			return;
		}
		auto& state = it->second;
		state.running = true;
		if (resetTime) {
			state.currentTime = 0;
		}
		this->m_targetAnimation = name;
		this->m_transitTime = transitTime;
		this->m_currentTransitProgress = 0;
	}

	void OpenGLAnimatedModelAnimator::calculateBoneTransform(Time deltaTime)
	{

		std::map<std::string, glm::mat4> currentPose;
		auto currentStateIt = this->m_animations.find(this->m_currentAnimation);
		if (currentStateIt != this->m_animations.end()) {
			auto& state = currentStateIt->second;
			// dealing with time
			if (state.running) {
				if (state.invert) {
					state.currentTime -= deltaTime.asSecond() * state.speedFactor;
					if (state.currentTime <= 0) {
						if (!state.loop) {
							state.currentTime = 0.001f;
							state.running = false;
						}
						else {
							state.currentTime = state.animation->getDuration() + state.currentTime;
						}
					}
				}
				else {
					state.currentTime += deltaTime.asSecond() * state.speedFactor;

					if (state.currentTime >= state.animation->getDuration()) {
						if (!state.loop) {
							state.currentTime = state.animation->getDuration() - 0.001f;
							state.running = false;
						}
						else {
							do {
								state.currentTime -= state.animation->getDuration();
							} while (state.currentTime >= state.animation->getDuration());
						}
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
				currentPose[boneName] = transform;
			}
		}

		if (!this->m_targetAnimation.empty()) {
			auto targetStateIt = this->m_animations.find(this->m_targetAnimation);
			if (targetStateIt == this->m_animations.end()) {
				NS_CORE_WARN("Target animation not found: {0}", this->m_targetAnimation);
				return;
			}
			else {
				std::map<std::string, glm::mat4> targetPose;
				auto& targetState = targetStateIt->second;

				// dealing with time
				if (targetState.running) {
					if (targetState.invert) {
						targetState.currentTime -= deltaTime.asSecond() * targetState.speedFactor;
						if (targetState.currentTime <= 0) {
							if (!targetState.loop) {
								targetState.running = false;
								targetState.currentTime = 0.001f;
							}
							else {
								targetState.currentTime = targetState.animation->getDuration() + targetState.currentTime;
							}
						}
					}
					else {
						targetState.currentTime += deltaTime.asSecond() * targetState.speedFactor;

						if (targetState.currentTime >= targetState.animation->getDuration()) {
							if (!targetState.loop) {
								targetState.running = false;
								targetState.currentTime = targetState.animation->getDuration() - 0.001f;
							}
							else {
								do {
									targetState.currentTime -= targetState.animation->getDuration();
								} while (targetState.currentTime >= targetState.animation->getDuration());
							}
						}
					}
				}
				// end dealing with time

				const auto& frames = targetState.animation->getKeyFrames();
				auto previousFrame = frames[0];
				auto nextFrame = frames[0];
				for (uint32_t i = 1; i < frames.size(); i++) {
					nextFrame = frames[i];
					if (nextFrame->timeStamp > targetState.currentTime)
						break;
					previousFrame = nextFrame;
				}
				float totalTime = nextFrame->timeStamp - previousFrame->timeStamp;
				float currentTime = targetState.currentTime - previousFrame->timeStamp;
				float progression = currentTime / totalTime;


				for (const auto& [boneName, boneTransform] : previousFrame->pose) {

					const auto& nextBoneTransform = nextFrame->pose.find(boneName)->second;
					Transform transform = Transform::Interpolate(boneTransform, nextBoneTransform, progression);
					targetPose[boneName] = transform;
				}

				for (auto& [boneName, boneTransform] : currentPose) {
					auto& targetTransform = targetPose[boneName];
					Transform transform = Transform::Interpolate(boneTransform, targetTransform, this->m_currentTransitProgress);
					targetPose[boneName] = transform;
				}
				this->m_currentTransitProgress += (deltaTime / this->m_transitTime);
				if (this->m_currentTransitProgress >= 1.f) {
					this->m_currentAnimation = this->m_targetAnimation;
					this->m_targetAnimation.clear();
					this->m_currentTransitProgress = 0;
					this->m_transitTime = 0;
				}
				applyPoseToBones(targetPose, this->m_model->getRootJoint(), glm::identity<glm::mat4>());
			}
		}
		else {
			this->applyPoseToBones(currentPose, this->m_model->getRootJoint(), glm::identity<glm::mat4>());
		}
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
