#include "nspch.h"
#include "OpenGLVertexBuffer.h"

namespace TrashEngine {

	OpenGLVertexBuffer::OpenGLVertexBuffer(GLsizeiptr size, GLenum usage) :
		m_size(size), m_usage(usage)
	{
		glCreateBuffers(1, &this->m_handle);
		glNamedBufferData(this->m_handle, size, nullptr, this->m_usage);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &this->m_handle);
	}

	void OpenGLVertexBuffer::storeData(GLsizeiptr size, GLintptr offset, const void* data)
	{
		glNamedBufferSubData(this->m_handle, offset, size, data);
	}

}
