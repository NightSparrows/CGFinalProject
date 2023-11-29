#pragma once

#include <TrashEngine/utils/Transform.h>

namespace TrashEngine {

	struct TransformComponent
	{

		Transform transform;

		TransformComponent(const Transform& trans)
		{
			this->transform = trans;
		}

		TransformComponent() : transform() {}
		TransformComponent(const TransformComponent& other)
		{
			this->transform = other.transform;
		}

	};

}
