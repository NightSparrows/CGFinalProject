
#include "nspch.h"

#include <TrashEngine/graphics/Camera.h>

namespace TrashEngine {

	

	Camera::Camera(float aspectRatio, float fov, float zNear, float zFar) :
		position(0, 0, 0), rotation(glm::identity<glm::quat>()), aspectRatio(aspectRatio), fov(fov), zNear(zNear), zFar(zFar)
	{
		this->updateProjectionMatrix();
		this->updateViewMatrix();
	}

	NS_API void Camera::updateProjectionMatrix()
	{
		this->m_projectionMatrix = glm::perspective(this->fov, this->aspectRatio, this->zNear, this->zFar);
	}

	NS_API void Camera::updateViewMatrix()
	{
		glm::vec3 front = this->rotation * glm::vec3(0, 0, -1);
		glm::vec3 up = this->rotation * glm::vec3(0, 1, 0);

		this->m_viewMatrix = glm::lookAt(this->position, this->position + front, up);
	}

	NS_API const glm::mat4& Camera::getProjectionMatrix() const
	{
		return this->m_projectionMatrix;
	}

	NS_API const glm::mat4& Camera::getViewMatrix() const
	{
		return this->m_viewMatrix;
	}

}

