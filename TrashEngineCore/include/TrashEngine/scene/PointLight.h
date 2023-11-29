#pragma once

#include <TrashEngine/deps/glm/glm.hpp>

namespace TrashEngine {

	struct PointLight
	{
	public:

		glm::vec3 position;
		// > 0 for enabling
		float enabled;
		glm::vec3 color;
		float blank2;
		glm::vec4 attenuation;

	};

}
