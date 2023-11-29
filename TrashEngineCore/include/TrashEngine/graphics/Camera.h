#pragma once

#include <TrashEngine/core/Base.h>
#include <TrashEngine/deps/glm/glm.hpp>
#include <TrashEngine/deps/glm/gtc/type_ptr.hpp>

namespace TrashEngine {

	class Camera
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="aspectRatio"></param>
		/// <param name="fov"></param>
		/// <param name=""></param>
		/// <param name=""></param>
		/// <returns></returns>
		NS_API Camera(float aspectRatio, float fov, float zNear, float zFar);

		virtual ~Camera() = default;

		NS_API void updateProjectionMatrix();

		NS_API void updateViewMatrix();

		NS_API const glm::mat4& getProjectionMatrix() const;

		NS_API const glm::mat4& getViewMatrix() const;

	public:
		glm::vec3 position;
		glm::quat rotation;
		float aspectRatio;
		float fov;
		float zNear;
		float zFar;

	protected:
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix;


	};

}
