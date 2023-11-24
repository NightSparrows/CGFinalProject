#include "nspch.h"
#include "OpenGLMaterial.h"

namespace TrashEngine {

	bool OpenGLMaterial::loadDiffuseTexture(const std::string& filePath)
	{
		this->m_diffuseTexture = CreateScope<OpenGLTexture>();

		OpenGLTexture::TextureConfig config;
		config.anisotropicFiltering = true;
		config.mipMapping = true;

		return this->m_diffuseTexture->loadTexture2D(filePath, config);
	}

}
