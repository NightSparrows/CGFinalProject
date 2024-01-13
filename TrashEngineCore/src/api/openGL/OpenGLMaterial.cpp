#include "nspch.h"
#include "OpenGLMaterial.h"

namespace TrashEngine {

	bool OpenGLMaterial::loadDiffuseTexture(const std::string& filePath)
	{
		this->m_diffuseTexture = CreateScope<OpenGLTexture>();

		OpenGLTexture::TextureConfig config;
		config.anisotropicFiltering = true;
		config.mipMapping = true;

		bool result = this->m_diffuseTexture->loadTexture2D(filePath, config);
		if (!result) {
			this->m_diffuseTexture.reset();
		}
		return result;
	}

	bool OpenGLMaterial::loadNormalTexture(const std::string& filePath)
	{
		this->m_normalTexture = CreateScope<OpenGLTexture>();

		OpenGLTexture::TextureConfig config;
		config.anisotropicFiltering = true;
		config.mipMapping = true;

		bool result = this->m_normalTexture->loadTexture2D(filePath, config);
		if (!result) {
			this->m_normalTexture.reset();
		}

		return result;
	}

	bool OpenGLMaterial::loadMetallicTexture(const std::string& filePath)
	{
		this->m_metallicTexture = CreateScope<OpenGLTexture>();

		OpenGLTexture::TextureConfig config;
		config.anisotropicFiltering = true;
		config.mipMapping = true;

		bool result = this->m_metallicTexture->loadTexture2D(filePath, config);
		if (!result) {
			this->m_metallicTexture.reset();
		}

		return result;
	}

	bool OpenGLMaterial::loadRoughnessTexture(const std::string& filePath)
	{
		this->m_roughnessTexture = CreateScope<OpenGLTexture>();

		OpenGLTexture::TextureConfig config;
		config.anisotropicFiltering = true;
		config.mipMapping = true;

		bool result = this->m_roughnessTexture->loadTexture2D(filePath, config);
		if (!result) {
			this->m_roughnessTexture.reset();
		}

		return result;
	}

}
