#include "nspch.h"
#include "OpenGLParticle.h"

namespace TrashEngine {

	OpenGLParticle::OpenGLParticle(const std::string textureFilePath, uint32_t numberOfRow) :
		m_numberOfRow(numberOfRow)
	{
		this->m_texture = CreateScope<OpenGLTexture>();
		OpenGLTexture::TextureConfig config;
		config.anisotropicFiltering = true;
		config.mipMapping = true;
		this->m_texture->loadTexture2D(textureFilePath, config);
		// TODO make the max particle count configurable
		glCreateBuffers(1, &this->m_dataBuffer);
		// GPU only buffer no flag
		glNamedBufferStorage(this->m_dataBuffer, this->m_maxNumberOfParticle * sizeof(ParticleData), nullptr, 0);
	}

	OpenGLParticle::~OpenGLParticle()
	{
		glDeleteBuffers(1, &this->m_dataBuffer);
	}

}
