#pragma once

#include <TrashEngine/core/Base.h>

#include <TrashEngine/deps/glm/glm.hpp>
#include <TrashEngine/deps/glm/ext/quaternion_float.hpp>

namespace TrashEngine {

	class Transform
	{
	public:

		NS_API void setScale(const glm::vec3& scale);

		NS_API const glm::vec3& getScale() const;

		// rotate the transfrom 
		NS_API void rotate(const glm::vec3& axis, float angle);

		NS_API void setRotation(const glm::quat& rotation);

		NS_API const glm::quat& getRotation() const;

		// transform base on current position
		NS_API void translate(const glm::vec3 displacement);

		// set the position absolutly
		NS_API void setPosition(const glm::vec3& position);

		NS_API const glm::vec3& getPosition() const;

		// update the matrix
		NS_API void updateMatrix();

		NS_API operator glm::mat4();

		NS_API glm::mat4 matrix();

	private:
		bool m_modified{false};
		glm::mat4 m_matrix{1.f};
		glm::vec3 m_position{0};
		glm::quat m_rotation = glm::identity<glm::quat>();
		glm::vec3 m_scale{1.f};
	};

}
