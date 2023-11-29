#include "nspch.h"
#include "OpenGLFramebuffer.h"

namespace TrashEngine {

	OpenGLFramebuffer::OpenGLFramebuffer(uint32_t width, uint32_t height) :
		m_size((int)width, (int)height)
	{
		glCreateFramebuffers(1, &this->m_handle);
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &this->m_handle);
	}

	void OpenGLFramebuffer::useFramebuffer()
	{
		glViewport(0, 0, this->m_size.x, this->m_size.y);
		glBindFramebuffer(GL_FRAMEBUFFER, this->m_handle);
	}

}
