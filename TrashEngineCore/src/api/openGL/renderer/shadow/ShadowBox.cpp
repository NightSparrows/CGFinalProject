#include "nspch.h"

#include <TrashEngine/deps/glm/gtx/transform.hpp>
#include <TrashEngine/deps/glm/gtx/quaternion.hpp>
#include "ShadowBox.h"

namespace TrashEngine {

	ShadowBox::ShadowBox() :
		m_camera(nullptr), m_offset(15.f), m_lightViewMatrix(glm::identity<glm::mat4>()), m_projectionMatrix(glm::identity<glm::mat4>())
	{
	}

	void ShadowBox::update(glm::vec3 lightDirection)
	{
		if (!this->m_camera)
			return;
		float nearDist = this->m_camera->zNear;
		float farDist = 200.f;					// shadow distance
		float Hnear = 2 * glm::tan(this->m_camera->fov / 2) * nearDist;
		float Wnear = Hnear * this->m_camera->aspectRatio;
		float Hfar = 2 * tan(this->m_camera->fov / 2) * farDist;
		//float Hfar = 2 * tan(this->m_camera->fov / 2) * 100.f;
		float Wfar = Hfar * this->m_camera->aspectRatio;

		glm::mat4 rotation = this->calculateCameraRotationMatrix();
		glm::vec3 forwardVector = glm::vec3(rotation * glm::vec4(0, 0, -1, 0));	// camera forward vector
		glm::vec3 upVector = rotation * glm::vec4(0, 1, 0, 0);
		glm::vec3 rightVector = glm::cross(forwardVector, upVector);
		glm::vec3 centerFar = this->m_camera->position + (forwardVector * farDist/* or shadow dis? */);

		glm::vec3 topLeftFar = centerFar + (upVector * (Hfar / 2)) - (rightVector * (Wfar / 2));
		glm::vec3 topRightFar = centerFar + (upVector * (Hfar / 2)) + (rightVector * (Wfar / 2));
		glm::vec3 bottomLeftFar = centerFar - (upVector * (Hfar / 2)) - (rightVector * (Wfar / 2));
		glm::vec3 bottomRightFar = centerFar - (upVector * (Hfar / 2)) + (rightVector * (Wfar / 2));

		glm::vec3 centerNear = this->m_camera->position + (forwardVector * nearDist);

		glm::vec3 topLeftNear = centerNear + (upVector * (Hnear / 2)) - (rightVector * (Wnear / 2));
		glm::vec3 topRightNear = centerNear + (upVector * (Hnear / 2)) + (rightVector * (Wnear / 2));
		glm::vec3 bottomLeftNear = centerNear - (upVector * (Hnear / 2)) - (rightVector * (Wnear / 2));
		glm::vec3 bottomRightNear = centerNear - (upVector * (Hnear / 2)) + (rightVector * (Wnear / 2));

		glm::vec3 frustumCenter = (centerFar - centerNear) * 0.5f;

		glm::mat4 lightView = glm::lookAt(glm::normalize(-lightDirection), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));

		std::array<glm::vec3, 8> frustumToLightView
		{
			lightView * glm::vec4(bottomRightNear, 1.0f),
			lightView * glm::vec4(topRightNear, 1.0f),
			lightView * glm::vec4(bottomLeftNear, 1.0f),
			lightView * glm::vec4(topLeftNear, 1.0f),
			lightView * glm::vec4(bottomRightFar, 1.0f),
			lightView * glm::vec4(topRightFar, 1.0f),
			lightView * glm::vec4(bottomLeftFar, 1.0f),
			lightView * glm::vec4(topLeftFar, 1.0f)
		};

		// find max and min points to define a ortho matrix around
		glm::vec3 min{ INFINITY, INFINITY, INFINITY };
		glm::vec3 max{ -INFINITY, -INFINITY, -INFINITY };
		for (unsigned int i = 0; i < frustumToLightView.size(); i++) {
			if (frustumToLightView[i].x < min.x)
				min.x = frustumToLightView[i].x;
			if (frustumToLightView[i].y < min.y)
				min.y = frustumToLightView[i].y;
			if (frustumToLightView[i].z < min.z)
				min.z = frustumToLightView[i].z;

			if (frustumToLightView[i].x > max.x)
				max.x = frustumToLightView[i].x;
			if (frustumToLightView[i].y > max.y)
				max.y = frustumToLightView[i].y;
			if (frustumToLightView[i].z > max.z)
				max.z = frustumToLightView[i].z;
		}

		min.z -= this->m_offset;
		max.z += this->m_offset;
		//min.x -= this->m_offset;
		//max.x += this->m_offset;
		//min.y -= this->m_offset;
		//max.y += this->m_offset;

		float l = min.x;
		float r = max.x;
		float b = min.y;
		float t = max.y;
		// because max.z is positive and in NDC the positive z axis is 
		// towards us so need to set it as the near plane flipped same for min.z.
		float n = -max.z;
		float f = -min.z;

		// finally, set our ortho projection
		// and create the light space view-projection matrix
		this->m_projectionMatrix = glm::ortho(l, r, b, t, n, f);
		this->m_lightViewMatrix = lightView;

	}

	glm::mat4 ShadowBox::calculateCameraRotationMatrix()
	{
		//glm::mat4 rotation = glm::mat4(1.f);
		//rotation = glm::rotate(rotation, glm::radians(-this->m_camera->getRotation().y), glm::vec3(0, 1, 0));
		//rotation = glm::rotate(rotation, glm::radians(-this->m_camera->getRotation().x), glm::vec3(1, 0, 0));
		//return rotation;

		return glm::toMat4(this->m_camera->rotation);
	}

	glm::mat4 ShadowBox::getProjectionMatrix()
	{
		return this->m_projectionMatrix;
	}

	glm::mat4 ShadowBox::getLightViewMatrix()
	{
		return this->m_lightViewMatrix;
	}

	void ShadowBox::setCamera(Camera* camera)
	{
		this->m_camera = camera;
	}

}
