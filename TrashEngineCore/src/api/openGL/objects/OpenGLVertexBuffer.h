#pragma once

#include <deps/glad/glad.h>

namespace TrashEngine {

	class OpenGLVertexBuffer
	{
	public:
		OpenGLVertexBuffer();
		virtual ~OpenGLVertexBuffer();

	private:
		GLuint m_bufferId{ 0 };
	};

}
