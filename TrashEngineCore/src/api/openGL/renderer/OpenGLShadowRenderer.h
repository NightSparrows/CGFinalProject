#pragma once

#include "OpenGLRenderer.h"
#include <api/openGL/objects/OpenGLShaderProgram.h>
#include <api/openGL/OpenGLStaticModel.h>
#include <TrashEngine/scene/DirectionLight.h>
#include <api/openGL/model/animated/OpenGLAnimatedModelAnimator.h>
#include <TrashEngine/scene/component/AnimatedModelComponent.h>
#include <TrashEngine/scene/component/AnimatedModelAnimatorComponent.h>

#include <api/openGL/renderer/shadow/ShadowBox.h>

namespace TrashEngine {

	class OpenGLShadowRenderer :public OpenGLRenderer
	{
	public:
		struct InstanceData
		{
			glm::mat4 transform;
		};
		struct AnimatedInstanceData
		{
			glm::mat4 transformationMatrix;
			Ref<OpenGLAnimatedModelAnimator> animator;
		};

		OpenGLShadowRenderer();
		~OpenGLShadowRenderer();

		void prepareScene(Scene* scene) override;

		void render() override;

		void setCamera(Camera* camera);

		void setSunLight(const DirectionLight& light);

	private:

		Scope<ShadowBox> m_shadowBox;
		Scope<OpenGLShaderProgram> m_shader;
		GLint m_projViewMatrixLocation;
		GLint m_transformMatrixLocation;

		glm::mat4 m_projectionViewMatrix;

		DirectionLight m_sunLight;
		uint32_t m_shadowMapSize;

		GLuint m_shadowFBO;
		GLuint m_shadowDepthTexture;

		// animated buffer
		GLuint m_boneTransformStorageBuffer;

		// objects
		std::map <Ref<OpenGLStaticModel>, std::vector<InstanceData>> m_staticList;
		std::vector<AnimatedInstanceData> m_animatedInstances;
	};

}
