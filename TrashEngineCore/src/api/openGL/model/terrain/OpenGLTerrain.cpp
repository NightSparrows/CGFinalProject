#include "nspch.h"
#include "OpenGLTerrain.h"

namespace TrashEngine {

	OpenGLTerrain::OpenGLTerrain(OpenGLNormalMapGenerator* normalMapGenerator, float heightIntensity, glm::ivec2 position) :
		m_normalMapGenerator(normalMapGenerator), m_heightIntensity(heightIntensity), m_position(position)
	{

	}

	bool OpenGLTerrain::loadHeightMap(const std::string& filePath)
	{
		this->m_heightMapTexture = CreateScope<OpenGLTexture>();
		this->m_normalMapTexture = CreateScope<OpenGLTexture>();

		OpenGLTexture::TextureConfig config;
		config.mipMapping = false;
		config.anisotropicFiltering = false;
		if (!this->m_heightMapTexture->loadTexture2D(filePath, config)) {
			return false;
		}
		if (!this->m_normalMapGenerator->generateNormalMapFromHeight(this->m_heightMapTexture.get(), this->m_normalMapTexture.get())) {
			return false;
		}

		return true;
	}

}
