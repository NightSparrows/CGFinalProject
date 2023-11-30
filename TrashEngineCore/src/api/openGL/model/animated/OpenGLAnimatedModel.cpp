#include "nspch.h"
#include "OpenGLAnimatedModel.h"

namespace TrashEngine {

    OpenGLAnimatedModel::OpenGLAnimatedModel()
    {
        glCreateVertexArrays(1, &this->m_vaoHandle);
    }

    OpenGLAnimatedModel::~OpenGLAnimatedModel()
    {
        this->deleteBuffers();
        glDeleteVertexArrays(1, &this->m_vaoHandle);
    }

    bool OpenGLAnimatedModel::loadData(AnimatedModelData* modelData)
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
		glNamedBufferData(this->m_dataBufferHandle, modelData->vertices.size() * sizeof(AnimatedModelData::VertexData), modelData->vertices.data(), GL_STATIC_DRAW);

		glVertexArrayElementBuffer(this->m_vaoHandle, this->m_indexBufferHandle);
		glVertexArrayVertexBuffer(this->m_vaoHandle, 0, this->m_dataBufferHandle, 0, sizeof(AnimatedModelData::VertexData));

		glEnableVertexArrayAttrib(this->m_vaoHandle, 0);
		glVertexArrayAttribBinding(this->m_vaoHandle, 0, 0);
		glVertexArrayAttribFormat(this->m_vaoHandle, 0, 3, GL_FLOAT, GL_FALSE, offsetof(AnimatedModelData::VertexData, position));

		glEnableVertexArrayAttrib(this->m_vaoHandle, 1);
		glVertexArrayAttribBinding(this->m_vaoHandle, 1, 0);
		glVertexArrayAttribFormat(this->m_vaoHandle, 1, 3, GL_FLOAT, GL_FALSE, offsetof(AnimatedModelData::VertexData, normal));

		glEnableVertexArrayAttrib(this->m_vaoHandle, 2);
		glVertexArrayAttribBinding(this->m_vaoHandle, 2, 0);
		glVertexArrayAttribFormat(this->m_vaoHandle, 2, 3, GL_FLOAT, GL_FALSE, offsetof(AnimatedModelData::VertexData, tangent));

		glEnableVertexArrayAttrib(this->m_vaoHandle, 3);
		glVertexArrayAttribBinding(this->m_vaoHandle, 3, 0);
		glVertexArrayAttribFormat(this->m_vaoHandle, 3, 2, GL_FLOAT, GL_FALSE, offsetof(AnimatedModelData::VertexData, texCoord));

		glEnableVertexArrayAttrib(this->m_vaoHandle, 4);
		glVertexArrayAttribBinding(this->m_vaoHandle, 4, 0);
		glVertexArrayAttribIFormat(this->m_vaoHandle, 4, 4, GL_INT, offsetof(AnimatedModelData::VertexData, boneID));

		glEnableVertexArrayAttrib(this->m_vaoHandle, 5);
		glVertexArrayAttribBinding(this->m_vaoHandle, 5, 0);
		glVertexArrayAttribFormat(this->m_vaoHandle, 5, 4, GL_FLOAT, GL_FALSE, offsetof(AnimatedModelData::VertexData, weight));

		this->m_meshes.resize(modelData->meshes.size());
		for (uint32_t i = 0; i < modelData->meshes.size(); i++) {
			this->m_meshes[i].indexCount = modelData->meshes[i].indexCount;
			this->m_meshes[i].indexOffset = modelData->meshes[i].indexOffset;
			this->m_meshes[i].material = this->m_materials[modelData->meshes[i].materialIndex].get();
		}

		this->m_bones.resize(modelData->bones.size());
		for (uint32_t i = 0; i < modelData->bones.size(); i++) {
			this->m_bones[i].name = modelData->bones[i].name;
			this->m_bones[i].offsetMatrix = modelData->bones[i].offsetMatrix;
		}

		this->m_rootJoint = CreateRef<OpenGLAnimatedModelJoint>(modelData->rootJoint->boneID, modelData->rootJoint->transformation);
		this->loadJoint(this->m_rootJoint, modelData->rootJoint);

		return true;
    }

    void OpenGLAnimatedModel::bind()
    {
        glBindVertexArray(this->m_vaoHandle);
    }

    void OpenGLAnimatedModel::deleteBuffers()
    {
        if (this->m_dataBufferHandle != 0) {
            GLuint buffers[2] = { this->m_dataBufferHandle, this->m_indexBufferHandle };
            glDeleteBuffers(2, buffers);
            this->m_dataBufferHandle = 0;
            this->m_indexBufferHandle = 0;
        }
    }

	void OpenGLAnimatedModel::loadJoint(Ref<OpenGLAnimatedModelJoint> joint, Ref<AnimatedModelJointData> jointData)
	{
		joint->m_name = jointData->name;
		for (auto childData : jointData->children) {
			auto childJoint = CreateRef<OpenGLAnimatedModelJoint>(childData->boneID, childData->transformation);
			loadJoint(childJoint, childData);
			joint->addChild(childJoint);
		}
	}

}
