#pragma once

#include <vector>
#include <TrashEngine/core/Base.h>
#include <TrashEngine/deps/glm/glm.hpp>

namespace TrashEngine {

	class AnimatedModelJointData
	{
	public:
		std::vector<Ref<AnimatedModelJointData>> children;

		// if bone it is not a bone it will be UINT32_MAX
		uint32_t boneID;
		std::string name;

		glm::mat4 transformation;

	};

}
