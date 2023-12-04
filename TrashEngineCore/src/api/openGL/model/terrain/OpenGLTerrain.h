#pragma once

#include <TrashEngine/graphics/model/terrain/Terrain.h>
#include <api/openGL/renderer/normalMapGenerator/OpenGLNormalMapGenerator.h>
#include <api/openGL/OpenGLMaterial.h>

namespace TrashEngine {

	class OpenGLTerrain : public Terrain
	{
	public:
		OpenGLTerrain(OpenGLNormalMapGenerator* m_normalMapGenerator, float heightIntensity, glm::ivec2 position);

		bool loadHeightMap(const std::string& filePath) override;

		inline OpenGLTexture* getHeightMapTexture() { return this->m_heightMapTexture.get(); }

		inline OpenGLTexture* getNormalMapTexture() { return this->m_normalMapTexture.get(); }

		inline float getHeightIntensity() const { return this->m_heightIntensity; }

		inline const glm::ivec2& getPosition() const { return this->m_position; }

	private:
		OpenGLNormalMapGenerator* m_normalMapGenerator;

		// the height of max height and min height intensity [-heightIntensity, heightIntensity]
		float m_heightIntensity;
		glm::ivec2 m_position;

		Scope<OpenGLTexture> m_heightMapTexture;
		Scope<OpenGLTexture> m_normalMapTexture;

		Scope<OpenGLTexture> m_blendMapTexture;

	};

}
