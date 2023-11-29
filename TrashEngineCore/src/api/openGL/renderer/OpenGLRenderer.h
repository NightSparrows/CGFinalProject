#pragma once

#include <TrashEngine/scene/Scene.h>

namespace TrashEngine {

	class OpenGLRenderer
	{
	public:

		virtual void prepareScene(Scene* scene) = 0;

		virtual void render() = 0;

	};

}
