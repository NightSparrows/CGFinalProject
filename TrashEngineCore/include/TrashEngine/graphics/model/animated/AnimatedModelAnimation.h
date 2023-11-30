#pragma once

#include <map>

#include <TrashEngine/utils/Transform.h>

namespace TrashEngine {

	class AnimatedModelAnimation
	{
	public:
		struct KeyFrame
		{
			float timeStamp;							// where the keyframe is (in second)
			std::map<std::string, Transform> pose;		// the bone name to where local transform
		};

	public:
		virtual ~AnimatedModelAnimation() = default;

		inline float getDuration() const { return this->m_duration; }

		void setDuration(float duration) { this->m_duration = duration; }

		std::vector<Ref<KeyFrame>>& getKeyFrames() { return this->m_frames; }

	protected:
		float m_duration;			// the whole animation time
		std::vector<Ref<KeyFrame>> m_frames;
	};

}
