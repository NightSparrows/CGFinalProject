#include "nspch.h"
#include <TrashEngine/scene/component/TransformComponent.h>
#include <TrashEngine/scene/component/StaticModelComponent.h>
#include "OpenGLShadowRenderer.h"
#include <api/openGL/builder/OpenGLShaderProgramBuilder.h>

namespace TrashEngine {
	
	OpenGLShadowRenderer::OpenGLShadowRenderer() :
		m_shadowMapSize(4096)
	{
		this->m_shadowBox = CreateScope<ShadowBox>();

		OpenGLShaderProgramBuilder shaderBuilder;
		shaderBuilder.addShaderFromFile("assets/TrashEngine/shaders/shadow/shader.vert", GL_VERTEX_SHADER);
		shaderBuilder.addShaderFromFile("assets/TrashEngine/shaders/shadow/shader.frag", GL_FRAGMENT_SHADER);
		this->m_shader = Scope<OpenGLShaderProgram>(shaderBuilder.build());
		this->m_projViewMatrixLocation = this->m_shader->getUniformLocationFromName("u_projectionViewMatrix");
		this->m_transformMatrixLocation = this->m_shader->getUniformLocationFromName("u_transformationMatrix");
		this->m_shader->getUniformLocationFromName("u_isStatic");

		glCreateFramebuffers(1, &this->m_shadowFBO);

		glCreateTextures(GL_TEXTURE_2D, 1, &this->m_shadowDepthTexture);
		glTextureStorage2D(this->m_shadowDepthTexture, 1, GL_DEPTH_COMPONENT24, this->m_shadowMapSize, this->m_shadowMapSize);
		glTextureParameteri(this->m_shadowDepthTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(this->m_shadowDepthTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(this->m_shadowDepthTexture, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(this->m_shadowDepthTexture, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glNamedFramebufferTexture(this->m_shadowFBO, GL_DEPTH_ATTACHMENT, this->m_shadowDepthTexture, 0);
		glNamedFramebufferDrawBuffer(this->m_shadowFBO, GL_NONE);
		glNamedFramebufferReadBuffer(this->m_shadowFBO, GL_NONE);

		glCreateBuffers(1, &this->m_boneTransformStorageBuffer);
		glNamedBufferStorage(this->m_boneTransformStorageBuffer, 1000 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
	}

	OpenGLShadowRenderer::~OpenGLShadowRenderer()
	{
		glDeleteBuffers(1, &this->m_boneTransformStorageBuffer);

		glDeleteTextures(1, &this->m_shadowDepthTexture);
		glDeleteFramebuffers(1, &this->m_shadowFBO);
	}

	void OpenGLShadowRenderer::prepareScene(Scene* scene)
	{
		// prepare object
		// static
		this->m_staticList.clear();
		auto view = scene->Reg().view<TransformComponent, StaticModelComponent>();
		for (auto [entity, transform, model] : view.each()) {
			Ref<OpenGLStaticModel> ogModel = StaticCastRef<OpenGLStaticModel>(model.model);
			auto& list = this->m_staticList[ogModel];
			auto& instanceData = list.emplace_back();
			instanceData.transform = transform.transform;
		}
		// animated
		this->m_animatedInstances.clear();
		auto animView = scene->Reg().view<TransformComponent, AnimatedModelAnimatorComponent>();
		for (auto [entity, transform, animator] : animView.each()) {
			Ref<OpenGLAnimatedModelAnimator> ogAnimator = StaticCastRef<OpenGLAnimatedModelAnimator>(animator.animator);
			auto& instanceData = this->m_animatedInstances.emplace_back();
			instanceData.transformationMatrix = transform.transform;
			instanceData.animator = ogAnimator;
		}


	}

	void OpenGLShadowRenderer::render()
	{
		this->m_shadowBox->update(this->m_sunLight.direction);

		this->m_projectionViewMatrix = this->m_shadowBox->getProjectionMatrix() * this->m_shadowBox->getLightViewMatrix();
		glBindFramebuffer(GL_FRAMEBUFFER, this->m_shadowFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		this->m_shader->bind();
		this->m_shader->load(this->m_projViewMatrixLocation, this->m_projectionViewMatrix);

		// draw object
		// static
		this->m_shader->loadFloat("u_isStatic", 1.f);
		for (auto [model, instanceDataList] : this->m_staticList) {
			model->bind();

			for (const auto& instanceData : instanceDataList) {
				this->m_shader->load(this->m_transformMatrixLocation, instanceData.transform);
				for (const auto& mesh : model->getMeshes()) {
					glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, (void*)(mesh.indexOffset * sizeof(GLuint)));
				}
			}

		}
		// animated model
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, this->m_boneTransformStorageBuffer);
		this->m_shader->loadFloat("u_isStatic", 0);
		for (auto& instance : this->m_animatedInstances) {
			auto model = instance.animator->getModel();
			model->bind();

			this->m_shader->load(this->m_transformMatrixLocation, instance.transformationMatrix);
			auto& boneList = instance.animator->getBoneTransformations();
			glNamedBufferSubData(this->m_boneTransformStorageBuffer, 0, boneList.size() * sizeof(glm::mat4), boneList.data());
			for (const auto& mesh : model->getMeshes()) {
				glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, (void*)(mesh.indexOffset * sizeof(GLuint)));
			}

		}

		this->m_shader->unbind();
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void OpenGLShadowRenderer::setCamera(Camera* camera)
	{
		this->m_shadowBox->setCamera(camera);
	}

	void OpenGLShadowRenderer::setSunLight(const DirectionLight& light)
	{
		this->m_sunLight = light;
	}

}
