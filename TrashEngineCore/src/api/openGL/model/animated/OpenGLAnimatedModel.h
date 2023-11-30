#pragma once

#include <deps/glad/glad.h>
#include <TrashEngine/graphics/model/animated/AnimatedModel.h>
#include <api/openGL/OpenGLMaterial.h>
#include "OpenGLAnimatedModelJoint.h"


namespace TrashEngine {

	class OpenGLAnimatedModel : public AnimatedModel
	{
	public:
		struct MeshInfo
		{
			OpenGLMaterial* material;
			uint32_t indexOffset;
			uint32_t indexCount;
		};

		struct BoneInfo
		{
			std::string name;
			glm::mat4 offsetMatrix;		// the origin pose that apply to this model with no animation
		};

		OpenGLAnimatedModel();
		~OpenGLAnimatedModel();

		bool loadData(AnimatedModelData* modelData) override;

		void bind();

		const std::vector<MeshInfo>& getMeshes() const { return this->m_meshes; }

		const std::vector<BoneInfo>& getBones() const { return this->m_bones; }

		Ref<const OpenGLAnimatedModelJoint> getRootJoint() const { return this->m_rootJoint;}

	protected:
		void deleteBuffers();

		void loadJoint(Ref<OpenGLAnimatedModelJoint> joint, Ref<AnimatedModelJointData> jointData);

	private:
		GLuint m_vaoHandle{ 0 };
		GLuint m_indexBufferHandle{ 0 };
		GLuint m_dataBufferHandle{ 0 };

		std::vector<BoneInfo> m_bones;
		std::vector<MeshInfo> m_meshes;
		Ref<OpenGLAnimatedModelJoint> m_rootJoint;

		std::vector<Scope<OpenGLMaterial>> m_materials;

	};

}
