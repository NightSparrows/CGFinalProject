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
	}

}
