#include "nspch.h"
#include "OpenGLStaticModel.h"

namespace TrashEngine {

	OpenGLStaticModel::OpenGLStaticModel()
	{
		glCreateVertexArrays(1, &this->m_vaoHandle);
	}

	OpenGLStaticModel::~OpenGLStaticModel()
	{
		this->deleteBuffers();
		glDeleteVertexArrays(1, &this->m_vaoHandle);
	}

	bool OpenGLStaticModel::loadData(StaticModelData* modelData)
	{
		this->deleteBuffers();

		// TODO load materials
		for (auto& materialData : modelData->materials) {
			auto material = CreateScope<OpenGLMaterial>();

			material->setDiffuseColor(materialData.diffuseColor);
			material->setAmbient(materialData.ambient);
			material->setDiffuse(materialData.diffuse);
			material->setSpecular(materialData.specular);
			material->setReflectivity(materialData.reflectivity);
			material->setShininess(materialData.shininess);

			if (!materialData.diffuseTexture.empty()) {
				material->loadDiffuseTexture(materialData.diffuseTexture);
			}

			if (!materialData.normalTexture.empty()) {
				material->loadNormalTexture(materialData.normalTexture);
			}
			
			this->m_materials.emplace_back(std::move(material));
		}

		GLuint buffers[2];
		glCreateBuffers(2, buffers);
		this->m_dataBufferHandle = buffers[0];
		this->m_indexBufferHandle = buffers[1];
		glNamedBufferData(this->m_indexBufferHandle, modelData->indices.size() * sizeof(uint32_t), modelData->indices.data(), GL_STATIC_DRAW);
		glNamedBufferData(this->m_dataBufferHandle, modelData->vertices.size() * sizeof(StaticModelData::VertexData), modelData->vertices.data(), GL_STATIC_DRAW);

		glVertexArrayElementBuffer(this->m_vaoHandle, this->m_indexBufferHandle);
		glVertexArrayVertexBuffer(this->m_vaoHandle, 0, this->m_dataBufferHandle, 0, sizeof(StaticModelData::VertexData));

		glEnableVertexArrayAttrib(this->m_vaoHandle, 0);
		glVertexArrayAttribBinding(this->m_vaoHandle, 0, 0);
		glVertexArrayAttribFormat(this->m_vaoHandle, 0, 3, GL_FLOAT, GL_FALSE, offsetof(StaticModelData::VertexData, position));

		glEnableVertexArrayAttrib(this->m_vaoHandle, 1);
		glVertexArrayAttribBinding(this->m_vaoHandle, 1, 0);
		glVertexArrayAttribFormat(this->m_vaoHandle, 1, 3, GL_FLOAT, GL_FALSE, offsetof(StaticModelData::VertexData, normal));

		glEnableVertexArrayAttrib(this->m_vaoHandle, 2);
		glVertexArrayAttribBinding(this->m_vaoHandle, 2, 0);
		glVertexArrayAttribFormat(this->m_vaoHandle, 2, 3, GL_FLOAT, GL_FALSE, offsetof(StaticModelData::VertexData, tangent));

		glEnableVertexArrayAttrib(this->m_vaoHandle, 3);
		glVertexArrayAttribBinding(this->m_vaoHandle, 3, 0);
		glVertexArrayAttribFormat(this->m_vaoHandle, 3, 2, GL_FLOAT, GL_FALSE, offsetof(StaticModelData::VertexData, texCoord));
 
		this->m_meshes.resize(modelData->meshes.size());
		for (uint32_t i = 0; i < modelData->meshes.size(); i++) {
			this->m_meshes[i].indexCount = modelData->meshes[i].indexCount;
			this->m_meshes[i].indexOffset = modelData->meshes[i].indexOffset;
			this->m_meshes[i].material = this->m_materials[modelData->meshes[i].materialIndex].get();
		}

		return true;
	}

	void OpenGLStaticModel::bind()
	{
		glBindVertexArray(this->m_vaoHandle);
	}

	void OpenGLStaticModel::deleteBuffers()
	{
		if (this->m_dataBufferHandle != 0) {
			GLuint buffers[2] = { this->m_dataBufferHandle, this->m_indexBufferHandle };
			glDeleteBuffers(2, buffers);
			this->m_dataBufferHandle = 0;
			this->m_indexBufferHandle = 0;
		}
	}

}
