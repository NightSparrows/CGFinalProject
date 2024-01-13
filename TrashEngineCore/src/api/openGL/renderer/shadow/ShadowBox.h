#pragma once

#include <TrashEngine/graphics/Camera.h>

#define SHADOW_DISTANCE 100.0f
#define SHADOW_OFFSET 10.f;

namespace TrashEngine {

	class ShadowBox
	{

	public:
		ShadowBox();

		void update(glm::vec3 lightDirection);

		glm::mat4 getProjectionMatrix();
		glm::mat4 getLightViewMatrix();

		void setCamera(Camera* camera);

	private:

		glm::mat4 calculateCameraRotationMatrix();

	private:

		float m_offset;		// offset of the box

		glm::mat4 m_lightViewMatrix;
		glm::mat4 m_projectionMatrix;

		Camera* m_camera;

	};

}
