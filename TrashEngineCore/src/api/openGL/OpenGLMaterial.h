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

		OpenGLTexture* getDiffuseTexture() { return this->m_diffuseTexture.get(); }

		OpenGLTexture* getNormalTexture() { return this->m_normalTexture.get(); }

		bool loadMetallicTexture(const std::string& filePath);
		OpenGLTexture* getMetallicTexture() { return this->m_metallicTexture.get(); }

		inline float getAO() const { return this->m_ao; }

		inline float getRoughness() const { return this->m_roughnesss; }

		inline float getMetallic() const { return this->m_metallic; }

		inline float getEmissive() const { return this->m_emissive; }

		 // TODO setter

	private:

		Scope<OpenGLTexture> m_diffuseTexture;
		Scope<OpenGLTexture> m_normalTexture;

		Scope<OpenGLTexture> m_metallicTexture;
		Scope<OpenGLTexture> m_roughnessTexture;
		Scope<OpenGLTexture> m_aoTexture;

		glm::vec3 m_diffuseColor;
		float m_metallic;
		float m_roughnesss;
		float m_ao{ 1.f };
		float m_emissive;

	};

}
