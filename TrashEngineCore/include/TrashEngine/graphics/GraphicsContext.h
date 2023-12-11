#pragma once

#include <TrashEngine/core/RenderWindow.h>

#include "StaticModel.h"
#include "model/animated/AnimatedModel.h"
#include "model/animated/AnimatedModelAnimator.h"
#include "model/terrain/Terrain.h"
#include "model/particle/Particle.h"
#include "MasterRenderer.h"

namespace TrashEngine {

	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		NS_API virtual Ref<StaticModel> createStaticModel() = 0;

		NS_API virtual Ref<AnimatedModel> createAnimatedModel() = 0;

		NS_API virtual Ref<Terrain> createTerrain(float heightIntensity, glm::ivec2 position) = 0;

		// create the particle prototype
		NS_API virtual Ref<Particle> createParticle(const std::string& filePath, uint32_t numberOfRow) = 0;

		NS_API virtual Ref<AnimatedModelAnimator> createAnimatedModelAnimator(Ref<AnimatedModel> animatedModel) = 0;

		NS_API virtual Ref<MasterRenderer> createMasterRenderer(glm::ivec2 renderSize) = 0;

	};

}
