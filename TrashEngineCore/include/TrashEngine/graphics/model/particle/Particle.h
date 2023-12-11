#pragma once

#include <TrashEngine/deps/glm/glm.hpp>

namespace TrashEngine {

	/**
	* The prototype of a particle
	* will store the max particle count to this particle prototype
	*/
	class Particle
	{
	public:
		virtual ~Particle() = default;

		struct StartInfo
		{
			glm::vec3 position;			// the start in world position
			glm::vec3 velocity;
			float gravityEffect;
			float rotation;				// rotate by z axis
			float rotateVelocity;
			glm::vec2 scale;			// what is the scale of this particle, because basic particle is a square
			float lifeLength;
		};

	};

}
