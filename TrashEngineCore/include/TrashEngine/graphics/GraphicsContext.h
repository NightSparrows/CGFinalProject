#pragma once

#include <TrashEngine/core/RenderWindow.h>

#include "StaticModel.h"
#include "MasterRenderer.h"

namespace TrashEngine {

	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		NS_API virtual Ref<StaticModel> createStaticModel() = 0;

		NS_API virtual Ref<MasterRenderer> createMasterRenderer(glm::ivec2 renderSize) = 0;

	};

}
