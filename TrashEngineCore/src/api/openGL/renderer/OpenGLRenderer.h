#pragma once

#include <TrashEngine/core/Time.h>
#include <TrashEngine/scene/Scene.h>
#include <TrashEngine/graphics/Camera.h>

namespace TrashEngine {

	class OpenGLRenderer
	{
	public:

		virtual void update(Camera* camera, Time deltaTime) {}

		virtual void prepareScene(Scene* scene) = 0;

		virtual void render() = 0;

	};

}
