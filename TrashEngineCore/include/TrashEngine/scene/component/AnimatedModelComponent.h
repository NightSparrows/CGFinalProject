#pragma once

#include <TrashEngine/core/Base.h>
#include <TrashEngine/graphics/model/animated/AnimatedModel.h>

namespace TrashEngine {

	struct AnimatedModelComponent
	{
		Ref<AnimatedModel> model;

		AnimatedModelComponent(Ref<AnimatedModel> model)
		{
			this->model = model;
		}

		AnimatedModelComponent() = default;
		AnimatedModelComponent(const AnimatedModelComponent&) = default;

	};

}
