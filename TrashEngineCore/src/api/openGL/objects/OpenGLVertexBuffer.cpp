#include "nspch.h"
#include "OpenGLVertexBuffer.h"

namespace TrashEngine {

	OpenGLVertexBuffer::OpenGLVertexBuffer()
	{
		glCreateBuffers(1, &this->m_bufferId);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &this->m_bufferId);
	}

}
