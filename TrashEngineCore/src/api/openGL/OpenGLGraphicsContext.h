#pragma once

#include <TrashEngine/graphics/GraphicsContext.h>
#include <deps/glad/glad.h>
#include <core/GLFWRenderWindow.h>

/// <summary>
/// Only include in core source code, not include explicitly
/// </summary>
namespace TrashEngine {

	class OpenGLGraphicsContext : public GraphicsContext
	{
	public:
		~OpenGLGraphicsContext() = default;

		bool init(RenderWindow* window);

		void cleanUp();

	private:
		GLFWRenderWindow* m_window;
	};

}
