#pragma once

#include <TrashEngine/deps/glm/glm.hpp>

namespace TrashEngine {

	struct DirectionLight
	{
		glm::vec3 direction;
		float enabled;
		glm::vec3 color;
		float blank;
	};

}
