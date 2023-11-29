#pragma once

#include <TrashEngine/core/Base.h>
#include <TrashEngine/deps/glm/glm.hpp>

#include <TrashEngine/graphics/Material.h>
#include "objects/OpenGLTexture.h"

namespace TrashEngine {

	class OpenGLMaterial : public Material
	{
	public:

		bool loadDiffuseTexture(const std::string& filePath);

		bool loadNormalTexture(const std::string& filePath);

		inline const glm::vec3& getDiffuseColor() const { return this->m_diffuseColor; }

		void setDiffuseColor(const glm::vec3& color) { this->m_diffuseColor = color; }

		inline const glm::vec3& getAmbient() const { return this->m_ambient; }

		inline const glm::vec3& getDiffuse() const { return this->m_diffuse; }

		inline const glm::vec3& getSpecular() const { return this->m_specular; }

		inline float getReflectivity() const { return this->m_reflectivity; }

		inline float getShininess() const { return this->m_shininess; }

		void setAmbient(const glm::vec3& ambient) { this->m_ambient = ambient; }

		void setDiffuse(const glm::vec3& diffuse) { this->m_diffuse = diffuse; }

		void setSpecular(const glm::vec3& specular) { this->m_specular = specular; }

		void setReflectivity(float reflectivity) { this->m_reflectivity = reflectivity; }

		void setShininess(float shininess) { this->m_shininess = shininess; }

		OpenGLTexture* getDiffuseTexture() { return this->m_diffuseTexture.get(); }

		OpenGLTexture* getNormalTexture() { return this->m_normalTexture.get(); }

	private:

		glm::vec3 m_diffuseColor;
		Scope<OpenGLTexture> m_diffuseTexture;
		Scope<OpenGLTexture> m_normalTexture;

		glm::vec3 m_ambient;
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;
		float m_reflectivity;
		float m_shininess;
	};

}
