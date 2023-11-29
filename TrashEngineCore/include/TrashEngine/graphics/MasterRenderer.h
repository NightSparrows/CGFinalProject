#pragma once

#include "Camera.h"
#include <TrashEngine/scene/Scene.h>

namespace TrashEngine {

	class MasterRenderer
	{
	public:
		virtual ~MasterRenderer() = default;

		/// <summary>
		/// Draw the current frame, which handle all of the process and draw to swapchain buffers
		/// </summary>
		/// <param name="camera"></param>
		/// <returns></returns>
		NS_API virtual void renderFrame(Camera* camera, Scene* scene) = 0;

	};

}
