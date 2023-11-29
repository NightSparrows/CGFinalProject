#pragma once

#include <TrashEngine/deps/glm/glm.hpp>
#include <deps/glad/glad.h>

namespace TrashEngine {

	class OpenGLFramebuffer
	{
	public:
		OpenGLFramebuffer(uint32_t width, uint32_t height);
		~OpenGLFramebuffer();

		void useFramebuffer();

		GLuint getHandle() { return this->m_handle; }

	private:
		GLuint m_handle;

		glm::ivec2 m_size;

	};

}

