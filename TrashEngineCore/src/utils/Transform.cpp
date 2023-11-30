
#include "nspch.h"
#include <TrashEngine/deps/glm/gtx/quaternion.hpp>

#include <TrashEngine/utils/Transform.h>

#include <TrashEngine/deps/glm/gtx/matrix_decompose.hpp>


namespace TrashEngine {
	Transform::Transform(const glm::mat4& matrix)
	{
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(matrix, this->m_scale, this->m_rotation, this->m_position, skew, perspective);
	}

	NS_API void Transform::setScale(const glm::vec3& scale)
	{
		this->m_scale = scale;
		this->m_modified = true;
	}
	NS_API const glm::vec3& Transform::getScale() const
	{
		return this->m_scale;
	}

	NS_API void Transform::rotate(const glm::vec3& axis, float angle)
	{
		this->m_rotation = glm::rotate(this->m_rotation, glm::radians(angle), axis);
		this->m_modified = true;
	}

	NS_API void Transform::setRotation(const glm::quat& rotation)
	{
		this->m_rotation = rotation;
		this->m_modified = true;
	}

	NS_API const glm::quat& Transform::getRotation() const
	{
		return this->m_rotation;
	}
	NS_API void Transform::translate(const glm::vec3 displacement)
	{
		this->m_position += displacement;
		this->m_modified = true;
	}
	NS_API void Transform::setPosition(const glm::vec3& position)
	{
		this->m_position = position;
		this->m_modified = true;
	}
	NS_API const glm::vec3& Transform::getPosition() const
	{
		return this->m_position;
	}
	NS_API void Transform::updateMatrix()
	{
		this->m_matrix = glm::translate(glm::identity<glm::mat4>(), this->m_position);
		this->m_matrix *= glm::toMat4(this->m_rotation);
		this->m_matrix = glm::scale(this->m_matrix, this->m_scale);
		this->m_modified = false;
	}

	NS_API Transform::operator glm::mat4()
	{
		return this->matrix();
	}

	Transform::operator glm::mat4() const
	{
		glm::mat4 posMatrix = glm::translate(glm::mat4(1.f), this->m_position);
		glm::mat4 rotMatrix = glm::toMat4(this->m_rotation);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.f), this->m_scale);
		return posMatrix * rotMatrix * scaleMatrix;
	}

	NS_API glm::mat4 Transform::matrix()
	{
		if (this->m_modified) {
			this->updateMatrix();
		}
		return this->m_matrix;
	}

	NS_API Transform Transform::Interpolate(const Transform& a, const Transform& b, float progression)
	{
		Transform transform;
		transform.m_position = glm::mix(a.m_position, b.m_position, progression);
		transform.m_rotation = glm::slerp(a.m_rotation, b.m_rotation, progression);
		transform.m_scale = glm::mix(a.m_scale, b.m_scale, progression);
		transform.m_modified = true;
		return transform;
	}

}