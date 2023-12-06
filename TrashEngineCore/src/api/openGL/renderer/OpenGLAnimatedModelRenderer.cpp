#include "nspch.h"
#include <TrashEngine/scene/component/TransformComponent.h>
#include <TrashEngine/scene/component/AnimatedModelComponent.h>
#include <TrashEngine/scene/component/AnimatedModelAnimatorComponent.h>
#include <api/openGL/builder/OpenGLShaderProgramBuilder.h>
#include "OpenGLAnimatedModelRenderer.h"

namespace TrashEngine {

	OpenGLAnimatedModelRenderer::OpenGLAnimatedModelRenderer()
	{
		OpenGLShaderProgramBuilder shaderBuilder;
		shaderBuilder.addShaderFromFile("assets/TrashEngine/shaders/deferredShading/animatedModel/shader.vert", GL_VERTEX_SHADER);
		shaderBuilder.addShaderFromFile("assets/TrashEngine/shaders/deferredShading/animatedModel/shader.frag", GL_FRAGMENT_SHADER);
		this->m_shader = Scope<OpenGLShaderProgram>(shaderBuilder.build());

		glUniformBlockBinding(this->m_shader->getHandle(), glGetUniformBlockIndex(this->m_shader->getHandle(), "globalBuffer"), 0);
		this->m_transformationMatrixLocation = this->m_shader->getUniformLocationFromName("u_transformationMatrix");
		this->m_tiModelMatrixLocation = this->m_shader->getUniformLocationFromName("u_tiModelMatrix");
		this->m_shader->load(this->m_shader->getUniformLocationFromName("diffuseSampler"), 0);
		this->m_shader->load(this->m_shader->getUniformLocationFromName("normalTextureSampler"), 1);

		glCreateBuffers(1, &this->m_materialStorageBuffer);
		glNamedBufferStorage(this->m_materialStorageBuffer, sizeof(MaterialData), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);

		glCreateBuffers(1, &this->m_boneTransformStorageBuffer);
		glNamedBufferStorage(this->m_boneTransformStorageBuffer, 1000 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);

	}

	OpenGLAnimatedModelRenderer::~OpenGLAnimatedModelRenderer()
	{
		GLuint buffers[2] = { this->m_boneTransformStorageBuffer, this->m_materialStorageBuffer };
		glDeleteBuffers(2, buffers);
	}

	void OpenGLAnimatedModelRenderer::prepareScene(Scene* scene)
	{
		this->m_instances.clear();
		auto view = scene->Reg().view<TransformComponent, AnimatedModelAnimatorComponent>();
		for (auto [entity, transform, animator] : view.each()) {
			Ref<OpenGLAnimatedModelAnimator> ogAnimator = StaticCastRef<OpenGLAnimatedModelAnimator>(animator.animator);
			auto& instanceData = this->m_instances.emplace_back();
			instanceData.transformationMatrix = transform.transform;
			instanceData.animator = ogAnimator;
		}
	}

	void OpenGLAnimatedModelRenderer::render()
	{
		this->m_shader->bind();

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, this->m_materialStorageBuffer);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, this->m_boneTransformStorageBuffer);
		for (auto& instance : this->m_instances) {
			auto model = instance.animator->getModel();
			model->bind();

			this->m_shader->load(this->m_transformationMatrixLocation, instance.transformationMatrix);
			this->m_shader->load(this->m_tiModelMatrixLocation, glm::transpose(glm::inverse(instance.transformationMatrix)));

			// for testing
			auto& boneList = instance.animator->getBoneTransformations();
			glNamedBufferSubData(this->m_boneTransformStorageBuffer, 0, boneList.size() * sizeof(glm::mat4), boneList.data());
			for (const auto& mesh : model->getMeshes()) {
				MaterialData materialData{};
				// TODO
				materialData.diffuseColor = mesh.material->getDiffuseColor();
				if (mesh.material->getDiffuseTexture() != nullptr) {
					mesh.material->getDiffuseTexture()->bindUnit(0);
					materialData.hasDiffuseTexture = 1;
				}
				else {
					materialData.hasDiffuseTexture = 0;
				}
				materialData.hasNormalTexture = 0;
				if (mesh.material->getNormalTexture() != nullptr) {
					mesh.material->getNormalTexture()->bindUnit(1);
					materialData.hasNormalTexture = 1;
				}
				materialData.ao = mesh.material->getAO();
				materialData.roughness = mesh.material->getRoughness();
				materialData.metallic = mesh.material->getMetallic();

				glNamedBufferSubData(this->m_materialStorageBuffer, 0, sizeof(MaterialData), &materialData);
				glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, (void*)(mesh.indexOffset * sizeof(GLuint)));
			}

		}

		this->m_shader->unbind();
	}

}

