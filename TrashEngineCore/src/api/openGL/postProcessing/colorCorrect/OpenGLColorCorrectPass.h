#pragma once

#include <TrashEngine/core/Base.h>
#include <api/openGL/objects/OpenGLShaderProgram.h>

namespace TrashEngine {

	class OpenGLColorCorrectPass
	{
	public:
		OpenGLColorCorrectPass(glm::ivec2 sceneSize);
		~OpenGLColorCorrectPass();

		void resize(glm::ivec2 sceneSize);

		GLuint renderPass(GLuint inputTexture);

		void setExposure(float exposure) { this->m_exposure = exposure; }
		
		inline float getExposure() const { return this->m_exposure; }

	private:
		glm::ivec2 m_sceneSize;

		float m_exposure{ 1.f };

		Scope<OpenGLShaderProgram> m_colorProgram;

		GLuint m_resultTexture{ 0 };			// the result combine raw scene and bloom scene
	};

}
