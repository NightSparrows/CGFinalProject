#pragma once

#include <TrashEngine/core/Base.h>
#include <api/openGL/objects/OpenGLShaderProgram.h>

namespace TrashEngine {

	class OpenGLBloomPass
	{
	public:
		OpenGLBloomPass(glm::ivec2 sceneSize);
		~OpenGLBloomPass();

		void resize(glm::ivec2 sceneSize);

		GLuint renderBloom(GLuint rawSceneTexture);

		void setIntensity(float intensity) { this->m_intensity = intensity; }

		inline float getIntensity() const { return this->m_intensity; }

		void setThreshold(float threshold) { this->m_threshold = threshold; }

		inline float getThreshold() const { return this->m_threshold; }

	private:
		glm::ivec2 m_sceneSize;

		// the bloom threshold
		float m_threshold{ 1.f };
		// how to bloom intensity is
		float m_intensity{ 1.f };

		// how many bloom level
		uint32_t m_levels{ 5 };

		Scope<OpenGLShaderProgram> m_fetchColorProgram;
		Scope<OpenGLShaderProgram> m_downBlurProgram;
		Scope<OpenGLShaderProgram> m_upAddProgram;
		Scope<OpenGLShaderProgram> m_combineProgram;

		std::vector<glm::ivec2> m_sizes;
		std::vector<GLuint> m_downScaleTextures;
		std::vector<GLuint> m_blurTempTextures;		// store the temp texture for gaussian blur
		std::vector<GLuint> m_upScaleTextures;

		GLuint m_resultTexture;			// the result combine raw scene and bloom scene

	};

}
