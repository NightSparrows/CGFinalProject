#pragma once

#include <map>
#include <vector>

#include "OpenGLRenderer.h"

#include <api/openGL/objects/OpenGLShaderProgram.h>
#include <api/openGL/OpenGLStaticModel.h>

namespace TrashEngine {

	class OpenGLStaticModelRenderer : public OpenGLRenderer
	{
	public:
		OpenGLStaticModelRenderer();

		void prepareScene(Scene* scene) override;

		void render() override;

	public:
		struct InstanceData
		{
			glm::mat4 transform;
		};

		struct MaterialData
		{
			float hasDiffuseTexture;
			glm::vec3 diffuseColor;
			float hasNormalTexture;
			float hasMetallicTexture;
			float metallic;
			float hasRoughnessTexture;
			float roughness;
			float hasAOTexture;
			float ao;
			float emissive;
		};


	private:
		Scope<OpenGLShaderProgram> m_shader;
		GLint m_transformationMatrixLocation;
		GLint m_tiModelMatrixLocation;

		GLuint m_materialStorageBuffer;
		std::map <Ref<OpenGLStaticModel>, std::vector<InstanceData>> m_entityList;

	};

}
