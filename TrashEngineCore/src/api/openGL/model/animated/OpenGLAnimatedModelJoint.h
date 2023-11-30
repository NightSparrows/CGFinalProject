#pragma once

#include <TrashEngine/core/Base.h>
#include <TrashEngine/deps/glm/glm.hpp>

namespace TrashEngine {
	class OpenGLAnimatedModelJoint
	{
		friend class OpenGLAnimatedModel;
	public:
		OpenGLAnimatedModelJoint(uint32_t boneID, glm::mat4 transformationMatrix);

		const std::vector<Ref<OpenGLAnimatedModelJoint>>& getChildren() const { return this->m_children; }

		const glm::mat4& getTransformationMatrix() const { return this->m_transformationMatrix; }

		inline uint32_t getBoneID() const { return this->m_boneID; }

		inline const std::string& getName() const { return this->m_name; }

	protected:
		void addChild(Ref<OpenGLAnimatedModelJoint> child);

	private:
		std::vector<Ref<OpenGLAnimatedModelJoint>> m_children;
		uint32_t m_boneID;
		glm::mat4 m_transformationMatrix;
		std::string m_name;

	};
}
