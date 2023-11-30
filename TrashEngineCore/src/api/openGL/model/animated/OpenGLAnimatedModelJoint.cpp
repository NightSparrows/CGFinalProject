#include "nspch.h"
#include "OpenGLAnimatedModelJoint.h"

namespace TrashEngine {
	OpenGLAnimatedModelJoint::OpenGLAnimatedModelJoint(uint32_t boneID, glm::mat4 transformationMatrix) :
		m_boneID(boneID), m_transformationMatrix(transformationMatrix)
	{
	}
	void OpenGLAnimatedModelJoint::addChild(Ref<OpenGLAnimatedModelJoint> child)
	{
		this->m_children.push_back(child);
	}

}
