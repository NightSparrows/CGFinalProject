#include "nspch.h"
#include <TrashEngine/scene/component/TransformComponent.h>
#include <TrashEngine/scene/component/StaticModelComponent.h>
#include <api/openGL/builder/OpenGLShaderProgramBuilder.h>
#include "OpenGLStaticModelRenderer.h"

namespace TrashEngine {

	OpenGLStaticModelRenderer::OpenGLStaticModelRenderer()
	{
		OpenGLShaderProgramBuilder shaderBuilder;
		shaderBuilder.addShaderFromFile("assets/TrashEngine/shaders/deferredShading/staticModel/shader.vert", GL_VERTEX_SHADER);
		shaderBuilder.addShaderFromFile("assets/TrashEngine/shaders/deferredShading/staticModel/shader.frag", GL_FRAGMENT_SHADER);
		this->m_shader = Scope<OpenGLShaderProgram>(shaderBuilder.build());

		glUniformBlockBinding(this->m_shader->getHandle(), glGetUniformBlockIndex(this->m_shader->getHandle(), "globalBuffer"), 0);
		this->m_transformationMatrixLocation = this->m_shader->getUniformLocationFromName("u_transformationMatrix");
		this->m_tiModelMatrixLocation = this->m_shader->getUniformLocationFromName("u_tiModelMatrix");
		this->m_shader->load(this->m_shader->getUniformLocationFromName("diffuseSampler"), 0);
		this->m_shader->load(this->m_shader->getUniformLocationFromName("normalTextureSampler"), 1);

		glCreateBuffers(1, &this->m_materialStorageBuffer);
		glNamedBufferStorage(this->m_materialStorageBuffer, sizeof(MaterialData), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);


	}

	void OpenGLStaticModelRenderer::prepareScene(Scene* scene)
	{
		this->m_entityList.clear();
		auto view = scene->Reg().view<TransformComponent, StaticModelComponent>();
		for (auto [entity, transform, model] : view.each()) {
			Ref<OpenGLStaticModel> ogModel = StaticCastRef<OpenGLStaticModel>(model.model);
			auto& list = this->m_entityList[ogModel];
			auto& instanceData = list.emplace_back();
			instanceData.transform = transform.transform;
		}
	}

	void OpenGLStaticModelRenderer::render()
	{
		this->m_shader->bind();

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, this->m_materialStorageBuffer);
		for (auto [model, instanceDataList] : this->m_entityList) {
			model->bind();

			for (const auto& instanceData : instanceDataList) {
				this->m_shader->load(this->m_transformationMatrixLocation, instanceData.transform);
				this->m_shader->load(this->m_tiModelMatrixLocation, glm::transpose(glm::inverse(instanceData.transform)));
				for (const auto& mesh : model->getMeshes()) {
					MaterialData materialData{};
					materialData.ambient = mesh.material->getAmbient();
					materialData.diffuse = mesh.material->getDiffuse();
					materialData.specular = mesh.material->getSpecular();
					materialData.shininess = mesh.material->getShininess();// TODO
					materialData.reflectivity = mesh.material->getReflectivity();
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

					glNamedBufferSubData(this->m_materialStorageBuffer, 0, sizeof(MaterialData), &materialData);
					glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, (void*)(mesh.indexOffset * sizeof(GLuint)));

				}
			}

		}

		this->m_shader->unbind();
	}

}

