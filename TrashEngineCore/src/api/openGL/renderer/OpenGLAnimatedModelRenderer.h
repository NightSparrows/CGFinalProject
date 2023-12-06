#pragma once

#include <map>
#include <vector>

#include "OpenGLRenderer.h"

#include <api/openGL/objects/OpenGLShaderProgram.h>
#include <api/openGL/model/animated/OpenGLAnimatedModel.h>
#include <api/openGL/model/animated/OpenGLAnimatedModelAnimator.h>

namespace TrashEngine {

	class OpenGLAnimatedModelRenderer : public OpenGLRenderer
	{
	public:
		OpenGLAnimatedModelRenderer();
		~OpenGLAnimatedModelRenderer();

		void prepareScene(Scene* scene) override;

		void render() override;

	public:
		struct InstanceData
		{
			glm::mat4 transformationMatrix;
			Ref<OpenGLAnimatedModelAnimator> animator;
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
		std::vector<InstanceData> m_instances;

		GLuint m_boneTransformStorageBuffer;
	};

}
