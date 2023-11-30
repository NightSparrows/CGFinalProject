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
			glm::vec3 diffuse;
			glm::vec3 ambient;
			glm::vec3 specular;
			float shininess;
			float reflectivity;
			float hasDiffuseTexture;
			glm::vec3 diffuseColor;
			float hasNormalTexture;
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
