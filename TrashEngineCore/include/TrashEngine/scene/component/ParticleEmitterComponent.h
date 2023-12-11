#pragma once

#include <TrashEngine/core/Base.h>
#include <TrashEngine/graphics/model/particle/Particle.h>

namespace TrashEngine {

	class ParticleEmitterComponent
	{
	public:

		Ref<Particle> particle;

		// the particle ready to emit
		std::vector<Particle::StartInfo> startInfos;

	};

}
