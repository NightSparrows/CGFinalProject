#pragma once

#include "OpenGLRenderer.h"
#include <api/openGL/objects/OpenGLShaderProgram.h>
#include <api/openGL/OpenGLStaticModel.h>

namespace TrashEngine {

	class OpenGLSkyRenderer :public OpenGLRenderer
	{
	public:
		OpenGLSkyRenderer();
		~OpenGLSkyRenderer();

		void prepareScene(Scene* scene) override;

		void render() override;

		void setSunAngle(float angle);

		inline float getSunAngle() const { return this->m_sunAngle; }
	
		inline const glm::vec3& getSunColor() const { return this->m_sunColor; }

		inline const glm::vec3& getSunDirection() const { return this->m_sunDirection; }

	protected:
		void calcSunColor();

		glm::vec3 computeSkyColor(glm::vec3 ray, glm::vec3 origin);

	public:
		float m_planetRadius{ 120.f };
		float m_atmosphereRadius{ 150.f };
		float m_seeHeight{ 120.f };
		glm::vec3 m_sunColor;
	private:
		
		Scope<OpenGLShaderProgram> m_shader;
		
		Scope<OpenGLStaticModel> m_sphereModel;

		float m_sunAngle{ 1.f };
		glm::vec3 m_sunDirection;


	};

}
