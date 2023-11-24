#pragma once

#include <deps/glad/glad.h>

namespace TrashEngine {

	class OpenGLVertexBuffer
	{
	public:
		OpenGLVertexBuffer(GLsizeiptr size, GLenum usage);
		virtual ~OpenGLVertexBuffer();

		void storeData(GLsizeiptr size, GLintptr offset, const void* data);

	private:
		GLuint m_handle{ 0 };
		GLsizeiptr m_size;
		GLenum m_usage{ GL_STATIC_DRAW };
	};

}
